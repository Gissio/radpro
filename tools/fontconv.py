# MCU renderer
# fontconv
# (C) 2023-2025 Gissio

from pprint import pprint

import argparse
import os

from bdflib import reader
import freetype

DEFAULT_PIXELS = 16
DEFAULT_BITS_PER_PIXEL = 4


class MinMax:
    def __init__(self):
        self.min = None
        self.max = None

    def update(self, value):
        if self.min is None or value < self.min:
            self.min = value
        if self.max is None or value > self.max:
            self.max = value


class Glyph:
    def __init__(self):
        self.left = 0
        self.bottom = 0
        self.width = 0
        self.height = 0

        self.advance = 0

        self.bitmap = []


class Font:
    def __init__(self):
        self.name = ''
        self.copyright = ''
        self.codepoint_set = ''
        self.variable_name = ''

        self.size = None
        self.cap_height = None
        self.ascent = None
        self.descent = None

        self.boundingbox_left = MinMax()
        self.boundingbox_bottom = MinMax()
        self.boundingbox_right = MinMax()
        self.boundingbox_top = MinMax()

        self.glyphs_width = MinMax()
        self.glyphs_height = MinMax()

        self.advance = MinMax()

        self.glyphs = {}

    def add_glyph(self, codepoint, glyph):
        self.boundingbox_left.update(glyph.left)
        self.boundingbox_bottom.update(glyph.bottom)
        self.boundingbox_right.update(glyph.left + glyph.width)
        self.boundingbox_top.update(glyph.bottom + glyph.height)

        self.glyphs_width.update(glyph.width)
        self.glyphs_height.update(glyph.height)

        self.advance.update(glyph.advance)

        self.glyphs[codepoint] = glyph


def parse_codepoint_set(codepoint_set):
    codepoints = []

    if codepoint_set:
        codepoint_ranges = codepoint_set.split(',')

        for codepoint_range in codepoint_ranges:
            codepoint_range_items = codepoint_range.split('-', 2)
            if len(codepoint_range_items) == 1:
                value = int(codepoint_range_items[0], 0)

                if 0 <= value:
                    codepoints.append(value)
                else:
                    raise RuntimeError('invalid codepoint set: negative value')
            elif len(codepoint_range_items) == 2:
                start = int(codepoint_range_items[0], 0)
                end = int(codepoint_range_items[1], 0)

                if 0 <= start <= end:
                    codepoints.extend(list(range(start, end + 1)))
                else:
                    raise RuntimeError(
                        f'invalid codepoint set: {codepoint_range_items[0]} > {codepoint_range_items[1]}')

    return codepoints


def build_codepoint_set(codepoints):
    if not codepoints:
        return ''

    def build_codepoint_range(start, end):
        if start == end:
            return f'{hex(start)}'
        else:
            return f'{hex(start)}-{hex(end)}'

    codepoint_list = list(codepoints)
    codepoint_list.sort()

    codepoint_ranges = []
    start = codepoint_list[0]
    prev = codepoint_list[0]

    for codepoint in codepoint_list[1:]:
        if codepoint != prev + 1:
            codepoint_ranges.append(build_codepoint_range(start, prev))
            start = codepoint
        prev = codepoint

    codepoint_ranges.append(build_codepoint_range(start, prev))

    return codepoint_ranges


def get_bdf_property(bdf_font, key, default_value):
    key_encoded = key.encode('utf-8')

    if key_encoded in bdf_font.properties:
        value = bdf_font.properties[key_encoded]
        if isinstance(value, bytes):
            value = value.decode('utf-8')
    else:
        value = default_value

    return value


def load_bitmap_font(path, codepoint_set, font_variable_name):
    try:
        codepoints = parse_codepoint_set(codepoint_set)

        # Load
        font_file = open(path, 'rb')

        bdf_font = reader.read_bdf(font_file)

        font = Font()

        # Font properties
        font.name = bdf_font.name.decode('utf-8')
        font.name = get_bdf_property(bdf_font, 'FAMILY_NAME', font.name)
        weight_name = get_bdf_property(bdf_font, 'WEIGHT_NAME', '')
        if weight_name:
            font.name += ' ' + weight_name
        font.copyright = get_bdf_property(
            bdf_font, 'COPYRIGHT', bdf_font.comments)
        font.variable_name = font_variable_name

        # Glyphs
        for bdf_glyph in bdf_font.glyphs:
            if len(codepoints) > 0 and bdf_glyph.codepoint not in codepoints:
                continue

            glyph = Glyph()

            glyph.left = bdf_glyph.bbX
            glyph.bottom = bdf_glyph.bbY
            glyph.width = bdf_glyph.bbW
            glyph.height = bdf_glyph.bbH
            glyph.advance = bdf_glyph.advance

            glyph.bitmap = [0] * (glyph.width * glyph.height)
            for y in range(glyph.height):
                for x in range(glyph.width):
                    data_x = glyph.width - 1 - x
                    data_y = glyph.height - 1 - y
                    glyph.bitmap[y * glyph.width +
                                 x] = ((bdf_glyph.data[data_y] >> data_x) & 1) * 255

            font.add_glyph(bdf_glyph.codepoint, glyph)

        # Size, ascent, descent, cap height
        font.size = get_bdf_property(
            bdf_font, 'POINT_SIZE', bdf_font.ptSize)
        font.ascent = get_bdf_property(
            bdf_font, 'FONT_ASCENT', font.boundingbox_top.max)
        font.descent = get_bdf_property(
            bdf_font, 'FONT_DESCENT', font.boundingbox_bottom.min)
        font.cap_height = get_bdf_property(
            bdf_font, 'CAP_HEIGHT', bdf_font.ptSize)
        font.pixel_bitnum = 1

        return font

    except Exception as e:
        print(f'error: {e}')

        return None


def load_vector_font(path, codepoint_set, font_variable_name, pixels, pixel_bitnum):
    try:
        codepoints = parse_codepoint_set(codepoint_set)

        # Load
        face = freetype.Face(path)

        if face.is_scalable:
            face.set_char_size(int(64 * pixels))
        else:
            pixels = 0

        font = Font()

        # Font properties
        font_family = ''
        font_subfamily = ''
        for i in range(face.sfnt_name_count):
            name = face.get_sfnt_name(i)
            value = ''
            try:
                if name.platform_id == 1:  # Macintosh
                    value = name.string.decode('utf-8')
                elif name.platform_id == 3:  # Windows
                    value = name.string.decode('utf-16-be')
                else:
                    value = name.string.decode(
                        'utf-8', errors='ignore')
            except UnicodeDecodeError:
                pass
            if name.name_id == 0:  # Copyright
                font.copyright = value
            elif name.name_id == 1:  # Family
                font_family = value
            elif name.name_id == 2:  # Subfamily
                font_subfamily = value
        if font_family:
            font.name = f'{font_family} {font_subfamily}'.strip()
        else:
            basename = os.path.basename(path)
            font.name = os.path.splitext(basename)[0]
        if pixels:
            font.name += f' {int(pixels)}'
        font.variable_name = font_variable_name

        # Glyphs
        for codepoint, glyph_index in face.get_chars():
            if len(codepoints) > 0 and codepoint not in codepoints:
                continue

            face.load_glyph(glyph_index, freetype.FT_LOAD_RENDER)

            glyph = Glyph()

            glyph.left = face.glyph.bitmap_left
            glyph.bottom = face.glyph.bitmap_top - face.glyph.bitmap.rows
            glyph.width = face.glyph.bitmap.width
            glyph.height = face.glyph.bitmap.rows
            glyph.advance = face.glyph.advance.x // 64

            glyph.bitmap = [0] * (glyph.width * glyph.height)
            for y in range(glyph.height):
                for x in range(glyph.width):
                    glyph.bitmap[y * glyph.width + x] = face.glyph.bitmap.buffer[
                        y * face.glyph.bitmap.pitch + x]

            font.add_glyph(codepoint, glyph)

        # Size, ascent, descent, cap height
        font.size = int(pixels)
        font.cap_height = (face.size.ascender + face.size.descender) // 64
        font.ascent = face.size.ascender // 64
        font.descent = -face.size.descender // 64
        font.pixel_bitnum = pixel_bitnum

        return font

    except Exception as e:
        print(f'error: {e}')

        return None


class Bitstream:
    def __init__(self):
        self.data = bytearray()
        self.bit_index = 0

    def __ne__(self, other):
        return self.bit_index != other.bit_index or self.data != other.data

    def clear(self):
        self.data.clear()
        self.bit_index = 0

    def write_bit(self, value):
        if self.bit_index == 0:
            self.data.append(0)
        if value:
            self.data[-1] |= (1 << self.bit_index)

        self.bit_index = (self.bit_index + 1) % 8

    def write_bits(self, value, bit_num):
        for _ in range(bit_num):
            self.write_bit(value & 1)
            value >>= 1

    def write_unary(self, value):
        for _ in range(value):
            self.write_bit(1)

        self.write_bit(0)

    def write_rice(self, value, fixed_bitnum):
        remainder = value & ((1 << fixed_bitnum) - 1)
        quotient = value >> fixed_bitnum

        self.write_bits(remainder, fixed_bitnum)
        self.write_unary(quotient)

    def write_variable(self, value):
        self.bit_index = 0

        chunks = []
        while True:
            chunk = value & 0x7f
            value >>= 7

            chunks.append(chunk)

            if value == 0:
                break

        for i, byte in enumerate(reversed(chunks)):
            if i < len(chunks) - 1:
                byte |= 0x80
            self.data.append(byte)

    def write_uint16(self, value):
        self.bit_index = 0

        self.data.append((value >> 8) & 0xff)
        self.data.append(value & 0xff)

    def append(self, other):
        self.data.extend(other.data)
        self.bit_index = 0

    def append_bits(self, other):
        for i in range(other.get_bitnum()):
            self.write_bit(other.read_bit(i))

    def get_bitnum(self):
        if self.bit_index == 0:
            return 8 * len(self.data)
        else:
            return 8 * (len(self.data) - 1) + self.bit_index

    def read_bit(self, bit_index):
        return (self.data[bit_index // 8] & (1 << (bit_index % 8))) != 0


class EncoderSettings:
    def __init__(self):
        self.boundingbox_left_bitnum = 0
        self.boundingbox_bottom_bitnum = 0
        self.boundingbox_width_bitnum = 0
        self.boundingbox_height_bitnum = 0

        self.advance_bitnum = 0
        self.pixel_bitnum = 0

        self.run_length_black_bitnum = 0
        self.run_length_white_bitnum = 0


def get_bitnum(value):
    value = abs(value) - 1 if value < 0 else value
    bit_num = 0

    while value > 0:
        bit_num += 1
        value >>= 1

    return bit_num


def get_bitnum_minmax(min_max):
    bit_num_a = get_bitnum(min_max.min)
    bit_num_b = get_bitnum(min_max.max)

    return max(bit_num_a, bit_num_b)


def encode_glyph(glyph, settings, encoded_block):
    bitshift = 8 - settings.pixel_bitnum
    white_value = (1 << settings.pixel_bitnum) - 1

    symbols = []

    # Pixels
    bitmap_index = 0
    while bitmap_index < len(glyph.bitmap):
        value = glyph.bitmap[bitmap_index] >> bitshift
        bitmap_index += 1

        symbol = Bitstream()

        # Encode pixel value
        symbol.write_bits(value, settings.pixel_bitnum)
        if value == 0 or value == white_value:
            # RLE black/white pixels
            run_length = 1
            while bitmap_index < len(glyph.bitmap):
                new_value = glyph.bitmap[bitmap_index] >> bitshift
                if value != new_value:
                    break

                bitmap_index += 1
                run_length += 1

            if value == 0:
                symbol.write_rice(
                    run_length, settings.run_length_black_bitnum)

            else:
                symbol.write_rice(
                    run_length - 1, settings.run_length_white_bitnum)

        symbols.append(symbol)

    # Encode symbol repeats
    symbol_index = 0
    while symbol_index < len(symbols):
        best_repeat_length = 0
        best_repeat_num = 1
        best_repeat_bitnum = 0

        for repeat_length in range(2, glyph.width):
            source_index = symbol_index
            dest_index = symbol_index + repeat_length
            repeat_num = 1
            repeat_bitnum = 0
            bit_num = 0

            while dest_index < len(symbols):
                if symbols[source_index] != symbols[dest_index]:
                    break
                bit_num += symbols[source_index].get_bitnum()
                source_index += 1
                dest_index += 1
                if source_index == symbol_index + repeat_length:
                    source_index = symbol_index
                    repeat_num += 1
                    repeat_bitnum = bit_num

            if repeat_bitnum > best_repeat_bitnum:
                best_repeat_length = repeat_length
                best_repeat_num = repeat_num
                best_repeat_bitnum = repeat_bitnum

        if best_repeat_num > 1:
            encoded_repeat_length = best_repeat_length - 2
            encoded_repeat_num = best_repeat_num - 2

            symbol = Bitstream()

            symbol.write_bits(0, settings.pixel_bitnum)
            symbol.write_rice(0, settings.run_length_black_bitnum)
            symbol.write_unary(encoded_repeat_length)
            symbol.write_unary(encoded_repeat_num)

            compressed_bitnum = symbol.get_bitnum()

            if compressed_bitnum < best_repeat_bitnum:
                del symbols[symbol_index + best_repeat_length:symbol_index +
                            best_repeat_length * best_repeat_num]
                symbols.insert(symbol_index, symbol)
                symbol_index += best_repeat_length

        symbol_index += 1

    encoded_glyph = Bitstream()

    encoded_glyph.write_bits(
        glyph.left, settings.boundingbox_left_bitnum)
    encoded_glyph.write_bits(
        glyph.bottom, settings.boundingbox_bottom_bitnum)
    encoded_glyph.write_bits(
        glyph.width, settings.boundingbox_width_bitnum)
    encoded_glyph.write_bits(
        glyph.height, settings.boundingbox_height_bitnum)
    encoded_glyph.write_bits(
        glyph.advance, settings.advance_bitnum)

    for symbol in symbols:
        encoded_glyph.append_bits(symbol)

    encoded_block.write_variable(
        len(encoded_glyph.data))
    encoded_block.append(encoded_glyph)


def encode_font(font):
    settings = EncoderSettings()

    settings.boundingbox_left_bitnum = 1 + \
        get_bitnum_minmax(font.boundingbox_left)
    settings.boundingbox_bottom_bitnum = 1 + \
        get_bitnum_minmax(font.boundingbox_bottom)
    settings.boundingbox_width_bitnum = get_bitnum_minmax(
        font.glyphs_width)
    settings.boundingbox_height_bitnum = get_bitnum_minmax(
        font.glyphs_height)
    settings.advance_bitnum = get_bitnum_minmax(
        font.advance)
    settings.pixel_bitnum = font.pixel_bitnum

    best_encoded_font = None

    # matrix = [None, None, None, None, None, None, None, None]
    # for i in range(8):
    #     matrix[i] = [0, 0, 0, 0, 0, 0, 0, 0]

    for run_length_black_bitnum in range(8):
        for run_length_white_bitnum in range(8):
            settings.run_length_black_bitnum = run_length_black_bitnum
            settings.run_length_white_bitnum = run_length_white_bitnum

            encoded_font = Bitstream()

            encoded_font.write_uint16(
                font.cap_height)
            encoded_font.write_uint16(
                font.ascent)
            encoded_font.write_uint16(
                font.descent)
            encoded_font.write_uint16(
                font.boundingbox_left.min)
            encoded_font.write_uint16(
                font.boundingbox_bottom.min)
            encoded_font.write_uint16(
                font.boundingbox_right.max - font.boundingbox_left.min)
            encoded_font.write_uint16(
                font.boundingbox_top.max - font.boundingbox_bottom.min)
            encoded_font.write_variable(
                settings.boundingbox_left_bitnum)
            encoded_font.write_variable(
                settings.boundingbox_bottom_bitnum)
            encoded_font.write_variable(
                settings.boundingbox_width_bitnum)
            encoded_font.write_variable(
                settings.boundingbox_height_bitnum)
            encoded_font.write_variable(
                settings.advance_bitnum)
            encoded_font.write_variable(
                settings.pixel_bitnum)
            encoded_font.write_variable(
                settings.run_length_black_bitnum)
            encoded_font.write_variable(
                settings.run_length_white_bitnum)

            encoded_block = Bitstream()
            block_codepoint = None
            next_codepoint = None
            for codepoint, glyph in sorted(font.glyphs.items()):
                # Latin and cyrillic uppercase and lowercase indices
                if codepoint != next_codepoint or codepoint in (0x41, 0x61, 0x410, 0x430):
                    if encoded_block.get_bitnum():
                        encoded_font.write_variable(
                            len(encoded_block.data))
                        encoded_font.write_variable(
                            block_codepoint)
                        encoded_font.append(
                            encoded_block)

                        encoded_block.clear()

                    block_codepoint = codepoint

                next_codepoint = codepoint + 1
                encode_glyph(glyph, settings, encoded_block)

            encoded_font.write_variable(
                len(encoded_block.data))
            encoded_font.write_variable(
                block_codepoint)
            encoded_font.append(
                encoded_block)

            encoded_font.write_variable(0)

            if best_encoded_font is None or len(encoded_font.data) < len(best_encoded_font.data):
                best_encoded_font = encoded_font

    #         matrix[run_length_black_bitnum][run_length_white_bitnum] = len(
    #             encoded_font.data)

    # print(font.name)
    # pprint(matrix)

    return best_encoded_font


def write_encoded_font(font, encoded_font, path):
    codepoint_set = ','.join(build_codepoint_set(font.glyphs.keys()))

    define_name = font.variable_name.upper()

    with open(path, 'w') as file:
        file.write(f'/**\n')
        file.write(f' * Font: {font.name}\n')
        file.write(f' * Copyright: {font.copyright}\n')
        file.write(f' * Codepoints: {codepoint_set}\n')
        file.write(f' */\n\n')

        file.write(f'#include <stdint.h>\n\n')

        file.write(f'#define {define_name}_SIZE {font.size}\n')
        file.write(f'#define {define_name}_ASCENT {font.ascent}\n')
        file.write(f'#define {define_name}_DESCENT {font.descent}\n')
        file.write(f'#define {define_name}_CAP_HEIGHT {font.cap_height}\n')
        file.write(
            f'#define {define_name}_LINE_HEIGHT {font.ascent + font.descent}\n')
        file.write(
            f'#define {define_name}_BOUNDINGBOX_LEFT {font.boundingbox_left.min}\n')
        file.write(
            f'#define {define_name}_BOUNDINGBOX_BOTTOM {font.boundingbox_bottom.min}\n')
        file.write(
            f'#define {define_name}_BOUNDINGBOX_WIDTH {font.boundingbox_right.max - font.boundingbox_left.min}\n')
        file.write(
            f'#define {define_name}_BOUNDINGBOX_HEIGHT {font.boundingbox_top.max - font.boundingbox_bottom.min}\n\n')

        file.write(
            f'const uint8_t {font.variable_name}[{len(encoded_font.data)}] = {{\n')
        for i in range(0, len(encoded_font.data), 16):
            row = encoded_font.data[i:i+16]
            file.write(
                '    ' + ', '.join(f'0x{byte:02x}' for byte in row) + ',\n')
        file.write(f'}};\n')


def main():
    # Parse arguments
    parser = argparse.ArgumentParser(
        description='Converts a .ttf/.otf/.bdf file to the mcu-renderer format.')
    parser.add_argument('-s', '--codepoint-set',
                        dest='codepoint_set',
                        default='',
                        help='set Unicode codepoints to convert (e.g. "32-255" or "0x2e,0x30-0x39")')
    parser.add_argument('-p', '--pixels',
                        dest='pixels',
                        type=float,
                        default=DEFAULT_PIXELS,
                        help=f'set number of pixels per em-square for font rasterization (supports fractional values)')
    parser.add_argument('-b', '--bits-per-pixel',
                        dest='bits_per_pixel',
                        type=int,
                        default=DEFAULT_BITS_PER_PIXEL,
                        choices=range(1, 9),
                        help=f'set bits per pixel (default: {DEFAULT_BITS_PER_PIXEL})')
    parser.add_argument('-a', '--ascent',
                        dest='ascent',
                        type=int,
                        help='override font ascent (baseline to top of line)')
    parser.add_argument('-d', '--descent',
                        dest='descent',
                        type=int,
                        help='override font descent (bottom of line to baseline)')
    parser.add_argument('-c', '--cap-height',
                        dest='cap_height',
                        type=int,
                        help='override font cap height')
    parser.add_argument('-n', '--variable-name',
                        dest='variable_name',
                        help='override C-language font variable name')
    parser.add_argument('input_filename')
    parser.add_argument('output_filename')
    args = parser.parse_args()

    if args.pixels <= 0:
        print('error: pixels must be positive')
        exit(1)

    if args.variable_name is not None:
        font_variable_name = args.variable_name
    else:
        filename = os.path.basename(args.output_filename)
        basename = os.path.splitext(filename)[0]
        font_variable_name = ''.join(
            c if c.isalnum() else '_' for c in basename)

    # Read font
    print(f'Building {args.output_filename}...')

    if args.input_filename.lower().endswith('.bdf'):
        font = load_bitmap_font(args.input_filename,
                                args.codepoint_set,
                                font_variable_name)
    else:
        font = load_vector_font(args.input_filename,
                                args.codepoint_set,
                                font_variable_name,
                                args.pixels,
                                args.bits_per_pixel)

    if font is None:
        exit(1)

    if args.ascent is not None:
        font.ascent = args.ascent
    if args.descent is not None:
        font.descent = args.descent
    if args.cap_height is not None:
        font.cap_height = args.cap_height

    # Verify missing characters
    requested_codepoints = parse_codepoint_set(args.codepoint_set)
    if len(requested_codepoints):
        available_codepoints = font.glyphs.keys()
        missing_codepoints = list(
            set(requested_codepoints) - set(available_codepoints))
        if len(missing_codepoints):
            missing_codepoint_set = ','.join(
                build_codepoint_set(missing_codepoints))
            print('warning: requested codepoints not available: ' +
                  missing_codepoint_set)

    # Encode
    encoded_font = encode_font(font)

    # Write
    write_encoded_font(font, encoded_font, args.output_filename)


if __name__ == '__main__':
    main()
