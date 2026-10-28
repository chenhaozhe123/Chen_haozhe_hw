# QuantStarter

A C++17 project for the Software Engineering and C++ for Quantitative Analysis
and Algorithmic Trading course. The library computes consecutive simple returns
and a trailing simple moving average (SMA). A console example uses a fixed price
series so that the output is easy to check by hand.

## Documentation

<!-- DOCS_LINK_START -->
[Online API documentation](https://chenhaozhe123.github.io/Chen_haozhe_hw/).
Doxygen documentation is deployed by successful default-branch CI runs.
Check [workflow status](https://github.com/chenhaozhe123/Chen_haozhe_hw/actions) to verify publication.
<!-- DOCS_LINK_END -->

## Build and run

Requirements: a C++17 compiler, CMake 3.20 or newer, and a native build tool
(Make, Ninja, or Visual Studio). The pinned doctest and Google Benchmark sources
are included, so normal configuration and builds do not download dependencies.
Doxygen is required only when generating documentation.

Run from the repository root (macOS/Linux):

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
./build/quant_demo
```

Expected demo output:

```text
Prices: 100.000000 102.000000 101.000000 105.000000 107.000000
Simple returns: 0.020000 -0.009804 0.039604 0.019048
SMA (window=3): 101.000000 102.666667 104.333333
```

For a multi-configuration generator such as Visual Studio, add `--config Release`
to build commands, `-C Release` to CTest, and run executables from their `Release/`
subdirectories. CI runs on Ubuntu by default. The manual workflow offers an
additional macOS check; the project has also passed local macOS verification.

## Unit tests

```sh
ctest --test-dir build --output-on-failure
# Show the individual doctest cases and assertions:
./build/tests/quant_tests
```

Tests use **doctest 2.4.11**. They check hand-calculated results, invalid windows,
empty input, non-positive/non-finite prices, return overflow, large averages,
subnormal prices, recovery after large observations expire, input preservation
and agreement with independently recomputed window means.
CTest registers one executable containing all doctest cases.

## Benchmark

```sh
./build/benchmarks/quant_benchmark --benchmark_min_time=0.05s \
  --benchmark_out=build/benchmark.json --benchmark_out_format=json
```

**Google Benchmark 1.9.1** compares the O(n) rolling SMA with an O(n * window)
baseline. Both validate inputs and allocate an output vector; deterministic input
generation happens outside the timed loop. Each iteration calculates a complete
series. Results include elapsed time per iteration and input items per second.
Hardware, compiler, power mode and system load affect the results; there is no
fixed speed threshold. CI prints timings and uploads JSON results as artifacts.
The stable queue uses additional bookkeeping, so its advantage depends on the
input size and window length; small windows may favor the naive implementation.

## Generate API documentation

Install Doxygen, for example `brew install doxygen` on macOS or
`sudo apt-get install doxygen` on Ubuntu, then run:

```sh
cmake -S . -B build -DQUANT_BUILD_DOCS=ON
cmake --build build --target docs
# Open docs/html/index.html in a browser.
```

Doxygen warnings fail the documentation build. Generated HTML is ignored by Git.
To configure only documentation, also pass `-DQUANT_BUILD_TESTS=OFF` and
`-DQUANT_BUILD_BENCHMARKS=OFF` in a separate build directory.

## Project layout

```text
.
├── CMakeLists.txt             # library, demo and optional targets
├── include/quant/analytics.hpp # public, Doxygen-documented API
├── src/                      # calculations and console example
├── tests/                    # doctest unit tests
├── benchmarks/               # Google Benchmark comparison
├── third_party/              # pinned dependencies and license information
├── docs/                     # documentation sources and publication guide
├── scripts/set_docs_url.py    # set the real README documentation URL
├── .github/workflows/ci.yml   # build, tests, benchmark and Pages deployment
├── Doxyfile                  # strict HTML documentation configuration
├── AGENTS.md                 # AI agent commands and coding rules
├── CLAUDE.md                 # imports AGENTS.md for Claude Code
└── USAGE.md                  # assignment guide and submission checklist
```

## Continuous integration and publication

Every push and pull request builds the program, runs the tests and benchmark on
Ubuntu, and generates Doxygen documentation on Ubuntu. A manual workflow trigger
can additionally enable macOS checks using the `run_macos` option. Keeping macOS
optional prevents hosted macOS runner queues from blocking ordinary publication.
On a push to the repository's default branch,
documentation is deployed to GitHub Pages **after all requested build checks and documentation
generation succeed**. Pull requests and other branches generate documentation
artifacts without deploying the production site.

Before the first default-branch push, enable **Settings → Pages → Build and
deployment → Source: GitHub Actions**. The repository must be public for the
assignment. See [publication and verification instructions](docs/PUBLISHING.md).
See the [verification record](docs/HOSTED_VALIDATION.md) for hosted evidence and
the latest workflow run for the current commit's status.

## Algorithm notes

- Simple return: `prices[i] / prices[i - 1] - 1`. Output is a fraction, not a
  percentage. Zero or one observation produces no returns.
- SMA: a queue made from two aggregate stacks maintains complete windows without
  subtracting large outgoing values from a rounded sum. Each value is pushed,
  transferred and popped at most once: O(n) total time and O(window) auxiliary
  storage. There are `n - window + 1` results. Inputs must be finite, strictly
  positive prices, and `1 <= window <= n`.
- Sums store a normalized fraction and a separate binary exponent, preventing
  sum overflow and premature division of subnormal prices. Results still incur
  ordinary floating-point rounding. This is an educational example, not a trading
  execution or accounting system.

## Rebuild after moving the project

CMake caches contain absolute paths. If the project directory is moved or renamed,
use a new build directory, including for downloaded or extracted dependencies:

```sh
cmake -S . -B build-clean -DCMAKE_BUILD_TYPE=Release
cmake --build build-clean --parallel 2
ctest --test-dir build-clean --output-on-failure
```

Alternatively, remove the old generated build directory and configure it again.
Refreshing only the top-level cache may leave stale dependency caches behind.

Third-party versions, source URLs, hashes and licenses are recorded in
[third_party/README.md](third_party/README.md).
