"""
Generates the screenshots (view-*.png, menu-*.png) and the title images
(radpro-title-tryme.png, radpro-media.png) in docs/img from the firmware.

Builds the simulator firmware (platform.io/src, with the flags of the
radpro-color-landscape simulator target in CMakeLists.txt) into a headless
host program (harness.c), runs one script per screenshot and writes the
images at 2x, like the simulator window. Screenshots are deterministic: the
simulated pulses use a fixed seed, and the clock runs with the simulation.

Each script starts from a freshly powered-on counter with default settings
(no saved state). See harness.c for the script commands; in addition,
"clock HH:MM" sets the time shown in the title bar.

Usage: python generate.py [--out DIR] [--list] [name ...]
Needs gcc on the PATH (on Windows, for example MSYS2 UCRT64; run it from
PowerShell or cmd) and Pillow.
"""

import argparse
import calendar
import concurrent.futures
import glob
import os
import re
import subprocess
import sys
import tempfile

from PIL import Image, ImageDraw, ImageFont

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..', '..'))
FIRMWARE = os.path.join(ROOT, 'platform.io')
SRC = os.path.join(FIRMWARE, 'src')
IMG = os.path.join(ROOT, 'docs', 'img')

LANGUAGE = 'en'

# Flags of the radpro-color-landscape simulator target (CMakeLists.txt),
# without SDL (MCURENDERER_SDL) and the serial port (SIMULATOR_COMM)
CFLAGS = [
    '-std=c2x', '-O1', '-w',
    '-DLANGUAGE="%s"' % LANGUAGE,
    '-DSTRINGS="strings/%s.h"' % LANGUAGE,
    '-DMCUMAX_PAWN_EVAL_DISABLED',
    '-DSIMULATOR',
    '-DFLASH_BASE_=0x0', '-DFLASH_SIZE_=0x10000', '-DFLASH_PAGE_SIZE=0x400',
    '-DFLASH_WORD_SIZE=0x2', '-DFIRMWARE_BASE=0', '-DFIRMWARE_SIZE=0',
    '-DSTART_POWERED',
    '-DTUBE_HV_PWM',
    '-DSIMULATE_PULSES',
    '-DKEYBOARD_5_KEYS', '-DKEYBOARD_KEY_POWER_OK',
    '-DBUZZER',
    '-DVIBRATOR',
    '-DPULSE_LED',
    '-DGAME',
    '-DDISPLAY_320X240', '-DDISPLAY_COLOR',
    '-DFONT_SYMBOLS="fonts/font_symbols_color.h"',
    '-DFONT_LARGE="fonts/font_large_color_115.h"',
    '-DFONT_SMALL="fonts/font_small_%s_color_21.h"' % LANGUAGE,
    '-DFONT_MEDIUM="fonts/font_medium_%s_color_32.h"' % LANGUAGE,
]
INCLUDES = [SRC, os.path.join(SRC, 'system'), os.path.join(FIRMWARE, 'lib', 'mcu-max'),
            os.path.join(FIRMWARE, 'lib', 'mcu-renderer')]

# Replaced by harness.c
REPLACED = ['main.c', 'sdl/sdlsim_display.c', 'sdl/sdlsim_keyboard.c',
            'sdl/sdlsim_buzzer.c', 'sdl/sdlsim_rtc.c']

SCALE = 2

# Highlighted rectangle (left, top, right, bottom, in display pixels): the
# rest of the image is dimmed
BAR_HIGHLIGHT = (1, 158, 184, 240)
LOCK_HIGHLIGHT = (142, 6, 174, 39)

# Common setup: display always on, rate warning at 1 µSv/h and rate alarm
# at 10 µSv/h (rateAlerts[] indices in measurements/pulses.c)
SETUP = '''
set displaySleep 5
set rateWarning 2
set rateAlarm 5
wait 3
'''

SCENARIOS = {
    'view-instantaneous': (SETUP + '''
usvh 0.2
wait 120
clock 15:14
screenshot
''', None),

    'view-visualbar': (SETUP + '''
usvh 0.2
wait 120
clock 15:14
screenshot
''', BAR_HIGHLIGHT),

    # The alert icon flashes with the heartbeat: wait for an "on" second
    'view-alert': (SETUP + '''
usvh 25
wait 61
clock 15:30
screenshot
''', None),

    'view-average': (SETUP + '''
usvh 0.17
press down
wait 367
clock 15:18
screenshot
''', None),

    'view-cumulative': (SETUP + '''
usvh 0.17
press down
press down
wait 482
clock 15:20
screenshot
''', None),

    # Levels high enough that 5 s bins stay within one decade
    'view-history': (SETUP + '''
usvh 0.24
press up
wait 180
usvh 0.19
wait 120
usvh 0.32
wait 90
usvh 0.22
wait 240
clock 15:22
screenshot
''', None),

    'view-lock': (SETUP + '''
usvh 0.2
wait 120
press left+ok 1200
press up
wait 1
clock 15:35
screenshot
''', LOCK_HIGHLIGHT),

    'view-settings': (SETUP + '''
press right
clock 15:38
screenshot
''', None),

    'view-datalog': (SETUP + '''
press right
press down
press down
press down
press down
press right
clock 15:43
screenshot
''', None),

    # Settings > Measurements > Instantaneous
    'menu-instantaneous': (SETUP + '''
press right
press down
press down
press right
press down
press down
press down
press right
clock 15:45
screenshot
''', None),

    'view-rng': ('''
set displaySleep 5
wait 3
cps 50
press right
press up
press up
press up
press right
press down
press right
wait 20
clock 15:40
screenshot
''', None),
}


# Title images (README, simulator page): background radiation (long enough
# that the 10 minute history starts after power-on), then the last 30
# seconds at 50 µSv/h (history, Dusk theme). Then the level drops to
# 2.5 µSv/h, a warning (instantaneous rate, Night theme; statistics, Day
# theme). The alert icon flashes with the heartbeat: screenshots land on
# "on" seconds.
TITLE = SETUP + '''
usvh 0.2
press up
wait 700
usvh 50
wait 31
clock 12:32
set displayTheme 1
screenshot history
usvh 2.5
press down
wait 20
set displayTheme 2
screenshot instantaneous
press right
press up
press right
set displayTheme 0
screenshot statistics
'''

# Photo of a counter with a monochrome display (not generated)
PHOTO = os.path.join(HERE, 'monochrome-photo.png')

# Composite images: script, rows of tiles (screenshot names or image
# files), and whether to add the "Try me!" ribbon
COMPOSITES = {
    'radpro-title-tryme': (TITLE, [['instantaneous', PHOTO], ['history', 'statistics']], True),
    'radpro-media': (TITLE, [['instantaneous', 'history']], False),
}

# "Try me!" ribbon: a black band across the bottom right corner, between
# the lines x + y = RIBBON_START and x + y = RIBBON_END (image pixels)
RIBBON_START = 1840
RIBBON_END = 2008
RIBBON_TEXT = 'Try me!'
RIBBON_FONT = os.path.join(os.environ.get('WINDIR', r'C:\Windows'), 'Fonts', 'arialbd.ttf')
RIBBON_TEXT_LENGTH = 209
RIBBON_TEXT_CENTRE = 319  # along the band, as x - y


# Build

def generate_settings_setter(path):
    """Writes setSettingsField(): sets a Settings bitfield by name."""
    header = open(os.path.join(SRC, 'system', 'settings.h'), encoding='utf-8').read()
    struct = re.search(r'typedef struct\s*\{([^}]*)\}\s*Settings;', header).group(1)
    fields = re.findall(r'unsigned int (\w+)\s*:\s*\d+;', struct)

    with open(path, 'w', encoding='utf-8') as f:
        f.write('#include <string.h>\n\n#include "system/settings.h"\n\n'
                'bool setSettingsField(const char *name, int32_t value)\n{\n')
        for field in fields:
            f.write('    if (!strcmp(name, "%s"))\n    {\n        settings.%s = value;\n\n'
                    '        return true;\n    }\n' % (field, field))
        f.write('\n    return false;\n}\n')


def build(directory):
    sources = [os.path.join(HERE, 'harness.c'), os.path.join(directory, 'setters.c'),
               os.path.join(FIRMWARE, 'lib', 'mcu-renderer', 'mcu-renderer.c')]
    sources += glob.glob(os.path.join(FIRMWARE, 'lib', 'mcu-max', '*.c'))
    replaced = {os.path.normpath(os.path.join(SRC, path)) for path in REPLACED}
    sources += [path for path in glob.glob(os.path.join(SRC, '**', '*.c'), recursive=True)
                if os.path.normpath(path) not in replaced]

    generate_settings_setter(sources[1])

    includes = ['-I' + path for path in INCLUDES]

    def compile_source(index_source):
        index, source = index_source
        obj = os.path.join(directory, '%03d.o' % index)
        result = subprocess.run(['gcc', '-c'] + CFLAGS + includes + [source, '-o', obj],
                                capture_output=True, text=True)
        if result.returncode:
            raise RuntimeError('%s:\n%s' % (source, result.stderr))
        return obj

    with concurrent.futures.ThreadPoolExecutor() as executor:
        objects = list(executor.map(compile_source, enumerate(sources)))

    exe = os.path.join(directory, 'harness.exe')
    result = subprocess.run(['gcc'] + objects + ['-o', exe, '-lm'], capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError('link:\n' + result.stderr)
    return exe


# Run

def get_screenshot_names(script):
    """Returns the screenshot names of a script ("screenshot" alone: "screenshot")."""
    names = []
    for line in script.split('\n'):
        words = line.split()
        if words[:1] == ['screenshot']:
            names.append(words[1] if len(words) > 1 else 'screenshot')
    return names


def to_harness_script(script, workdir):
    """Resolves "clock HH:MM" and the screenshot paths for the harness."""
    lines = []
    for line in script.strip().split('\n'):
        words = line.split()
        if words[:1] == ['clock'] and ':' in words[1]:
            hour, minute = (int(value) for value in words[1].split(':'))
            line = 'clock %d' % calendar.timegm((2026, 6, 15, hour, minute, 0))
        elif words[:1] == ['screenshot']:
            name = words[1] if len(words) > 1 else 'screenshot'
            line = 'screenshot ' + os.path.join(workdir, name + '.ppm')
        lines.append(line)
    return '\n'.join(lines) + '\n'


def run_script(exe, directory, name, script):
    """Runs a script; returns its screenshots (name: image at 1x)."""
    workdir = os.path.join(directory, name)
    os.makedirs(workdir)

    # Each run starts with an empty flash (radpro-settings.bin in workdir)
    result = subprocess.run([exe], input=to_harness_script(script, workdir), cwd=workdir,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError('%s: %s' % (name, result.stderr.strip()))

    return {screenshot: Image.open(os.path.join(workdir, screenshot + '.ppm')).convert('RGB')
            for screenshot in get_screenshot_names(script)}


def upscale(image):
    return image.resize((image.width * SCALE, image.height * SCALE), Image.NEAREST)


def dim(value):
    """Dims a channel outside the highlight, like the earlier hand-made images."""
    return max(0, round(0.75 * value - 3.25))


def highlight_rectangle(image, rectangle):
    dimmed = image.point(dim)
    dimmed.paste(image.crop(rectangle), rectangle[:2])
    return dimmed


def add_ribbon(image):
    """Draws the "Try me!" ribbon (antialiased) over the bottom right corner."""
    width, height = image.size
    supersampling = 4

    band = Image.new('L', (width * supersampling, height * supersampling))
    corners = [(RIBBON_START - height, height), (width, RIBBON_START - width),
               (width, RIBBON_END - width), (RIBBON_END - height, height)]
    ImageDraw.Draw(band).polygon([(x * supersampling, y * supersampling) for x, y in corners],
                                 fill=255)
    band = band.resize((width, height), Image.LANCZOS)
    image.paste((0, 0, 0), (0, 0, width, height), band)

    # Font size: the text spans RIBBON_TEXT_LENGTH pixels
    size = 10
    while True:
        font = ImageFont.truetype(RIBBON_FONT, size + 1)
        left, top, right, bottom = font.getbbox(RIBBON_TEXT)
        if right - left > RIBBON_TEXT_LENGTH:
            break
        size += 1
    font = ImageFont.truetype(RIBBON_FONT, size)
    left, top, right, bottom = font.getbbox(RIBBON_TEXT)

    text = Image.new('L', (right - left + 8, bottom - top + 8))
    ImageDraw.Draw(text).text((4 - left, 4 - top), RIBBON_TEXT, font=font, fill=255)
    text = text.rotate(45, resample=Image.BICUBIC, expand=True)

    # Centre: RIBBON_TEXT_CENTRE along the band, in the middle across it
    across = (RIBBON_START + RIBBON_END) / 2
    centre_x = (across + RIBBON_TEXT_CENTRE) / 2
    centre_y = (across - RIBBON_TEXT_CENTRE) / 2
    position = (round(centre_x - text.width / 2), round(centre_y - text.height / 2))
    image.paste(Image.new('RGB', text.size, (255, 255, 255)), position, text)
    return image


def compose(tiles, layout, ribbon):
    rows = [[Image.open(tile).convert('RGB') if tile.endswith('.png') else upscale(tiles[tile])
             for tile in row] for row in layout]
    tile_width, tile_height = rows[0][0].size
    image = Image.new('RGB', (tile_width * len(rows[0]), tile_height * len(rows)))
    for y, row in enumerate(rows):
        for x, tile in enumerate(row):
            image.paste(tile, (x * tile_width, y * tile_height))
    return add_ribbon(image) if ribbon else image


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('names', nargs='*', help='images to generate (default: all)')
    parser.add_argument('--out', default=IMG, help='output directory (default: docs/img)')
    parser.add_argument('--list', action='store_true', help='list the images')
    args = parser.parse_args()

    if args.list:
        print('\n'.join(list(SCENARIOS) + list(COMPOSITES)))
        return

    names = args.names or (list(SCENARIOS) + list(COMPOSITES))
    unknown = [name for name in names if name not in SCENARIOS and name not in COMPOSITES]
    if unknown:
        sys.exit('unknown image: ' + ', '.join(unknown))

    os.makedirs(args.out, exist_ok=True)

    with tempfile.TemporaryDirectory() as directory:
        print('Building...')
        exe = build(directory)

        # Composites sharing a script run it once
        runs = {}

        def run(name, script):
            if script not in runs:
                runs[script] = run_script(exe, directory, name, script)
            return runs[script]

        for name in names:
            if name in SCENARIOS:
                script, highlight = SCENARIOS[name]
                image = upscale(run(name, script)['screenshot'])
                if highlight:
                    image = highlight_rectangle(image, tuple(value * SCALE for value in highlight))
            else:
                script, layout, ribbon = COMPOSITES[name]
                image = compose(run(name, script), layout, ribbon)

            path = os.path.join(args.out, name + '.png')
            image.save(path)
            print(path)


if __name__ == '__main__':
    main()
