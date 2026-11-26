# Instantaneous rate simulation

Monte Carlo comparison of the current instantaneous rate algorithm with the proposed redesign, used to choose the redesign's parameters before porting it to the firmware.

## Files

- `algorithms.py`: `CurrentInstantaneous` is a literal port of the firmware (`pulses.c` `calculateRate`, `instantaneous.c` `updateInstantaneousRate`). `ProposedInstantaneous` is the redesign, written to be ported to C line by line.
- `simulate.py`: runs the scenarios in parallel and writes `results/report.md` plus plots.

## Running

```sh
python simulate.py          # full run, under a minute on a multi-core PC
python simulate.py --quick  # smoke test
```

Requires `numpy` and `matplotlib`.

## Proposed algorithm

- **Buffer:** cumulative pulse counts, 64 entries at 1 s plus 64 entries at 16 s (about 17 min). A window's count is one subtraction. Windows longer than 64 s start on a 16 s boundary.
- **Adaptive modes:** the smallest window holding at least N pulses (fast N = 20, precision N = 100), estimated as (n - 1) / W. If the buffer or the last restart doesn't allow N pulses, the longest available window is used, estimated as n / W.
- **Fixed mode:** n / W over 60 s, with no change detection.
- **Change detection:** every second, the last k = 1, 2, 4 ... 512 s are compared with the older part of the window using a likelihood-ratio (G) test for two Poisson counts. If G > 22, the window restarts at the k with the largest G.
- **Confidence:** `getConfidenceInterval` of the raw count.

## Scenarios

| Stage | What it checks |
|---|---|
| 1. Calibrate | G threshold for at most 0.1 false restarts per hour, at 0.07 to 1000 cps |
| 2. Steady | Bias, noise, 95% coverage and window length; choice of estimator |
| 3. Steps | x2, x10 and x100, up and down, for HH614 (0.07 cps), SBM-20 (0.25 cps) and pancake (1 cps) background rates |
| 4. Pass-by | Walking past a point source: peak x10 and x100, FWHM 10 s and 60 s |
| 5. Dead time | Non-paralyzable 150 us dead time as a renewal process: compensated rate, coverage and dose |
| 6. Alerts | Alert starts per hour, time in alert and delay, for thresholds of 0.3, 3 and 30 cps and true rates of 0.5x to 3x, under several alert policies |

## Results summary (2026-11-26)

- **Change detection:** G > 22 gives 0.01 to 0.06 false restarts per hour at every rate.
- **Estimator:** (n - 1) / W has at most 1.2% bias; n / W has up to +5.6%. Coverage is 95-96%, so `getConfidenceInterval` gives honest 95% intervals.
- **Fixed 60 s mode:** the current firmware reads +53% high with about ±220% standard deviation at 0.07 cps, because its window ends at the last second with pulses. The proposed n / W is unbiased.
- **Large changes (x10, x100):** the proposed design settles much faster. For example, a x100 drop takes 5 s instead of 307 s on an HH614-class tube, and 1 s instead of 96 s on an SBM-20-class tube.
- **Small changes (x2):** proposed fast behaves like current fast. Proposed precision is slower by design, for example about 12 min for a x2 drop on an HH614-class tube.
- **Time cap:** a 60 s or 5 min cap changes the results only for low-rate tubes and small changes. There it trades precision for speed, which is the same as a smaller N. Large changes are handled by change detection with or without a cap.
- **Pass-by:** proposed fast captures more of short peaks and overshoots less on the retreat than current fast.
- **Alerts:** without hysteresis, alerts chatter near the threshold with both the old and the new estimator. With a true rate at the threshold, the old firmware starts about 46, 290 and 900 alerts per hour at 0.3, 3 and 30 cps in fast mode. The new estimator doubles that only at the lowest rate. The strict policy (raise when the lower 95% bound reaches the threshold, clear when the upper bound drops below it) cuts this to 1-19 per hour. False alerts at 0.8x the threshold fall from 26-382 per hour to 0.4 or fewer. At 2x the threshold, delays stay within a few seconds of the current firmware, but exceedances of only 10-50% take longer to confirm.
- **Sawtooth vs. hold** (`simulate.py --hold`, results/hold.md): ending adaptive windows at the most recent second with pulses, instead of now, holds the reading steady between pulses. With the correction (n - k) / W, k = 1 + W / (W + n), it stays unbiased (within ±2%). It changes 4-14 times a minute at low rates instead of every second. Thanks to change detection, settle times, pass-by response and alerts are the same as the sawtooth's. Not adopted: on real use the sawtooth turned out to be barely noticeable.
- **Dead time:** confidence computed from raw counts keeps 95% coverage after compensation, so it needs no widening, because dead time makes the counts sub-Poisson. Compensating each second from its own count gets the dose right. The current firmware under-reads a 60 s exposure at 5000 cps by 23% in 60 s mode.
