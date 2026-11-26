# Tests

Analysis notebooks, capture scripts and simulations used while developing Rad Pro. These aren't unit tests.

| Folder | Contents |
|---|---|
| [battery-discharge](battery-discharge/) | Battery discharge captures and analysis (FS2011, FNIRSI GC-01) |
| [confidence-intervals](confidence-intervals/) | Derivation of the Poisson confidence interval approximation in `getConfidenceInterval` |
| [gmc800-pulse-stretcher](gmc800-pulse-stretcher/) | GQ GMC-800 pulse sound stretcher fix: recordings and analysis |
| [hh614-pulse-interval](hh614-pulse-interval/) | HH614 pulse interval capture and analysis |
| [instantaneous-averaging-3.1](instantaneous-averaging-3.1/) | Response curves of the Rad Pro 3.1 adaptive averaging (superseded by measurement-sim) |
| [measurement-sim](measurement-sim/) | Monte Carlo simulation of the instantaneous rate, average, dose and alert algorithms |
| [random-bits](random-bits/) | Random generator capture and test results (dieharder, ent) |
| [source-sensitivity](source-sensitivity/) | Tube sensitivity calculations for the radiation sources |
| [ui-mockup](ui-mockup/) | Rad Pro 2.0 UI mockup |

The capture scripts need the packages in [requirements.txt](requirements.txt).
