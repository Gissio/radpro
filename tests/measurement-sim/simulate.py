"""
Monte Carlo comparison of Rad Pro instantaneous rate algorithms.

Stages:
  1. calibrate  Change detection threshold for a target false restart rate.
  2. steady     Bias, noise, 95% coverage and window length at constant rates.
  3. steps      Response to steps (x2, x10, x100, up and down).
  4. passby     Walking past a point source (approach and retreat).
  5. deadtime   High rates with a non-paralyzable dead time, and dose.
  6. alerts     Alert policies near the threshold.

Usage: python simulate.py [--quick] [--hold | --history] [--processes N]
Writes results/report.md and plots to results/ (--hold: results/hold.md,
--history: results/history.md).
"""

import argparse
import math
import os
import time
from multiprocessing import Pool

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

from algorithms import (CurrentInstantaneous, ProposedInstantaneous,
                        dead_time_factor)

HERE = os.path.dirname(os.path.abspath(__file__))
RESULTS = os.path.join(HERE, 'results')

# Background count rates of typical tubes (~0.1-0.15 uSv/h)
TUBES = [
    ('HH614 class', 0.07),
    ('SBM-20 class', 0.25),
    ('Pancake class', 1.0),
]

N_FAST = 20
N_PRECISION = 100
CAP_FAST = 60
CAP_PRECISION = 300

G_CANDIDATES = [10, 12, 14, 16, 18, 20, 22, 25, 30]
FALSE_RESTART_TARGET = 0.1      # Per hour, at every rate

TAU = 150E-6                    # Dead time for stage 5 (s)

CHUNK = 10                      # Trials per task


# Pulse generation

def poisson_counts(rng, mean, trials):
    """Per-second counts and the tick (1..1000) of each second's first pulse."""
    mean = np.asarray(mean, dtype=float)
    counts = rng.poisson(mean, size=(trials, mean.size))
    u = rng.random(counts.shape)
    offsets = np.ceil(1000 * (1 - u ** (1 / np.maximum(counts, 1))))
    offsets = np.clip(offsets, 1, 1000).astype(np.int32)
    return counts, offsets


def renewal_times(rng, lam, tau, duration):
    """Measured pulse times of a non-paralyzable counter (renewal process)."""
    chunk = int(duration * lam / (1 + lam * tau) * 1.05) + 1000
    parts = []
    t0 = 0.0
    while t0 < duration:
        ts = t0 + np.cumsum(tau + rng.exponential(1 / lam, chunk))
        parts.append(ts)
        t0 = ts[-1]
    ts = np.concatenate(parts)
    return ts[ts < duration]


def counts_from_times(times, seconds):
    sec = np.floor(times).astype(np.int64)
    counts = np.bincount(sec, minlength=seconds)[:seconds]
    offsets = np.ones(seconds, dtype=np.int32)
    uniq, idx = np.unique(sec, return_index=True)
    offsets[uniq] = np.clip(np.ceil((times[idx] - uniq) * 1000), 1, 1000)
    return counts, offsets


def generate(kind, args, trials, rng):
    if kind == 'poisson':
        return poisson_counts(rng, args, trials)
    if kind == 'renewal':
        lam, tau, seconds = args
        counts = np.empty((trials, seconds), dtype=np.int64)
        offsets = np.empty((trials, seconds), dtype=np.int32)
        for i in range(trials):
            counts[i], offsets[i] = counts_from_times(
                renewal_times(rng, lam, tau, seconds), seconds)
        return counts, offsets
    raise ValueError(kind)


# Algorithms

def make_algorithm(spec):
    kind, kwargs = spec
    if kind == 'current':
        return CurrentInstantaneous(**kwargs)
    return ProposedInstantaneous(**kwargs)


def run_algorithm(spec, counts, offsets):
    trials, seconds = counts.shape
    record_g = (spec[0] == 'proposed') and spec[1].get('detect_only', False)
    est = np.empty((trials, seconds), dtype=np.float32)
    conf = np.empty((trials, seconds), dtype=np.float32)
    window = np.empty((trials, seconds), dtype=np.float32)
    g = np.empty((trials, seconds), dtype=np.float32) if record_g else None
    restarts = np.zeros(trials)

    for i in range(trials):
        alg = make_algorithm(spec)
        c = counts[i].tolist()
        o = offsets[i].tolist()
        e_row, c_row, w_row, g_row = [], [], [], []
        for s in range(seconds):
            r, cf = alg.update(c[s], o[s])
            e_row.append(r)
            c_row.append(cf)
            w_row.append(alg.window)
            if record_g:
                g_row.append(alg.last_g)
        est[i] = e_row
        conf[i] = c_row
        window[i] = w_row
        if record_g:
            g[i] = g_row
        restarts[i] = alg.restarts

    return {'est': est, 'conf': conf, 'window': window, 'g': g,
            'restarts': restarts}


def task_run(job):
    spec, kind, args, trials, seed = job
    rng = np.random.default_rng(seed)
    counts, offsets = generate(kind, args, trials, rng)
    return run_algorithm(spec, counts, offsets)


def run_jobs(pool, jobs, trials):
    """jobs: {key: (spec, kind, args, scenario_seed)}. Algorithms sharing a
    scenario seed see the same pulses (common random numbers)."""
    tasks = []
    keys = []
    for key, (spec, kind, args, seed) in jobs.items():
        for chunk, start in enumerate(range(0, trials, CHUNK)):
            n = min(CHUNK, trials - start)
            tasks.append((spec, kind, args, n, (seed, chunk)))
            keys.append(key)

    results = {}
    for key, result in zip(keys, pool.map(task_run, tasks, chunksize=1)):
        results.setdefault(key, []).append(result)

    merged = {}
    for key, parts in results.items():
        merged[key] = {}
        for name in parts[0]:
            if parts[0][name] is None:
                merged[key][name] = None
            else:
                merged[key][name] = np.concatenate([p[name] for p in parts])
    return merged


def current(mode, tau=0.0):
    return ('current', {'mode': mode, 'tau': tau})


def proposed(g_threshold, estimator, pulse_target=None, fixed_window=None,
             max_window=None, tau=0.0, **extra):
    return ('proposed', {'pulse_target': pulse_target,
                         'fixed_window': fixed_window,
                         'max_window': max_window,
                         'change_threshold': g_threshold,
                         'estimator': estimator,
                         'tau': tau,
                         **extra})


def comparison_specs(g_threshold, estimator, tau=0.0):
    return {
        'Current fast': current('fast', tau),
        'Current precision': current('precision', tau),
        'Proposed fast': proposed(g_threshold, estimator, N_FAST, tau=tau),
        'Proposed precision': proposed(g_threshold, estimator, N_PRECISION, tau=tau),
        'Proposed fast, 60 s cap': proposed(g_threshold, estimator, N_FAST,
                                            max_window=CAP_FAST, tau=tau),
        'Proposed precision, 5 min cap': proposed(g_threshold, estimator, N_PRECISION,
                                                  max_window=CAP_PRECISION, tau=tau),
    }


# Stage 1: calibrate change detection

def stage_calibrate(pool, cfg, report):
    rates = [0.07, 0.25, 1.0, 10.0, 100.0, 1000.0]
    seconds = cfg['calibrate_seconds']
    trials = cfg['calibrate_trials']
    spec = ('proposed', {'pulse_target': N_FAST, 'detect_only': True})

    jobs = {rate: (spec, 'poisson', np.full(seconds, rate), (1, i))
            for i, rate in enumerate(rates)}
    results = run_jobs(pool, jobs, trials)

    hours = trials * seconds / 3600
    table = {}
    for rate in rates:
        g = results[rate]['g']
        prev = np.concatenate([np.zeros((g.shape[0], 1)), g[:, :-1]], axis=1)
        table[rate] = [np.sum((g > thr) & (prev <= thr)) / hours
                       for thr in G_CANDIDATES]

    chosen = G_CANDIDATES[-1]
    for i, thr in enumerate(G_CANDIDATES):
        if all(table[rate][i] <= FALSE_RESTART_TARGET for rate in rates):
            chosen = thr
            break

    report.append('## 1. Change detection threshold\n')
    report.append(f'False restarts per hour at constant rates ({hours:.0f} h '
                  f'simulated per rate). Target: at most {FALSE_RESTART_TARGET}/h '
                  f'at every rate. **Chosen threshold: G > {chosen}.**\n')
    header = '| Rate (cps) | ' + ' | '.join(f'G > {t}' for t in G_CANDIDATES) + ' |'
    report.append(header)
    report.append('|' + '---|' * (len(G_CANDIDATES) + 1))
    for rate in rates:
        cells = ' | '.join(f'{v:.2f}' for v in table[rate])
        report.append(f'| {rate:g} | {cells} |')
    report.append('')
    return chosen


# Stage 2: steady state

def steady_metrics(result, rate, warmup):
    est = result['est'][:, warmup:].astype(float)
    conf = result['conf'][:, warmup:].astype(float)
    window = result['window'][:, warmup:]
    rel = est / rate - 1
    covered = np.abs(est - rate) <= conf * est
    hours = result['est'].shape[0] * result['est'].shape[1] / 3600
    return {
        'bias': 100 * rel.mean(),
        'std': 100 * rel.std(),
        'p99': 100 * np.percentile(np.abs(rel), 99),
        'coverage': 100 * covered.mean(),
        'window': float(np.median(window)),
        'restarts': result['restarts'].sum() / hours,
    }


def steady_table(report, title, metrics, specs, rates):
    report.append(f'### {title}\n')
    report.append('| Algorithm | Rate (cps) | Bias | Std dev | 99th pct error '
                  '| 95% coverage | Median window | Restarts/h |')
    report.append('|---|---|---|---|---|---|---|---|')
    for name in specs:
        for rate in rates:
            m = metrics[(name, rate)]
            report.append(f'| {name} | {rate:g} | {m["bias"]:+.1f}% | {m["std"]:.1f}% '
                          f'| {m["p99"]:.0f}% | {m["coverage"]:.1f}% '
                          f'| {m["window"]:.0f} s | {m["restarts"]:.2f} |')
    report.append('')


def stage_steady(pool, cfg, g_threshold, report):
    rates = [0.07, 0.25, 1.0, 10.0, 100.0]
    seconds = cfg['steady_seconds']
    trials = cfg['steady_trials']
    warmup = cfg['warmup']

    def run(specs):
        jobs = {}
        for name, spec in specs.items():
            for i, rate in enumerate(rates):
                jobs[(name, rate)] = (spec, 'poisson', np.full(seconds, rate), (2, i))
        results = run_jobs(pool, jobs, trials)
        return {key: steady_metrics(r, key[1], warmup) for key, r in results.items()}

    # Estimator choice
    estimator_specs = {
        f'Proposed {mode}, {est}': proposed(g_threshold, est, n)
        for mode, n in (('fast', N_FAST), ('precision', N_PRECISION))
        for est in ('n', 'n-1')
    }
    est_metrics = run(estimator_specs)
    worst = {est: max(abs(m['bias']) for (name, _), m in est_metrics.items()
                      if name.endswith(', ' + est))
             for est in ('n', 'n-1')}
    estimator = min(worst, key=worst.get)

    report.append('## 2. Steady state\n')
    report.append('Coverage: fraction of seconds where the true rate lies within the '
                  'displayed value +/- confidence interval (should be ~95%).\n')
    steady_table(report, 'Estimator when the pulse target is met (n / W vs (n - 1) / W)',
                 est_metrics, estimator_specs, rates)
    report.append(f'Largest absolute bias: n / W {worst["n"]:.1f}%, '
                  f'(n - 1) / W {worst["n-1"]:.1f}%. '
                  f'**Chosen estimator: {"n / W" if estimator == "n" else "(n - 1) / W"}.**\n')

    specs = {'Current 60 s': current('60s'),
             'Proposed 60 s': proposed(g_threshold, estimator, fixed_window=60)}
    specs.update(comparison_specs(g_threshold, estimator))
    metrics = run(specs)
    steady_table(report, 'All algorithms', metrics, specs, rates)
    return estimator


# Stage 3: steps

STEP_DELAYS = [1, 3, 10, 30, 100, 300, 1000]


def settle_time(ratio, delays, tolerance=0.2):
    ok = np.abs(ratio - 1) <= tolerance
    for i in range(len(ok)):
        if ok[i:].all():
            return delays[i]
    return None


def stage_steps(pool, cfg, g_threshold, estimator, report):
    pre = cfg['step_pre']
    post = cfg['step_post']
    trials = cfg['step_trials']
    specs = comparison_specs(g_threshold, estimator)

    scenarios = []
    for tube, bg in TUBES:
        for factor in (2, 10, 100):
            for direction in ('up', 'down'):
                before, after = (bg, factor * bg) if direction == 'up' else (factor * bg, bg)
                scenarios.append((tube, factor, direction, before, after))

    jobs = {}
    for si, (tube, factor, direction, before, after) in enumerate(scenarios):
        mean = np.concatenate([np.full(pre, before), np.full(post, after)])
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', mean, (3, si))
    results = run_jobs(pool, jobs, trials)

    delays = np.arange(1, post + 1)
    shown = [d for d in STEP_DELAYS if d <= post]

    report.append('## 3. Steps\n')
    report.append('Mean reading (over trials) divided by the new true rate, at '
                  'several delays after the step. Settle: first delay after which '
                  'the mean stays within +/-20% of the new rate. Plots show the '
                  'median and the 10-90% band.\n')
    for tube, bg in TUBES:
        report.append(f'### {tube} (background {bg} cps)\n')
        report.append('| Step | Algorithm | ' + ' | '.join(f'{d} s' for d in shown)
                      + ' | Settle |')
        report.append('|---|---|' + '---|' * (len(shown) + 1))
        for si, (t, factor, direction, before, after) in enumerate(scenarios):
            if t != tube:
                continue
            for name in specs:
                est = results[(si, name)]['est']
                ratio = np.mean(est[:, pre:], axis=0) / after
                cells = ' | '.join(f'{ratio[d - 1]:.2f}' for d in shown)
                st = settle_time(ratio, delays)
                st = f'{st} s' if st is not None else f'> {post} s'
                report.append(f'| x{factor} {direction} | {name} | {cells} | {st} |')
        report.append('')
        plot_steps(results, scenarios, specs, tube, pre, post)
        report.append(f'![Steps, {tube}](steps-{slug(tube)}.png)\n')


def slug(text):
    return text.lower().replace(' ', '-')


COLORS = {'Current': 'tab:red', 'Proposed': 'tab:blue', 'cap': 'tab:green'}


def color_for(name):
    if 'cap' in name:
        return COLORS['cap']
    return COLORS['Current'] if name.startswith('Current') else COLORS['Proposed']


def plot_band(ax, x, est, name):
    med = np.median(est, axis=0)
    lo, hi = np.percentile(est, [10, 90], axis=0)
    color = color_for(name)
    ax.plot(x, med, color=color, label=name, lw=1.2)
    ax.fill_between(x, np.maximum(lo, 1e-3), np.maximum(hi, 1e-3), color=color,
                    alpha=0.12, lw=0)


def plot_steps(results, scenarios, specs, tube, pre, post):
    cols = [(10, 'up'), (10, 'down'), (100, 'up'), (100, 'down')]
    fig, axes = plt.subplots(2, 4, figsize=(18, 8), sharex=True)
    lead = 60
    x = np.arange(-lead, post) + 1
    for row, mode in enumerate(('fast', 'precision')):
        for col, (factor, direction) in enumerate(cols):
            ax = axes[row, col]
            si = next(i for i, s in enumerate(scenarios)
                      if s[0] == tube and s[1] == factor and s[2] == direction)
            _, _, _, before, after = scenarios[si]
            for name in specs:
                if mode not in name:
                    continue
                est = results[(si, name)]['est'][:, pre - lead:pre + post]
                plot_band(ax, x, est, name)
            ax.plot(x, np.where(x <= 0, before, after), 'k--', lw=0.8, label='True rate')
            ax.set_yscale('log')
            ax.set_title(f'{mode}: x{factor} {direction}')
            ax.grid(alpha=0.3, which='both')
            if col == 0:
                ax.set_ylabel('Rate (cps)')
            if row == 1:
                ax.set_xlabel('Time after step (s)')
        axes[row, 0].legend(fontsize=8)
    fig.suptitle(f'Steps, {tube} (median and 10-90% band)')
    fig.tight_layout()
    fig.savefig(os.path.join(RESULTS, f'steps-{slug(tube)}.png'), dpi=90)
    plt.close(fig)


# Stage 4: pass-by

def passby_profile(bg, factor, fwhm, tc, seconds, sub=10):
    t = (np.arange(seconds * sub) + 0.5) / sub
    rate = bg * (1 + factor / (1 + ((t - tc) / (fwhm / 2)) ** 2))
    return rate.reshape(seconds, sub).mean(axis=1)


def stage_passby(pool, cfg, g_threshold, estimator, report):
    pre = cfg['passby_pre']
    post = cfg['passby_post']
    trials = cfg['passby_trials']
    specs = comparison_specs(g_threshold, estimator)
    seconds = pre + post

    scenarios = []
    for tube, bg in TUBES:
        for factor in (10, 100):
            for fwhm in (10, 60):
                scenarios.append((tube, bg, factor, fwhm))

    jobs = {}
    profiles = {}
    for si, (tube, bg, factor, fwhm) in enumerate(scenarios):
        profiles[si] = passby_profile(bg, factor, fwhm, pre, seconds)
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', profiles[si], (4, si))
    results = run_jobs(pool, jobs, trials)

    report.append('## 4. Walking past a source\n')
    report.append('Rate = background x (1 + peak / (1 + ((t - tc) / (FWHM / 2))^2)). '
                  'From the mean trajectory: peak reading / true peak, delay of '
                  'the reading peak, and mean reading / true rate over the retreat '
                  '(tc + FWHM to tc + 10 FWHM).\n')
    report.append('| Tube | Peak | FWHM | Algorithm | Peak ratio | Peak delay | Retreat ratio |')
    report.append('|---|---|---|---|---|---|---|')
    for si, (tube, bg, factor, fwhm) in enumerate(scenarios):
        true = profiles[si]
        tpk = int(np.argmax(true))
        for name in specs:
            med = np.mean(results[(si, name)]['est'], axis=0)
            region = slice(max(tpk - 3 * fwhm, 0), min(tpk + 10 * fwhm, seconds))
            rpk = region.start + int(np.argmax(med[region]))
            retreat = slice(tpk + fwhm, min(tpk + 10 * fwhm, seconds))
            report.append(f'| {tube} | x{factor} | {fwhm} s | {name} '
                          f'| {med[rpk] / true[tpk]:.2f} | {rpk - tpk:+d} s '
                          f'| {np.mean(med[retreat] / true[retreat]):.2f} |')
    report.append('')

    for tube, bg in TUBES:
        plot_passby(results, scenarios, specs, profiles, tube, pre)
        report.append(f'![Pass-by, {tube}](passby-{slug(tube)}.png)\n')


def plot_passby(results, scenarios, specs, profiles, tube, tc):
    cols = [(10, 10), (10, 60), (100, 10), (100, 60)]
    fig, axes = plt.subplots(2, 4, figsize=(18, 8))
    for row, mode in enumerate(('fast', 'precision')):
        for col, (factor, fwhm) in enumerate(cols):
            ax = axes[row, col]
            si = next(i for i, s in enumerate(scenarios)
                      if s[0] == tube and s[2] == factor and s[3] == fwhm)
            span = slice(max(tc - 5 * fwhm, 0),
                         min(tc + 15 * fwhm, profiles[si].size))
            x = np.arange(span.start, span.stop) - tc
            for name in specs:
                if mode not in name:
                    continue
                plot_band(ax, x, results[(si, name)]['est'][:, span], name)
            ax.plot(x, profiles[si][span], 'k--', lw=0.8, label='True rate')
            ax.set_yscale('log')
            ax.set_title(f'{mode}: peak x{factor}, FWHM {fwhm} s')
            ax.grid(alpha=0.3, which='both')
            if col == 0:
                ax.set_ylabel('Rate (cps)')
            if row == 1:
                ax.set_xlabel('Time from closest approach (s)')
        axes[row, 0].legend(fontsize=8)
    fig.suptitle(f'Pass-by, {tube} (median and 10-90% band)')
    fig.tight_layout()
    fig.savefig(os.path.join(RESULTS, f'passby-{slug(tube)}.png'), dpi=90)
    plt.close(fig)


# Stage 5: dead time

def task_dose(job):
    bg, lam, tau, pre, high, post, trials, seed, g_threshold, estimator = job
    rng = np.random.default_rng(seed)
    seconds = pre + high + post
    out = {k: [] for k in ('raw', 'current fast', 'current precision', 'current 60 s',
                           'per second')}
    for _ in range(trials):
        counts = rng.poisson(bg, seconds)
        offsets = np.full(seconds, 500, dtype=np.int32)
        hc, ho = counts_from_times(renewal_times(rng, lam, tau, high), high)
        counts[pre:pre + high] = hc
        offsets[pre:pre + high] = ho
        true = bg * (pre + post) + lam * high

        doses = {'raw': counts.sum()}
        for label, mode in (('current fast', 'fast'), ('current precision', 'precision'),
                            ('current 60 s', '60s')):
            alg = CurrentInstantaneous(mode, tau)
            dose = 0.0
            for c, o in zip(counts.tolist(), offsets.tolist()):
                alg.update(c, o)
                dose += alg.factor * c
            doses[label] = dose
        doses['per second'] = sum(c * dead_time_factor(c, tau) for c in counts.tolist())
        for k, v in doses.items():
            out[k].append(100 * (v / true - 1))
    return out


def stage_deadtime(pool, cfg, g_threshold, estimator, report):
    rates = [1000.0, 5000.0, 20000.0]
    seconds = cfg['deadtime_seconds']
    trials = cfg['deadtime_trials']
    warmup = 5

    specs = {
        'Current fast': current('fast', TAU),
        'Proposed fast': proposed(g_threshold, estimator, N_FAST, tau=TAU),
        'Proposed precision': proposed(g_threshold, estimator, N_PRECISION, tau=TAU),
        'Proposed 60 s': proposed(g_threshold, estimator, fixed_window=60, tau=TAU),
    }
    jobs = {}
    for i, lam in enumerate(rates):
        for name, spec in specs.items():
            jobs[(name, lam)] = (spec, 'renewal', (lam, TAU, seconds), (5, i))
    results = run_jobs(pool, jobs, trials)

    report.append('## 5. Dead time\n')
    report.append(f'Non-paralyzable dead time {TAU * 1E6:.0f} us, measured pulses '
                  'simulated as a renewal process, compensation with the same dead '
                  'time. Confidence from raw counts, not widened by the '
                  'compensation factor.\n')
    report.append('| Algorithm | True rate (cps) | Measured (cps) | Bias | Std dev '
                  '| 95% coverage | Restarts/h |')
    report.append('|---|---|---|---|---|---|---|')
    for name in specs:
        for lam in rates:
            m = steady_metrics(results[(name, lam)], lam, warmup)
            report.append(f'| {name} | {lam:g} | {lam / (1 + lam * TAU):.0f} '
                          f'| {m["bias"]:+.1f}% | {m["std"]:.1f}% '
                          f'| {m["coverage"]:.1f}% | {m["restarts"]:.2f} |')
    report.append('')

    # Dose: background, 60 s at 5000 cps, background
    dose_trials = cfg['dose_trials']
    jobs = [(0.25, 5000.0, TAU, 300, 60, 300, CHUNK, (6, i), g_threshold, estimator)
            for i in range(max(dose_trials // CHUNK, 1))]
    merged = {}
    for part in pool.map(task_dose, jobs):
        for k, v in part.items():
            merged.setdefault(k, []).extend(v)

    report.append('### Dose over a 60 s exposure at 5000 cps\n')
    report.append('300 s background (0.25 cps), 60 s at 5000 cps true, 300 s '
                  'background. Dose error vs the true number of pulses.\n')
    report.append('| Compensation | Mean dose error | Std dev |')
    report.append('|---|---|---|')
    labels = {'raw': 'None (raw counts)',
              'current fast': 'Current, factor from instantaneous (adaptive fast)',
              'current precision': 'Current, factor from instantaneous (adaptive precision)',
              'current 60 s': 'Current, factor from instantaneous (60 s)',
              'per second': 'Proposed, factor from each second\'s count'}
    for k, label in labels.items():
        v = np.array(merged[k])
        report.append(f'| {label} | {v.mean():+.1f}% | {v.std():.1f}% |')
    report.append('')


# Stage 6: alerts

ALERT_THRESHOLDS = [0.3, 3.0, 30.0]                   # cps
ALERT_RATIOS = [0.5, 0.8, 0.9, 1.0, 1.1, 1.25, 1.5, 2.0, 3.0]
# Policy: (raise factor, clear factor) on the 95% confidence half-width c.
# Raise when gate and reading * (1 - raise * c) >= threshold; clear when
# reading * (1 + clear * c) < threshold. 'none' is the current firmware
# (alert exactly while gate and reading >= threshold). Half of the 95%
# interval is about one standard deviation.
ALERT_POLICIES = {
    'none': None,
    'hysteresis': (0, 1),
    'hysteresis 1σ': (0, 0.5),
    'strict': (1, 1),
    'strict 1σ': (0.5, 0.5),
}
ALERT_CONFIDENCE_GATE = 0.75                           # Firmware gate (adaptive modes)


def alert_states(est, conf, gate, threshold, policy):
    """Per-second alert state for one alert policy."""
    factors = ALERT_POLICIES[policy]
    if factors is None:
        return gate & (est >= threshold)
    raise_factor, clear_factor = factors
    raise_on = gate & (est * (1 - raise_factor * conf) >= threshold)
    clear_on = est * (1 + clear_factor * conf) < threshold
    active = np.zeros_like(raise_on)
    state = np.zeros(est.shape[0], dtype=bool)
    for t in range(est.shape[1]):
        state = np.where(state, ~clear_on[:, t], raise_on[:, t])
        active[:, t] = state
    return active


def task_alerts(job):
    spec, timed, mean, threshold, pre, settle, trials, seed = job
    rng = np.random.default_rng(seed)
    counts, offsets = poisson_counts(rng, mean, trials)
    r = run_algorithm(spec, counts, offsets)
    est = r['est'].astype(float)
    conf = r['conf'].astype(float)
    gate = np.ones_like(est, dtype=bool) if timed else \
        (conf > 0) & (conf < ALERT_CONFIDENCE_GATE)

    out = {}
    for policy in ALERT_POLICIES:
        active = alert_states(est, conf, gate, threshold, policy)
        prev = np.concatenate([np.zeros((trials, 1), dtype=bool), active[:, :-1]], axis=1)
        raised = active & ~prev
        delays = []
        for i in range(trials):
            hits = np.flatnonzero(raised[i, pre:])
            delays.append(hits[0] + 1 if hits.size else math.inf)
        steady = slice(pre + settle, est.shape[1])
        out[policy] = {'raises': int(raised[:, steady].sum()),
                       'active': float(active[:, steady].sum()),
                       'seconds': trials * (est.shape[1] - pre - settle),
                       'delays': delays}
    return out


def stage_alerts(pool, cfg, g_threshold, estimator, report):
    pre = cfg['alert_pre']
    post = cfg['alert_post']
    settle = cfg['alert_settle']
    trials = cfg['alert_trials']

    algorithms = []
    for mode, n, old_mode in (('fast', N_FAST, 'fast'),
                              ('precision', N_PRECISION, 'precision')):
        algorithms += [
            (mode, 'Old', current(old_mode), False),
            (mode, 'New + change detection', proposed(g_threshold, estimator, n), False),
        ]
    algorithms += [
        ('1 minute', 'Old', current('60s'), True),
        ('1 minute', 'New', proposed(math.inf, estimator, fixed_window=60), True),
    ]

    jobs = []
    keys = []
    for ti, threshold in enumerate(ALERT_THRESHOLDS):
        for ri, ratio in enumerate(ALERT_RATIOS):
            mean = np.concatenate([np.full(pre, 0.2 * threshold),
                                   np.full(post, ratio * threshold)])
            for ai, (mode, name, spec, timed) in enumerate(algorithms):
                for chunk, start in enumerate(range(0, trials, CHUNK)):
                    n = min(CHUNK, trials - start)
                    jobs.append((spec, timed, mean, threshold, pre, settle, n,
                                 (7, ti, ri, chunk)))
                    keys.append((ti, ri, ai))

    merged = {}
    for key, part in zip(keys, pool.map(task_alerts, jobs, chunksize=1)):
        m = merged.setdefault(key, {p: {'raises': 0, 'active': 0.0, 'seconds': 0, 'delays': []}
                                    for p in ALERT_POLICIES})
        for p in ALERT_POLICIES:
            for k in ('raises', 'active', 'seconds'):
                m[p][k] += part[p][k]
            m[p]['delays'] += part[p]['delays']

    report.append('## 6. Alerts\n')
    report.append(f'The true rate jumps from 0.2x the alert threshold to the ratio shown '
                  f'and stays there for {post} s. Alert starts per hour and time in alert '
                  f'are measured after the first {settle} s; delay is the median time to '
                  'the first alert after the jump.\n')
    report.append('Policies (c = 95% confidence half-width, c/2 = about 1σ):\n')
    report.append('- **none**: current firmware, alert while reading >= threshold.')
    report.append('- **hysteresis**: raise at reading >= threshold; clear when '
                  'reading x (1 + c) < threshold.')
    report.append('- **hysteresis 1σ**: as hysteresis, clear when reading x (1 + c/2) < threshold.')
    report.append('- **strict**: raise when reading x (1 - c) >= threshold; clear when '
                  'reading x (1 + c) < threshold.')
    report.append('- **strict 1σ**: as strict, with c/2.\n')
    report.append('Adaptive modes only raise when the confidence interval is below '
                  f'+/-{ALERT_CONFIDENCE_GATE * 100:.0f}% (firmware gate).\n')

    starts_ratios = [0.5, 0.8, 0.9, 1.0, 1.1, 1.25]
    active_ratios = [0.8, 0.9, 1.0, 1.1, 1.25, 1.5]
    delay_ratios = [1.1, 1.25, 1.5, 2.0, 3.0]

    def cell(ti, ratio, ai, policy, what):
        m = merged[(ti, ALERT_RATIOS.index(ratio), ai)][policy]
        if what == 'starts':
            return f'{m["raises"] / (m["seconds"] / 3600):.1f}'
        if what == 'active':
            return f'{100 * m["active"] / m["seconds"]:.0f}%'
        d = np.median(m['delays'])
        return '-' if math.isinf(d) else f'{d:.0f} s'

    for ti, threshold in enumerate(ALERT_THRESHOLDS):
        report.append(f'### Threshold {threshold:g} cps\n')
        for what, ratios, title in (('starts', starts_ratios, 'Alert starts per hour'),
                                    ('active', active_ratios, 'Time in alert'),
                                    ('delay', delay_ratios, 'Delay to first alert')):
            report.append(f'**{title}** (true rate / threshold):\n')
            report.append('| Mode | Algorithm | Policy | ' +
                          ' | '.join(f'{r:g}x' for r in ratios) + ' |')
            report.append('|---|---|---|' + '---|' * len(ratios))
            for ai, (mode, name, spec, timed) in enumerate(algorithms):
                policies = ['none'] if name == 'Old' else list(ALERT_POLICIES)
                if what == 'delay':
                    # Hysteresis raises like none: same delay
                    policies = [p for p in policies if not p.startswith('hysteresis')]
                for policy in policies:
                    cells = ' | '.join(cell(ti, r, ai, policy, what) for r in ratios)
                    report.append(f'| {mode} | {name} | {policy} | {cells} |')
            report.append('')


# Hold comparison (python simulate.py --hold): sawtooth vs. holding the
# reading between pulses

HOLD_G_THRESHOLD = 22
VISIBLE_CHANGE = 0.001          # Relative change that shows on a 4-digit display


def hold_specs():
    specs = {}
    for mode, n in (('fast', N_FAST), ('precision', N_PRECISION)):
        specs[f'Old {mode}'] = current(mode)
        specs[f'Sawtooth {mode}'] = proposed(HOLD_G_THRESHOLD, 'n-1', n)
        specs[f'Hold (n-1) {mode}'] = proposed(HOLD_G_THRESHOLD, 'n-1', n, hold=True,
                                               hold_correction=1)
        specs[f'Hold (n-2) {mode}'] = proposed(HOLD_G_THRESHOLD, 'n-1', n, hold=True,
                                               hold_correction=2)
        specs[f'Hold (auto) {mode}'] = proposed(HOLD_G_THRESHOLD, 'n-1', n, hold=True,
                                                hold_correction='auto')
    return specs


def steadiness(result, warmup):
    est = result['est'][:, warmup:].astype(float)
    prev = est[:, :-1]
    cur = est[:, 1:]
    rel = np.abs(cur - prev) / np.maximum(np.maximum(cur, prev), 1e-12)
    return {'changes': 60 * np.mean(rel > VISIBLE_CHANGE),
            'step': 100 * np.mean(rel)}


def stage_hold(pool, cfg, report):
    specs = hold_specs()
    warmup = cfg['warmup']

    # Steady state
    rates = [0.07, 0.25, 1.0, 10.0, 100.0]
    seconds = cfg['steady_seconds']
    jobs = {}
    for name, spec in specs.items():
        for i, rate in enumerate(rates):
            jobs[(name, rate)] = (spec, 'poisson', np.full(seconds, rate), (8, i))
    results = run_jobs(pool, jobs, cfg['steady_trials'])

    report.append('## Steady state\n')
    report.append('Changes/min: seconds per minute in which the reading changes by more '
                  f'than {VISIBLE_CHANGE * 100:g}% (visible on a 4-digit display). Mean step: '
                  'mean absolute change from one second to the next.\n')
    report.append('| Algorithm | Rate (cps) | Bias | Std dev | 95% coverage | Median window '
                  '| Changes/min | Mean step |')
    report.append('|---|---|---|---|---|---|---|---|')
    for name in specs:
        for rate in rates:
            m = steady_metrics(results[(name, rate)], rate, warmup)
            s = steadiness(results[(name, rate)], warmup)
            report.append(f'| {name} | {rate:g} | {m["bias"]:+.1f}% | {m["std"]:.1f}% '
                          f'| {m["coverage"]:.1f}% | {m["window"]:.0f} s '
                          f'| {s["changes"]:.1f} | {s["step"]:.2f}% |')
    report.append('')

    # Steps
    pre = cfg['step_pre']
    post = cfg['step_post']
    scenarios = []
    for tube, bg in TUBES:
        for factor in (2, 10, 100):
            for direction in ('up', 'down'):
                before, after = (bg, factor * bg) if direction == 'up' else (factor * bg, bg)
                scenarios.append((tube, factor, direction, before, after))
    jobs = {}
    for si, (tube, factor, direction, before, after) in enumerate(scenarios):
        mean = np.concatenate([np.full(pre, before), np.full(post, after)])
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', mean, (9, si))
    results = run_jobs(pool, jobs, cfg['step_trials'])

    delays = np.arange(1, post + 1)
    shown = [d for d in STEP_DELAYS if d <= post]
    report.append('## Steps\n')
    report.append('Mean reading / new true rate after the step; settle = first delay after '
                  'which the mean stays within +/-20%.\n')
    for tube, bg in TUBES:
        report.append(f'### {tube} (background {bg} cps)\n')
        report.append('| Step | Algorithm | ' + ' | '.join(f'{d} s' for d in shown) + ' | Settle |')
        report.append('|---|---|' + '---|' * (len(shown) + 1))
        for si, (t, factor, direction, before, after) in enumerate(scenarios):
            if t != tube:
                continue
            for name in specs:
                ratio = np.mean(results[(si, name)]['est'][:, pre:], axis=0) / after
                cells = ' | '.join(f'{ratio[d - 1]:.2f}' for d in shown)
                st = settle_time(ratio, delays)
                st = f'{st} s' if st is not None else f'> {post} s'
                report.append(f'| x{factor} {direction} | {name} | {cells} | {st} |')
        report.append('')

    # Pass-by
    pre = cfg['passby_pre']
    post = cfg['passby_post']
    seconds = pre + post
    passbys = [(tube, bg, factor, fwhm) for tube, bg in TUBES
               for factor in (10, 100) for fwhm in (10, 60)]
    jobs = {}
    profiles = {}
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        profiles[si] = passby_profile(bg, factor, fwhm, pre, seconds)
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', profiles[si], (10, si))
    results = run_jobs(pool, jobs, cfg['passby_trials'])

    report.append('## Walking past a source\n')
    report.append('Mean trajectory: peak reading / true peak, delay of the reading peak, '
                  'mean reading / true rate over the retreat (tc + FWHM to tc + 10 FWHM).\n')
    report.append('| Tube | Peak | FWHM | Algorithm | Peak ratio | Peak delay | Retreat ratio |')
    report.append('|---|---|---|---|---|---|---|')
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        true = profiles[si]
        tpk = int(np.argmax(true))
        for name in specs:
            mean = np.mean(results[(si, name)]['est'], axis=0)
            region = slice(max(tpk - 3 * fwhm, 0), min(tpk + 10 * fwhm, seconds))
            rpk = region.start + int(np.argmax(mean[region]))
            retreat = slice(tpk + fwhm, min(tpk + 10 * fwhm, seconds))
            report.append(f'| {tube} | x{factor} | {fwhm} s | {name} '
                          f'| {mean[rpk] / true[tpk]:.2f} | {rpk - tpk:+d} s '
                          f'| {np.mean(mean[retreat] / true[retreat]):.2f} |')
    report.append('')

    # Alerts with hysteresis
    pre = cfg['alert_pre']
    post = cfg['alert_post']
    settle = cfg['alert_settle']
    trials = cfg['alert_trials']
    ratios = [0.8, 1.0, 1.25, 2.0]
    jobs = []
    keys = []
    for ti, threshold in enumerate(ALERT_THRESHOLDS):
        for ri, ratio in enumerate(ratios):
            mean = np.concatenate([np.full(pre, 0.2 * threshold), np.full(post, ratio * threshold)])
            for name, spec in specs.items():
                for chunk, start in enumerate(range(0, trials, CHUNK)):
                    jobs.append((spec, False, mean, threshold, pre, settle,
                                 min(CHUNK, trials - start), (11, ti, ri, chunk)))
                    keys.append((ti, ri, name))
    merged = {}
    for key, part in zip(keys, pool.map(task_alerts, jobs, chunksize=1)):
        m = merged.setdefault(key, {'raises': 0, 'active': 0.0, 'seconds': 0, 'delays': []})
        p = part['none' if key[2].startswith('Old') else 'hysteresis']
        for k in ('raises', 'active', 'seconds'):
            m[k] += p[k]
        m['delays'] += p['delays']

    report.append('## Alerts\n')
    report.append('Old: current firmware alerts (no hysteresis). Others: hysteresis (raise at '
                  'the threshold, clear below threshold / (1 + c)). Starts/h and time in alert '
                  f'after {settle} s; delay = median time to the first alert.\n')
    for ti, threshold in enumerate(ALERT_THRESHOLDS):
        report.append(f'### Threshold {threshold:g} cps\n')
        report.append('| Algorithm | ' + ' | '.join(f'Starts/h {r:g}x' for r in ratios) + ' | '
                      + ' | '.join(f'In alert {r:g}x' for r in ratios) + ' | '
                      + ' | '.join(f'Delay {r:g}x' for r in ratios[2:]) + ' |')
        report.append('|---|' + '---|' * (2 * len(ratios) + len(ratios[2:])))
        for name in specs:
            ms = [merged[(ti, ri, name)] for ri in range(len(ratios))]
            starts = ' | '.join(f'{m["raises"] / (m["seconds"] / 3600):.1f}' for m in ms)
            active = ' | '.join(f'{100 * m["active"] / m["seconds"]:.0f}%' for m in ms)
            dl = []
            for m in ms[2:]:
                d = np.median(m['delays'])
                dl.append('-' if math.isinf(d) else f'{d:.0f} s')
            report.append(f'| {name} | {starts} | {active} | {" | ".join(dl)} |')
        report.append('')


# History comparison (python simulate.py --history)

# Tabs (bin length for the 201-bin color layout: tab length / 200)
HISTORY_TABS = [('10 minutes', 3), ('1 hour', 18), ('1 day', 432)]
HISTORY_PULSE_MIN = 20          # Hybrid: bins with at least this many pulses use counts
HISTORY_METHODS = ['Reading, gated', 'Reading, no gate', 'Counts',
                   'Hybrid, gated', 'Hybrid, no gate']


def task_history(job):
    """Runs the instantaneous rate on Poisson counts, returning counts, the
    reading and its confidence per second."""
    spec, mean, trials, seed = job
    rng = np.random.default_rng(seed)
    counts, offsets = poisson_counts(rng, mean, trials)
    r = run_algorithm(spec, counts, offsets)
    return counts.astype(np.int32), r['est'], r['conf']


def run_history_jobs(pool, jobs, trials):
    tasks, keys = [], []
    for key, (spec, mean, seed) in jobs.items():
        for chunk, start in enumerate(range(0, trials, CHUNK)):
            tasks.append((spec, mean, min(CHUNK, trials - start), (seed, chunk)))
            keys.append(key)
    merged = {}
    for key, part in zip(keys, pool.map(task_history, tasks, chunksize=1)):
        merged.setdefault(key, []).append(part)
    return {key: tuple(np.concatenate([p[i] for p in parts]) for i in range(3))
            for key, parts in merged.items()}


def history_bins(counts, est, conf, start, length, method):
    """Bin values like the firmware's updateHistory: the reading averaged over
    the bin (gated: only seconds with confidence below +/-75%), pulses / time,
    or the hybrid of both."""
    trials, seconds = counts.shape
    bins = (seconds - start) // length
    span = slice(start, start + bins * length)
    c = counts[:, span].reshape(trials, bins, length).sum(axis=2)
    e = est[:, span].astype(float).reshape(trials, bins, length)
    k = conf[:, span].astype(float).reshape(trials, bins, length)

    count_rate = c / length
    if method == 'Counts':
        return count_rate
    gate = (k > 0) & (k < ALERT_CONFIDENCE_GATE) if 'gated' in method else np.ones_like(e, dtype=bool)
    weight = gate.sum(axis=2)
    reading = np.where(weight > 0, (e * gate).sum(axis=2) / np.maximum(weight, 1), 0.0)
    if method.startswith('Hybrid'):
        return np.where(c >= HISTORY_PULSE_MIN, count_rate, reading)
    return reading


def stage_history(pool, cfg, report):
    modes = [('fast', N_FAST), ('precision', N_PRECISION)]
    specs = {mode: proposed(HOLD_G_THRESHOLD, 'n-1', n) for mode, n in modes}

    # Steady state
    rates = [0.07, 0.25, 1.0, 10.0]
    seconds = cfg['history_seconds']
    warmup = cfg['warmup']
    jobs = {(mode, rate): (specs[mode], np.full(seconds, rate), (12, i))
            for mode in specs for i, rate in enumerate(rates)}
    steady = run_history_jobs(pool, jobs, cfg['history_trials'])

    report.append('## Steady state\n')
    report.append('Per bin: bias (mean / true rate - 1), scatter (standard deviation of '
                  'bin / true rate), and empty bins (bins that read 0, the "comb" at low '
                  'count rates).\n')
    for tab, length in HISTORY_TABS:
        report.append(f'### {tab} tab ({length} s bins)\n')
        report.append('| Mode | Method | ' + ' | '.join(f'{r:g} cps' for r in rates) + ' |')
        report.append('|---|---|' + '---|' * len(rates))
        for mode in specs:
            for method in HISTORY_METHODS:
                cells = []
                for rate in rates:
                    counts, est, conf = steady[(mode, rate)]
                    v = history_bins(counts, est, conf, warmup, length, method) / rate
                    cells.append(f'{100 * (v.mean() - 1):+.1f}% / {100 * v.std():.0f}% / '
                                 f'{100 * np.mean(v == 0):.0f}%')
                report.append(f'| {mode} | {method} | {" | ".join(cells)} |')
        report.append('')
    report.append('Cells: bias / scatter / empty bins.\n')

    # Steps: background, 10x for 1200 s, background
    pre, high, post = 1728, 1296, 1728          # Multiples of every bin length
    trials = cfg['history_step_trials']
    jobs = {}
    for ti, (tube, bg) in enumerate(TUBES):
        mean = np.concatenate([np.full(pre, bg), np.full(high, 10 * bg), np.full(post, bg)])
        for mode in specs:
            jobs[(tube, mode)] = (specs[mode], mean, (13, ti))
    steps = run_history_jobs(pool, jobs, trials)

    report.append('## Steps (x10 up, then back down)\n')
    report.append('Time until the mean bin value stays within +/-20% of the new rate, '
                  'counted from the step, in seconds (bins are aligned with the steps).\n')
    report.append('| Tube | Mode | Method | ' +
                  ' | '.join(f'{tab} up | {tab} down' for tab, _ in HISTORY_TABS) + ' |')
    report.append('|---|---|---|' + '---|' * (2 * len(HISTORY_TABS)))
    for tube, bg in TUBES:
        for mode in specs:
            counts, est, conf = steps[(tube, mode)]
            for method in HISTORY_METHODS:
                cells = []
                for tab, length in HISTORY_TABS:
                    v = history_bins(counts, est, conf, 0, length, method).mean(axis=0)
                    t_end = (np.arange(v.size) + 1) * length
                    for t0, t1, rate in ((pre, pre + high, 10 * bg), (pre + high, None, bg)):
                        sel = (t_end > t0) & ((t_end <= t1) if t1 else True)
                        ratio = v[sel] / rate
                        st = settle_time(ratio, t_end[sel] - t0)
                        cells.append(f'{st} s' if st is not None else '-')
                report.append(f'| {tube} | {mode} | {method} | {" | ".join(cells)} |')
    report.append('')

    # Example plots: one run per tube, fast mode
    for tube, bg in TUBES:
        counts, est, conf = steps[(tube, 'fast')]
        fig, axes = plt.subplots(len(HISTORY_TABS), 1, figsize=(12, 10))
        for ax, (tab, length) in zip(axes, HISTORY_TABS):
            window = (pre - 200 * length // 2, pre + high + 200 * length // 2)
            if tab == '1 day':
                window = (0, counts.shape[1])
            for method, color in zip(['Reading, gated', 'Counts', 'Hybrid, no gate'],
                                     ['tab:red', 'tab:gray', 'tab:blue']):
                v = history_bins(counts[:1], est[:1], conf[:1], 0, length, method)[0]
                t = (np.arange(v.size) + 0.5) * length
                sel = (t >= window[0]) & (t <= window[1])
                ax.step(t[sel] - pre, np.maximum(v[sel], bg / 30), where='mid', color=color,
                        lw=1, label=method, alpha=0.9 if color != 'tab:gray' else 0.5)
            truth = np.concatenate([np.full(pre, bg), np.full(high, 10 * bg), np.full(post, bg)])
            tt = np.arange(truth.size)
            sel = (tt >= window[0]) & (tt <= window[1])
            ax.plot(tt[sel] - pre, truth[sel], 'k--', lw=0.8, label='True rate')
            ax.set_yscale('log')
            ax.set_title(f'{tab} tab ({length} s bins)')
            ax.set_ylabel('Rate (cps)')
            ax.grid(alpha=0.3, which='both')
        axes[0].legend(fontsize=8)
        axes[-1].set_xlabel('Time from step up (s)')
        fig.suptitle(f'History, {tube} (one run, adaptive fast; zero bins drawn at the bottom)')
        fig.tight_layout()
        fig.savefig(os.path.join(RESULTS, f'history-{slug(tube)}.png'), dpi=90)
        plt.close(fig)
        report.append(f'![History, {tube}](history-{slug(tube)}.png)\n')


# Max value and startup (python simulate.py --max)

MAX_GATES = ['current', 'uniform']


def confidence_gate(conf, timed, gate):
    """current: adaptive modes need confidence narrower than +/-75%, the
    1-minute mode always passes. uniform: +/-75% in every mode."""
    good = (conf > 0) & (conf < ALERT_CONFIDENCE_GATE)
    if timed and gate == 'current':
        return np.ones_like(good)
    return good


def running_max(est, gate_mask, end):
    return np.max(np.where(gate_mask[:, :end], est[:, :end], 0.0), axis=1)


def stage_max(pool, cfg, report):
    modes = [('fast', proposed(HOLD_G_THRESHOLD, 'n-1', N_FAST), False),
             ('precision', proposed(HOLD_G_THRESHOLD, 'n-1', N_PRECISION), False),
             ('1 minute', proposed(HOLD_G_THRESHOLD, 'n-1', fixed_window=60), True)]

    # Steady background from a reset
    seconds = cfg['max_seconds']
    jobs = {(mode, tube): (spec, np.full(seconds, bg), (14, ti))
            for mode, spec, _ in modes for ti, (tube, bg) in enumerate(TUBES)}
    runs = run_history_jobs(pool, jobs, cfg['max_trials'])

    report.append('## Max value at background\n')
    report.append('Median (and 90th percentile) of Max / true rate over runs, after 1 '
                  f'hour and after {seconds // 3600} hours from a reset. "Startup" is the '
                  'fraction of runs whose Max already exceeds 2x the true rate within the '
                  'first 60 s.\n')
    report.append('| Mode | Tube | Gate | Max after 1 h | Max after '
                  f'{seconds // 3600} h | Startup > 2x |')
    report.append('|---|---|---|---|---|---|')
    for mode, spec, timed in modes:
        for tube, bg in TUBES:
            counts, est, conf = runs[(mode, tube)]
            est = est.astype(float)
            for gate in MAX_GATES:
                mask = confidence_gate(conf, timed, gate)
                m1 = running_max(est, mask, 3600) / bg
                m2 = running_max(est, mask, seconds) / bg
                s = running_max(est, mask, 60) / bg
                report.append(f'| {mode} | {tube} | {gate} '
                              f'| {np.median(m1):.2f} ({np.percentile(m1, 90):.2f}) '
                              f'| {np.median(m2):.2f} ({np.percentile(m2, 90):.2f}) '
                              f'| {100 * np.mean(s > 2):.0f}% |')
    report.append('')

    # Alerts at startup: true rate at half the threshold
    report.append('## False alerts at startup\n')
    report.append('Fraction of power-ons that raise an alert within the first 120 s, with '
                  'the true rate at 0.5x the alert threshold (hysteresis policy).\n')
    report.append('| Mode | Tube | Current gate | Uniform gate |')
    report.append('|---|---|---|---|')
    for mode, spec, timed in modes:
        for tube, bg in TUBES:
            counts, est, conf = runs[(mode, tube)]
            est = est[:, :120].astype(float)
            c = conf[:, :120].astype(float)
            cells = []
            for gate in MAX_GATES:
                mask = confidence_gate(c, timed, gate)
                active = alert_states(est, c, mask, 2 * bg, 'hysteresis')
                cells.append(f'{100 * np.mean(active.any(axis=1)):.1f}%')
            report.append(f'| {mode} | {tube} | {" | ".join(cells)} |')
    report.append('')

    # Peaks: walking past a source
    pre, post = cfg['passby_pre'], cfg['passby_post']
    passbys = [(tube, bg, factor, fwhm) for tube, bg in TUBES
               for factor in (10, 100) for fwhm in (10, 60)]
    jobs = {}
    profiles = {}
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        profiles[si] = passby_profile(bg, factor, fwhm, pre, pre + post)
        for mode, spec, _ in modes:
            jobs[(si, mode)] = (spec, profiles[si], (15, si))
    peaks = run_history_jobs(pool, jobs, cfg['passby_trials'])

    report.append('## Peaks\n')
    report.append('Walking past a source (as in report.md section 4): median Max / true '
                  'peak, with Max taken over the pass only (the first 600 s after a reset '
                  'are excluded).\n')
    report.append('| Tube | Peak | FWHM | Mode | Current gate | Uniform gate |')
    report.append('|---|---|---|---|---|---|')
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        peak = profiles[si].max()
        for mode, spec, timed in modes:
            counts, est, conf = peaks[(si, mode)]
            est = est[:, 600:].astype(float)
            c = conf[:, 600:]
            cells = []
            for gate in MAX_GATES:
                mask = confidence_gate(c, timed, gate)
                cells.append(f'{np.median(running_max(est, mask, est.shape[1])) / peak:.2f}')
            report.append(f'| {tube} | x{factor} | {fwhm} s | {mode} | {" | ".join(cells)} |')
    report.append('')


# Confidence gate for alerts and Max (python simulate.py --gate)

# Sensitivity (cpm per uSv/h), background 0.12 uSv/h, lowest alert threshold 0.5 uSv/h
GATE_TUBES = [('SI3BG', 3.267), ('HH614', 30.157), ('SBM-20', 106.105), ('LND 7317', 252.567)]
GATE_BACKGROUND = 0.12
GATE_THRESHOLD = 0.5
GATE_ADAPTIVE = [('none', None), ('±100%', 1.0), ('±75% (today)', 0.75), ('±50%', 0.5)]
GATE_FIXED = [('none (today)', None), ('full minute', 'full'), ('±75%', 0.75)]


def gate_mask(conf, rule):
    if rule is None:
        return np.ones(conf.shape, dtype=bool)
    if rule == 'full':
        mask = np.ones(conf.shape, dtype=bool)
        mask[:, :60] = False
        return mask
    return (conf > 0) & (conf < rule)


def stage_gate(pool, cfg, report):
    modes = [('fast', proposed(HOLD_G_THRESHOLD, 'n-1', N_FAST), GATE_ADAPTIVE),
             ('precision', proposed(HOLD_G_THRESHOLD, 'n-1', N_PRECISION), GATE_ADAPTIVE),
             ('1 minute', proposed(HOLD_G_THRESHOLD, 'n-1', fixed_window=60), GATE_FIXED)]
    seconds = cfg['gate_seconds']
    trials = cfg['gate_trials']
    jump = 3600

    jobs = {}
    for ti, (tube, sensitivity) in enumerate(GATE_TUBES):
        bg = GATE_BACKGROUND * sensitivity / 60
        threshold = GATE_THRESHOLD * sensitivity / 60
        for mode, spec, _ in modes:
            jobs[(tube, mode, 'background')] = (spec, np.full(seconds, bg), (16, ti))
            jobs[(tube, mode, 'power-on')] = (spec, np.full(1800, 2 * threshold), (17, ti))
            jobs[(tube, mode, 'jump')] = (spec, np.concatenate(
                [np.full(jump, bg), np.full(1800, 2 * threshold)]), (18, ti))
    runs = run_history_jobs(pool, jobs, trials)

    report.append('Per tube: background 0.12 uSv/h, alert threshold 0.5 uSv/h (the lowest '
                  'setting), alerts with hysteresis. **False alerts**: alert starts per day '
                  'at background, counting from power-on (startup included). **Power-on** / '
                  '**Jump**: median delay to the first alert when powering on in a field at '
                  '2x the threshold, or when the rate jumps from background to 2x the '
                  'threshold. **Max**: median Max / background after 24 h, and the fraction '
                  'of runs where Max never updated.\n')
    report.append('| Tube | Mode | Gate | False alerts/day | Power-on | Jump | Max | Max never updated |')
    report.append('|---|---|---|---|---|---|---|---|')
    for tube, sensitivity in GATE_TUBES:
        bg = GATE_BACKGROUND * sensitivity / 60
        threshold = GATE_THRESHOLD * sensitivity / 60
        for mode, spec, gates in modes:
            for name, rule in gates:
                _, est, conf = runs[(tube, mode, 'background')]
                est = est.astype(float)
                conf = conf.astype(float)
                mask = gate_mask(conf, rule)
                active = alert_states(est, conf, mask, threshold, 'hysteresis')
                prev = np.concatenate([np.zeros((trials, 1), dtype=bool), active[:, :-1]], axis=1)
                false_per_day = np.sum(active & ~prev) / (trials * seconds / 86400)
                mx = np.max(np.where(mask, est, 0.0), axis=1)

                delays = []
                for scenario, start in (('power-on', 0), ('jump', jump)):
                    _, e2, c2 = runs[(tube, mode, scenario)]
                    e2 = e2.astype(float)
                    c2 = c2.astype(float)
                    a2 = alert_states(e2, c2, gate_mask(c2, rule), threshold, 'hysteresis')
                    d = [np.flatnonzero(a2[i, start:])[0] + 1 if a2[i, start:].any() else math.inf
                         for i in range(trials)]
                    m = np.median(d)
                    delays.append('never' if math.isinf(m) else f'{m:.0f} s')

                report.append(f'| {tube} | {mode} | {name} | {false_per_day:.2f} | {delays[0]} '
                              f'| {delays[1]} | {np.median(mx) / bg:.2f} '
                              f'| {100 * np.mean(mx == 0):.0f}% |')
    report.append('')


# One-sided change detection (python simulate.py --onesided)

def onesided_specs():
    specs = {}
    for mode, n in (('fast', N_FAST), ('precision', N_PRECISION)):
        for direction, label in (('both', 'two-sided'), ('down', 'drops only')):
            specs[f'{mode}, {label}'] = proposed(HOLD_G_THRESHOLD, 'n-1', n,
                                                  change_direction=direction)
    return specs


def stage_onesided(pool, cfg, report):
    specs = onesided_specs()

    # Background with a realistic alert threshold
    seconds = cfg['gate_seconds']
    trials = cfg['gate_trials']
    jump = 3600
    jobs = {}
    for ti, (tube, sensitivity) in enumerate(GATE_TUBES):
        bg = GATE_BACKGROUND * sensitivity / 60
        threshold = GATE_THRESHOLD * sensitivity / 60
        for name, spec in specs.items():
            jobs[(tube, name, 'background')] = (spec, 'poisson', np.full(seconds, bg), (19, ti))
            jobs[(tube, name, 'jump')] = (spec, 'poisson', np.concatenate(
                [np.full(jump, bg), np.full(1800, 2 * threshold)]), (20, ti))
    runs = run_jobs(pool, jobs, trials)

    report.append('## Background\n')
    report.append('Background 0.12 uSv/h, alert threshold 0.5 uSv/h, hysteresis, ±75% gate. '
                  'Restarts: window restarts per hour (all are false at a steady rate). '
                  'Jump: median delay to the first alert when the rate jumps from background '
                  'to 2x the threshold. Max: median Max / background after 24 h.\n')
    report.append('| Tube | Mode | Restarts/h | False alerts/day | Jump | Max |')
    report.append('|---|---|---|---|---|---|')
    for tube, sensitivity in GATE_TUBES:
        bg = GATE_BACKGROUND * sensitivity / 60
        threshold = GATE_THRESHOLD * sensitivity / 60
        for name in specs:
            r = runs[(tube, name, 'background')]
            est = r['est'].astype(float)
            conf = r['conf'].astype(float)
            mask = gate_mask(conf, 0.75)
            active = alert_states(est, conf, mask, threshold, 'hysteresis')
            prev = np.concatenate([np.zeros((trials, 1), dtype=bool), active[:, :-1]], axis=1)
            false_per_day = np.sum(active & ~prev) / (trials * seconds / 86400)
            mx = np.median(np.max(np.where(mask, est, 0.0), axis=1)) / bg

            r2 = runs[(tube, name, 'jump')]
            e2 = r2['est'].astype(float)
            c2 = r2['conf'].astype(float)
            a2 = alert_states(e2, c2, gate_mask(c2, 0.75), threshold, 'hysteresis')
            d = [np.flatnonzero(a2[i, jump:])[0] + 1 if a2[i, jump:].any() else math.inf
                 for i in range(trials)]
            m = np.median(d)
            report.append(f'| {tube} | {name} | {r["restarts"].sum() / (trials * seconds / 3600):.3f} '
                          f'| {false_per_day:.2f} | {"never" if math.isinf(m) else f"{m:.0f} s"} '
                          f'| {mx:.2f} |')
    report.append('')

    # Steps
    pre = cfg['step_pre']
    post = cfg['step_post']
    scenarios = []
    for tube, bg in TUBES:
        for factor in (2, 10, 100):
            for direction in ('up', 'down'):
                before, after = (bg, factor * bg) if direction == 'up' else (factor * bg, bg)
                scenarios.append((tube, factor, direction, before, after))
    jobs = {}
    for si, (tube, factor, direction, before, after) in enumerate(scenarios):
        mean = np.concatenate([np.full(pre, before), np.full(post, after)])
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', mean, (21, si))
    results = run_jobs(pool, jobs, cfg['step_trials'])

    delays = np.arange(1, post + 1)
    shown = [d for d in STEP_DELAYS if d <= post]
    report.append('## Steps\n')
    report.append('Mean reading / new true rate after the step; settle = first delay after '
                  'which the mean stays within +/-20%.\n')
    for tube, bg in TUBES:
        report.append(f'### {tube} (background {bg} cps)\n')
        report.append('| Step | Mode | ' + ' | '.join(f'{d} s' for d in shown) + ' | Settle |')
        report.append('|---|---|' + '---|' * (len(shown) + 1))
        for si, (t, factor, direction, before, after) in enumerate(scenarios):
            if t != tube:
                continue
            for name in specs:
                ratio = np.mean(results[(si, name)]['est'][:, pre:], axis=0) / after
                cells = ' | '.join(f'{ratio[d - 1]:.2f}' for d in shown)
                st = settle_time(ratio, delays)
                st = f'{st} s' if st is not None else f'> {post} s'
                report.append(f'| x{factor} {direction} | {name} | {cells} | {st} |')
        report.append('')

    # Pass-by
    pre = cfg['passby_pre']
    post = cfg['passby_post']
    seconds = pre + post
    passbys = [(tube, bg, factor, fwhm) for tube, bg in TUBES
               for factor in (10, 100) for fwhm in (10, 60)]
    jobs = {}
    profiles = {}
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        profiles[si] = passby_profile(bg, factor, fwhm, pre, seconds)
        for name, spec in specs.items():
            jobs[(si, name)] = (spec, 'poisson', profiles[si], (22, si))
    results = run_jobs(pool, jobs, cfg['passby_trials'])

    report.append('## Walking past a source\n')
    report.append('Mean trajectory: peak reading / true peak, delay of the reading peak, '
                  'mean reading / true rate over the retreat.\n')
    report.append('| Tube | Peak | FWHM | Mode | Peak ratio | Peak delay | Retreat ratio |')
    report.append('|---|---|---|---|---|---|---|')
    for si, (tube, bg, factor, fwhm) in enumerate(passbys):
        true = profiles[si]
        tpk = int(np.argmax(true))
        for name in specs:
            mean = np.mean(results[(si, name)]['est'], axis=0)
            region = slice(max(tpk - 3 * fwhm, 0), min(tpk + 10 * fwhm, seconds))
            rpk = region.start + int(np.argmax(mean[region]))
            retreat = slice(tpk + fwhm, min(tpk + 10 * fwhm, seconds))
            report.append(f'| {tube} | x{factor} | {fwhm} s | {name} '
                          f'| {mean[rpk] / true[tpk]:.2f} | {rpk - tpk:+d} s '
                          f'| {np.mean(mean[retreat] / true[retreat]):.2f} |')
    report.append('')


# Main

CONFIGS = {
    'full': dict(calibrate_seconds=4 * 3600, calibrate_trials=40,
                 steady_seconds=3 * 3600, steady_trials=20, warmup=1200,
                 step_pre=1500, step_post=1200, step_trials=100,
                 passby_pre=1800, passby_post=900, passby_trials=100,
                 deadtime_seconds=300, deadtime_trials=40, dose_trials=40,
                 alert_pre=1200, alert_post=3 * 3600, alert_settle=600, alert_trials=20,
                 history_seconds=6 * 3600, history_trials=20, history_step_trials=100,
                 max_seconds=24 * 3600, max_trials=40,
                 gate_seconds=24 * 3600, gate_trials=20),
    'quick': dict(calibrate_seconds=3600, calibrate_trials=10,
                  steady_seconds=3600, steady_trials=10, warmup=1200,
                  step_pre=1200, step_post=600, step_trials=20,
                  passby_pre=1500, passby_post=700, passby_trials=20,
                  deadtime_seconds=60, deadtime_trials=10, dose_trials=10,
                  alert_pre=600, alert_post=1800, alert_settle=300, alert_trials=10,
                  history_seconds=3600, history_trials=10, history_step_trials=20,
                  max_seconds=2 * 3600, max_trials=10,
                  gate_seconds=3 * 3600, gate_trials=10),
}


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--quick', action='store_true', help='short smoke run')
    parser.add_argument('--hold', action='store_true',
                        help='only compare the sawtooth with holding between pulses '
                             '(writes results/hold.md)')
    parser.add_argument('--history', action='store_true',
                        help='only compare history bin methods (writes results/history.md)')
    parser.add_argument('--max', action='store_true',
                        help='only analyze the Max value and startup (writes results/max.md)')
    parser.add_argument('--onesided', action='store_true',
                        help='only compare two-sided and drops-only change detection '
                             '(writes results/onesided.md)')
    parser.add_argument('--gate', action='store_true',
                        help='only compare confidence gates for alerts and Max '
                             '(writes results/gate.md)')
    parser.add_argument('--processes', type=int, default=max((os.cpu_count() or 2) - 2, 1))
    args = parser.parse_args()
    cfg = CONFIGS['quick' if args.quick else 'full']

    os.makedirs(RESULTS, exist_ok=True)

    if args.onesided:
        start = time.time()
        report = ['# One-sided change detection\n',
                  'Generated by `simulate.py --onesided`. **two-sided**: change detection '
                  f'(G > {HOLD_G_THRESHOLD}) restarts the window on rises and drops (Rad Pro '
                  '3.2 as implemented); **drops only**: only when the recent rate is below '
                  'the rest of the window.\n']
        with Pool(args.processes) as pool:
            stage_onesided(pool, cfg, report)
        path = os.path.join(RESULTS, 'onesided.md')
        with open(path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(report) + '\n')
        print(f'Done in {time.time() - start:.0f} s: {path}')
        return

    if args.gate:
        start = time.time()
        report = ['# Confidence gate for alerts and Max\n',
                  'Generated by `simulate.py --gate`. Instantaneous rate with change detection '
                  f'(G > {HOLD_G_THRESHOLD}). The gate decides which readings may raise an '
                  'alert or set Max: none, a maximum confidence interval, or (1 minute) '
                  'only once a full minute has passed since power-on.\n']
        with Pool(args.processes) as pool:
            stage_gate(pool, cfg, report)
        path = os.path.join(RESULTS, 'gate.md')
        with open(path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(report) + '\n')
        print(f'Done in {time.time() - start:.0f} s: {path}')
        return

    if args.max:
        start = time.time()
        report = ['# Max value and startup\n',
                  'Generated by `simulate.py --max`. Instantaneous rate with change detection '
                  f'(G > {HOLD_G_THRESHOLD}). Gates for Max and for raising alerts: '
                  '**current** = adaptive modes need a confidence interval narrower than '
                  '+/-75%, 1 minute always passes; **uniform** = +/-75% in every mode.\n']
        with Pool(args.processes) as pool:
            stage_max(pool, cfg, report)
        path = os.path.join(RESULTS, 'max.md')
        with open(path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(report) + '\n')
        print(f'Done in {time.time() - start:.0f} s: {path}')
        return

    if args.history:
        start = time.time()
        report = ['# History bins\n',
                  'Generated by `simulate.py --history`. Instantaneous rate with change '
                  f'detection (G > {HOLD_G_THRESHOLD}). Methods: **Reading** = the reading '
                  'averaged over the bin (current firmware), **gated** = only seconds with '
                  'confidence below +/-75%; **Counts** = pulses / bin length; **Hybrid** = '
                  f'counts for bins with at least {HISTORY_PULSE_MIN} pulses, the averaged '
                  'reading otherwise. Bin lengths for the 201-bin color layout.\n']
        with Pool(args.processes) as pool:
            stage_history(pool, cfg, report)
        path = os.path.join(RESULTS, 'history.md')
        with open(path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(report) + '\n')
        print(f'Done in {time.time() - start:.0f} s: {path}')
        return

    if args.hold:
        start = time.time()
        report = ['# Sawtooth vs. hold between pulses\n',
                  'Generated by `simulate.py --hold`. All new variants use change detection '
                  f'(G > {HOLD_G_THRESHOLD}). Sawtooth: window ends now, (n - 1) / W. Hold: '
                  'window ends at the end of the most recent second with pulses, '
                  '(n - k) / W with k = 1, 2, or auto = 1 + W / (W + n).\n']
        with Pool(args.processes) as pool:
            stage_hold(pool, cfg, report)
        path = os.path.join(RESULTS, 'hold.md')
        with open(path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(report) + '\n')
        print(f'Done in {time.time() - start:.0f} s: {path}')
        return
    report = ['# Instantaneous rate simulation\n',
              f'Generated by `simulate.py`{" --quick" if args.quick else ""}. '
              f'Pulse targets: fast {N_FAST}, precision {N_PRECISION}. '
              f'Caps (variants): fast {CAP_FAST} s, precision {CAP_PRECISION} s. '
              'Proposed buffer: 64 x 1 s + 64 x 16 s.\n']

    start = time.time()
    with Pool(args.processes) as pool:
        def stage(name, fn, *a):
            t = time.time()
            r = fn(pool, cfg, *a, report) if a else fn(pool, cfg, report)
            print(f'{name}: {time.time() - t:.0f} s', flush=True)
            return r

        g_threshold = stage('calibrate', stage_calibrate)
        estimator = stage('steady', stage_steady, g_threshold)
        stage('steps', stage_steps, g_threshold, estimator)
        stage('passby', stage_passby, g_threshold, estimator)
        stage('deadtime', stage_deadtime, g_threshold, estimator)
        stage('alerts', stage_alerts, g_threshold, estimator)

    path = os.path.join(RESULTS, 'report.md')
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(report) + '\n')
    print(f'Done in {time.time() - start:.0f} s: {path}')


if __name__ == '__main__':
    main()
