# Measurement firmware check

Checks that the firmware's instantaneous rate code does exactly what the Python reference ([../algorithms.py](../algorithms.py)) does. Run it after any change to the measurement code.

```sh
python check.py            # about 5 s
python check.py --quick    # about 2 s
python check.py --verbose  # also prints how often dismissed alerts sound again
```

Requires `gcc` and `numpy`. On Windows, run it from PowerShell or the MSYS2 UCRT64 shell, with MSYS2's `gcc` on the PATH.

## What it does

`check.py` compiles the firmware's `platform.io/src/measurements/instantaneous.c` into a host program ([harness.c](harness.c)), together with these functions, copied verbatim from the sources each time:

- `calculateRate` and `getDeadTimeCompensationFactor` from `pulses.c`
- `getMeasurementAlertLevel` and `updateMeasurementAlertLevel` from `measurements.c`

Then it runs two checks:

1. **Rates:** random pulse sequences (0.01 to 5000 cps, with gaps and rate changes, half of them with 150 µs dead-time compensation) go to the firmware and to `ProposedInstantaneous` in all three averaging modes. The reading and its confidence must match every second, apart from float rounding. On a mismatch it prints the second and the pulse counts leading up to it.
2. **Alerts:** the firmware's alert level must follow the rule every second:
   - **Raise** when the reading reaches the threshold. In the adaptive modes, only with a confidence interval narrower than ±75%; in 1-minute mode, only once a full minute has been measured.
   - **Clear** when the reading falls below threshold / (1 + confidence).
   - **Arm** (sound, even after the alert was dismissed) only when the level rises.

   The warning is at the threshold and the alarm at 3x the threshold.

If you change the algorithm on purpose, change `../algorithms.py` the same way. For alert rule changes, also change `expected_alert_levels()` in `check.py`.
