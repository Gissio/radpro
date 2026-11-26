"""
Rad Pro instantaneous rate algorithms.

CurrentInstantaneous is a literal port of the Rad Pro 3.1 firmware (pulses.c
calculateRate + instantaneous.c updateInstantaneousRate).

ProposedInstantaneous is the 3.2 redesign: a per-second count buffer
(cumulative counts, two tiers), adaptive windows with a pulse target, and
change detection. It is the reference for instantaneous.c (pulse_target 20 or
100 with change_threshold 22, or fixed_window 60). max_window and hold are
variants that were simulated but not adopted.
"""

import bisect
import math
from collections import deque

SYSTICK_FREQUENCY = 1000

# Buffer layout of the proposed design
TIER1_LENGTH = 64       # Cumulative counts at 1 s resolution
TIER2_BLOCK = 16        # Seconds per tier 2 entry
TIER2_LENGTH = 64       # Cumulative counts at 16 s boundaries (~17 min)

# Change detection: compare the last k seconds against the older part of
# the window, for these k (values above TIER1_LENGTH snap to tier 2)
CHANGE_LENGTHS = (1, 2, 4, 8, 16, 32, 64, 128, 256, 512)


def confidence_interval(n):
    """Port of cmath.c getConfidenceInterval: relative 95% half-width."""
    if n <= 1:
        return 2.6888794
    if n == 2:
        return 1.7858216
    return 1.959964 / math.sqrt(n) + 0.92095147 / (0.34074598 + n)


def dead_time_factor(rate, tau):
    """Non-paralyzable dead-time compensation factor, clamped like the firmware."""
    return 1.0 / max(1.0 - rate * tau, 0.1)


class CurrentInstantaneous:
    """Firmware algorithm. Modes: fast, precision, 60s, 30s, 10s."""

    MIN_TIMES = {'fast': 0, 'precision': 5, '60s': 60, '30s': 30, '10s': 10}

    def __init__(self, mode, tau=0.0):
        self.min_time = self.MIN_TIMES[mode]
        self.timed = mode not in ('fast', 'precision')
        self.tau = tau
        self.periods = deque(maxlen=60)
        self.t = 0
        self.factor = 1.0
        self.window = 0.0
        self.restarts = 0

    def update(self, count, first_offset):
        """count: pulses in this second; first_offset: tick (1..1000) of the
        first pulse within the second."""
        t = self.t
        self.t += 1
        period_tick = (t + 1) * SYSTICK_FREQUENCY

        # Only non-empty periods are queued
        if count:
            self.periods.appendleft((t * SYSTICK_FREQUENCY + first_offset,
                                     period_tick, count))

        first = last = n = 0
        for first_tick, last_tick, c in self.periods:
            time = (period_tick - first_tick) // SYSTICK_FREQUENCY + 1
            if time > self.min_time and (self.timed or n > 19):
                break
            first = first_tick
            if not n:
                last = last_tick
            n += c

        # calculateRate
        ticks = last - first
        if n < 2:
            rate = 0.0
            confidence = 0.0
        else:
            if n > ticks:
                n += 1
                ticks += 1
            rate = (n - 1) * SYSTICK_FREQUENCY / ticks
            confidence = confidence_interval(n - 1)
        self.window = ticks / SYSTICK_FREQUENCY

        # applyDeadTimeCompensation
        self.factor = dead_time_factor(rate, self.tau) if self.tau else 1.0
        rate *= self.factor

        return rate, confidence


def _oldest_boundary(now):
    return max((now // TIER2_BLOCK - (TIER2_LENGTH - 1)) * TIER2_BLOCK, 0)


def _max_window(now, limit):
    """Largest window (s) the buffer can represent, not longer than limit."""
    if limit <= TIER1_LENGTH:
        return limit
    start = -(-(now - limit) // TIER2_BLOCK) * TIER2_BLOCK
    start = max(start, _oldest_boundary(now))
    return max(now - start, TIER1_LENGTH)


def _round_up_window(now, window, limit):
    """Smallest representable window >= window, or None if beyond limit."""
    if window <= TIER1_LENGTH:
        return window if window <= limit else None
    start = ((now - window) // TIER2_BLOCK) * TIER2_BLOCK
    if start < _oldest_boundary(now):
        return None
    window = now - start
    return window if window <= limit else None


def g_statistic(n1, n2, t1, t2):
    """Likelihood ratio (G) statistic for two Poisson counts with the same
    rate over durations t1 and t2."""
    total = n1 + n2
    if not total:
        return 0.0
    e1 = total * t1 / (t1 + t2)
    e2 = total - e1
    g = 0.0
    if n1:
        g += n1 * math.log(n1 / e1)
    if n2:
        g += n2 * math.log(n2 / e2)
    return 2.0 * g


class ProposedInstantaneous:
    """Redesign.

    pulse_target: adaptive mode, smallest window with at least this many
        pulses (falls back to the longest available window).
    fixed_window: fixed mode (s), estimate n / W, no change detection.
    max_window: optional time cap (s) for adaptive mode.
    change_threshold: G threshold for restarting the window.
    estimator: 'n' (n / W) or 'n-1' ((n - 1) / W) when the pulse target is met.
    detect_only: record the change statistic without restarting.
    hold: adaptive windows end at the end of the most recent second with
        pulses instead of now, so the reading only changes when pulses
        arrive. When the pulse target is met the estimate is
        (n - hold_correction) / W.
    """

    def __init__(self, pulse_target=None, fixed_window=None, max_window=None,
                 change_threshold=math.inf, estimator='n-1',
                 detect_only=False, tau=0.0, hold=False, hold_correction=1):
        self.pulse_target = pulse_target
        self.fixed_window = fixed_window
        self.max_window = max_window
        self.change_threshold = change_threshold
        self.estimator = estimator
        self.detect_only = detect_only
        self.tau = tau
        self.hold = hold
        self.hold_correction = hold_correction
        self.last_pulse_end = 0

        # The firmware keeps only the two ring buffers; the full list is
        # equivalent because only representable windows are used.
        self.cumulative = [0]
        self.restart = 0
        self.restarts = 0
        self.last_g = 0.0
        self.window = 0

    def update(self, count, first_offset=None):
        cumulative = self.cumulative
        cumulative.append(cumulative[-1] + count)
        now = len(cumulative) - 1
        if count:
            self.last_pulse_end = now

        if self.fixed_window:
            window = min(self.fixed_window, now)
            n = cumulative[now] - cumulative[now - window]
            rate = n / window
            confidence = confidence_interval(n)
        else:
            self._detect_change(now)

            limit = now - self.restart
            if self.max_window:
                limit = min(limit, self.max_window)

            # Window end: now, or with hold the end of the most recent
            # second with pulses since the last restart
            end = now
            if self.hold:
                end = self.last_pulse_end if self.last_pulse_end > self.restart else now

            # Smallest window (back from end) holding pulse_target pulses;
            # its start must be representable in the buffer
            window = None
            target = cumulative[end] - self.pulse_target
            if target >= 0:
                j = bisect.bisect_right(cumulative, target) - 1
                window = _round_up_window(now, now - j, limit)

            if window is not None:
                start = now - window
                n = cumulative[end] - cumulative[start]
                window = end - start
                if self.hold:
                    # 'auto': 2 when the window ends at a pulse (low rates),
                    # 1 when it ends at a second boundary (high rates)
                    k = self.hold_correction
                    if k == 'auto':
                        k = 1 + window / (window + n)
                    rate = (n - k) / window
                    confidence = confidence_interval(n - 1)
                elif self.estimator == 'n-1':
                    rate = (n - 1) / window
                    confidence = confidence_interval(n - 1)
                else:
                    rate = n / window
                    confidence = confidence_interval(n)
            else:
                window = _max_window(now, limit)
                n = cumulative[now] - cumulative[now - window]
                rate = n / window
                confidence = confidence_interval(n)

        self.window = window

        if self.tau:
            rate *= dead_time_factor(rate, self.tau)

        return rate, confidence

    def _detect_change(self, now):
        cumulative = self.cumulative
        length = _max_window(now, now - self.restart)

        best_g = 0.0
        best_k = 0
        for k in CHANGE_LENGTHS:
            k = _round_up_window(now, k, length)
            if k is None or 2 * k > length:
                break
            recent = cumulative[now] - cumulative[now - k]
            older = cumulative[now - k] - cumulative[now - length]
            g = g_statistic(recent, older, k, length - k)
            if g > best_g:
                best_g = g
                best_k = k

        self.last_g = best_g
        if (best_g > self.change_threshold) and not self.detect_only:
            self.restart = now - best_k
            self.restarts += 1
