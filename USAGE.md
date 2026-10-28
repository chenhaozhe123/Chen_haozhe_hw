# Assignment Guide and Submission Checklist

## Assignment requirements

The original assignment is in Russian. It requires a complete C++ project and an
automated engineering workflow, without prescribing a specific business algorithm.
This project implements price-series analysis, matching the course topic through
consecutive simple returns and a simple moving average.

Deadline: **October 28, 2026, 23:59 Moscow time**, equivalent to
**October 29, 2026, 04:59 Beijing time**.

| Grading criterion | Points | Implementation / remaining verification |
| --- | --- | --- |
| Public repository, clear structure and README | 2 | Public repository created; push the prepared project and verify the uploaded files |
| CMake and automated builds | 4 | `CMakeLists.txt`, `.github/workflows/ci.yml` |
| C++ testing framework and CI tests | 4 | `tests/`, using doctest |
| Benchmark tool and CI timing logs | 4 | `benchmarks/`, using Google Benchmark with JSON output |
| Automatic Doxygen publication and README link | 4 | `Doxyfile`, Pages workflow and repository-specific README URL configured; verify deployment |
| Root-level AI agent instructions | 2 | `AGENTS.md`, `CLAUDE.md` |

**The assignment awards full points for criteria 2-5 only when they actually pass
in CI/CD; otherwise, only half the points are awarded.**
Completed local files and workflow configuration do not prove hosted checks have
passed. Do not claim full marks before verifying the remote workflow.

## Run the project

Open a terminal in this directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
./build/quant_demo
ctest --test-dir build --output-on-failure
./build/benchmarks/quant_benchmark --benchmark_min_time=0.05s
```

The testing framework and benchmark library are included in `third_party/`, so
normal builds require no dependency downloads. On another computer, first install
a C++17 compiler, CMake 3.20 or newer, and a native build tool.

Documentation generation additionally requires Doxygen:

```sh
cmake -S . -B build -DQUANT_BUILD_DOCS=ON
cmake --build build --target docs
```

Then open `docs/html/index.html` in a browser.

See [docs/LOCAL_VALIDATION.md](docs/LOCAL_VALIDATION.md) for the local verification
and measurement record.

## Explain the functions

1. **Simple return**: `next_price / previous_price - 1`. A rise from 100 to 110
   produces 0.1, or 10%; a fall from 110 to 99 produces -0.1. A series of n prices
   produces n-1 returns.
2. **Simple moving average**: for a window of 3, calculate the means of observations
   1-3, 2-4, 3-5, and so on. Only complete windows are emitted, producing
   `n - window + 1` results.
3. **Why window maintenance takes linear time**: two aggregate stacks form a
   queue. Each observation is pushed, transferred and popped at most once, giving
   O(n) total time with O(window) auxiliary storage. No outgoing price is
   subtracted from a rounded sum. The naive benchmark implementation recomputes
   every window, taking O(n * window). Queue bookkeeping may cost more for small
   windows, so the benchmark reports actual timings rather than assuming a win.
4. **Tests versus benchmarks**: tests verify correctness; benchmarks measure
   execution time without enforcing a fixed speed threshold. Timings vary across
   machines.
5. **Input constraints**: prices must be finite and strictly positive; the window
   must satisfy `1 <= window <= n`. Invalid inputs raise exceptions. Empty input
   is valid for simple returns but invalid for SMA. Normalized sums protect
   against sum overflow, premature underflow and subtraction cancellation;
   results still incur ordinary floating-point rounding.

## Before submission

Follow [docs/PUBLISHING.md](docs/PUBLISHING.md) to push the project to
[chenhaozhe123/Chen_haozhe_hw](https://github.com/chenhaozhe123/Chen_haozhe_hw),
enable Pages and verify the README's configured documentation URL. For the same
submitted commit, verify that:

- The Ubuntu build, test and benchmark job passes. Additional macOS CI is
  available through the manual workflow's `run_macos` option.
- Doxygen generation and GitHub Pages deployment pass.
- The benchmark step logs six rows with measured timings.
- The README's online documentation link and API pages load successfully.

Submit the repository URL. The assignment PDF is not required to build the project.
Do not commit `build/` or generated HTML to Git.
