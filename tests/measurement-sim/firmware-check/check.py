"""
Checks the firmware's instantaneous rate code against the Python reference.

Compiles platform.io/src/measurements/instantaneous.c into a host program
(harness.c), together with calculateRate and getDeadTimeCompensationFactor
(pulses.c) and getMeasurementAlertLevel and updateMeasurementAlertLevel
(measurements.c), which are copied verbatim from the sources. Then:

  1. rates   Feeds random pulse sequences (0.01 to 5000 cps, with gaps and
             rate changes, with and without dead-time compensation) to the
             firmware and to ../algorithms.py ProposedInstantaneous, and
             compares the reading and its confidence in all three averaging
             modes.
  2. alerts  Checks the firmware's alert level, every second, against the
             rule: raise when the reading reaches the threshold (adaptive:
             confidence narrower than +/-75%; 1 minute: after a full minute),
             clear below threshold / (1 + confidence). Also checks that an
             alert is armed (sounds, even after being dismissed) only when its
             level rises.

Usage: python check.py [--quick] [--verbose]
Needs gcc on the PATH (on Windows, for example MSYS2 UCRT64) and numpy.
"""

import argparse
import math
import os
import platform
import subprocess
import sys
import tempfile

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
FIRMWARE = os.path.normpath(os.path.join(HERE, '..', '..', '..', 'platform.io'))
SRC = os.path.join(FIRMWARE, 'src')

sys.path.insert(0, os.path.dirname(HERE))
from algorithms import ProposedInstantaneous  # noqa: E402

# Settings index -> reference parameters (change detection G > 22)
MODES = {
    0: ('fast', dict(pulse_target=20, change_threshold=22)),
    1: ('precision', dict(pulse_target=100, change_threshold=22)),
    2: ('1 minute', dict(fixed_window=60)),
}

# Same flags as the simulator build (CMakeLists.txt)
CFLAGS = ['-std=c2x', '-O1', '-DSIMULATOR', '-DSIMULATE_PULSES', '-DGAME',
          '-DDISPLAY_COLOR', '-DDISPLAY_320X240', '-DKEYBOARD_5_KEYS', '-DBUZZER',
          '-DFLASH_PAGE_SIZE=0x400', '-DFIRMWARE_BASE=0', '-DFIRMWARE_SIZE=0',
          '-DSTRINGS="strings/en.h"']
INCLUDES = ['src', os.path.join('src', 'system'), os.path.join('lib', 'mcu-max'),
            os.path.join('lib', 'mcu-renderer')]


# Build

def extract(path, signature):
    """Returns a function's source, from its signature to its closing brace."""
    source = open(os.path.join(SRC, path), encoding='utf-8').read()
    start = source.index(signature)
    i = source.index('{', start)
    depth = 0
    while True:
        depth += {'{': 1, '}': -1}.get(source[i], 0)
        i += 1
        if depth == 0:
            return source[start:i]


def build(directory):
    extracted = os.path.join(directory, 'extracted.c')
    with open(extracted, 'w', encoding='utf-8') as f:
        f.write('\n\n'.join([
            '#include "measurements/measurements.h"\n'
            '#include "measurements/pulses.h"\n'
            '#include "peripherals/tube.h"\n'
            '#include "system/cmath.h"',
            extract('measurements/pulses.c', 'void calculateRate('),
            extract('measurements/pulses.c', 'float getDeadTimeCompensationFactor('),
            extract('measurements/measurements.c', 'AlertLevel getMeasurementAlertLevel('),
            extract('measurements/measurements.c', 'void updateMeasurementAlertLevel('),
        ]) + '\n')

    executable = os.path.join(directory, 'harness.exe' if os.name == 'nt' else 'harness')
    command = (['gcc'] + CFLAGS + ['-I' + os.path.join(FIRMWARE, d) for d in INCLUDES] +
               [os.path.join(HERE, 'harness.c'), extracted,
                os.path.join(SRC, 'system', 'cmath.c'), '-o', executable])
    if platform.system() != 'Windows':
        command.append('-lm')
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        sys.exit('Build failed:\n' + result.stdout + result.stderr)
    return executable


def run_harness(executable, mode, counts, threshold=1.0, dead_time=0.0):
    out = subprocess.run([executable, str(mode), repr(threshold), repr(dead_time)],
                         input=' '.join(map(str, counts)),
                         capture_output=True, text=True, check=True).stdout
    return np.array([line.split() for line in out.splitlines()], dtype=float)


# 1. Rates

def random_counts(rng, seconds):
    counts = []
    while len(counts) < seconds:
        length = int(rng.integers(1, 600))
        rate = 0.0 if rng.random() < 0.15 else 10 ** rng.uniform(-2, math.log10(5000))
        counts.extend(rng.poisson(rate, length).tolist())
    return counts[:seconds]


def check_rates(executable, trials):
    rng = np.random.default_rng(12345)
    worst = {mode: 0.0 for mode in MODES}
    seconds = 0
    restarts = 0
    for trial in range(trials):
        counts = random_counts(rng, int(rng.integers(200, 6000)))
        dead_time = 150E-6 if trial % 2 else 0.0
        seconds += len(counts)
        for mode, (name, params) in MODES.items():
            firmware = run_harness(executable, mode, counts, dead_time=dead_time)
            reference = ProposedInstantaneous(estimator='n-1', tau=dead_time, **params)
            for t, count in enumerate(counts):
                rate, confidence = reference.update(count)
                fw_rate, fw_confidence = firmware[t, 0], firmware[t, 1]
                if rate == 0:
                    error = 0.0 if (fw_rate == 0 and fw_confidence == 0) else math.inf
                else:
                    error = max(abs(fw_rate / rate - 1), abs(fw_confidence / confidence - 1))
                if not error < 1e-6:
                    print(f'FAIL rates: trial {trial}, {name}, second {t + 1}: firmware '
                          f'{fw_rate:.6g} ±{fw_confidence:.4g}, reference {rate:.6g} '
                          f'±{confidence:.4g}; counts {counts[max(t - 5, 0):t + 1]}')
                    return False
                worst[mode] = max(worst[mode], error)
            restarts += reference.restarts
    print(f'rates:  OK, {seconds} s x {len(MODES)} modes, {restarts} window restarts, '
          f'largest relative difference {max(worst.values()):.1e} (float rounding)')
    return True


# 2. Alerts

f32 = np.float32


def alert_level(value):
    return np.where(value >= f32(3), 2, np.where(value >= f32(1), 1, 0))


def expected_alert_levels(mode, threshold, rate, confidence):
    reading = rate.astype(f32) * f32(1.0 / threshold)
    if mode == 2:
        gate = np.arange(len(rate)) >= 59
    else:
        gate = (confidence > 0) & (confidence < 0.75)
    raise_level = np.where(gate, alert_level(reading), 0)
    hold_level = alert_level(reading * (f32(1) + confidence.astype(f32)))
    levels = np.zeros_like(raise_level)
    current = 0
    for t in range(len(rate)):
        current = max(raise_level[t], min(current, hold_level[t]))
        levels[t] = current
    return levels


def check_alerts(executable, hours, verbose):
    rng = np.random.default_rng(7)
    mismatches = 0
    seconds = 0
    if verbose:
        print('Dismissed alerts sounding again per hour (old rule: alert exactly while '
              'the reading >= threshold):')
        print(f'{"threshold":>9} {"mode":>10} {"rate":>6} {"old rule":>9} '
              f'{"hysteresis":>11} {"in alert":>9}')
    for threshold in (0.3, 3.0, 30.0):
        for mode, (name, _) in MODES.items():
            for ratio in (0.8, 1.0, 1.2, 2.7, 3.0):
                counts = rng.poisson(ratio * threshold, int(hours * 3600)).tolist()
                out = run_harness(executable, mode, counts, threshold=threshold)
                rate, confidence = out[:, 0], out[:, 1]
                levels, armed = out[:, 2].astype(int), out[:, 3].astype(int)
                seconds += len(counts)

                expected = expected_alert_levels(mode, threshold, rate, confidence)
                mismatches += int(np.sum(levels != expected))
                rises = levels > np.concatenate([[0], levels[:-1]])
                if not np.array_equal(armed.astype(bool), rises):
                    print(f'FAIL alerts: {name}, threshold {threshold}: alert armed '
                          'without a level rise, or not armed on a rise')
                    return False

                if verbose:
                    reading = rate.astype(f32) * f32(1.0 / threshold)
                    gate = (np.arange(len(rate)) >= 59) if mode == 2 else \
                        (confidence > 0) & (confidence < 0.75)
                    old = np.where(gate, alert_level(reading), 0)
                    old_rises = np.sum(old > np.concatenate([[0], old[:-1]]))
                    print(f'{threshold:9g} {name:>10} {ratio:5g}x {old_rises / hours:9.1f} '
                          f'{np.sum(rises) / hours:11.1f} {100 * np.mean(levels > 0):8.0f}%')
    if mismatches:
        print(f'FAIL alerts: {mismatches} seconds where the firmware alert level differs '
              'from the rule')
        return False
    print(f'alerts: OK, {seconds} s, alert level and arming follow the rule')
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--quick', action='store_true', help='fewer and shorter runs')
    parser.add_argument('--verbose', action='store_true', help='print the alert re-arm table')
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as directory:
        executable = build(directory)
        ok = check_rates(executable, 8 if args.quick else 40)
        ok = check_alerts(executable, 0.5 if args.quick else 3, args.verbose) and ok
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
