# Local verification record

Verified on **2026-10-28 (Asia/Shanghai)** on macOS arm64, Apple Clang 21.0.0,
CMake 3.31.5, Release configuration, C++17, Doxygen 1.18.0.

- CMake configuration and compilation: passed.
- Demo: output matches the README example.
- CTest: 1 registered executable passed.
- doctest: **17 test cases, 1455 assertions passed**, zero failures or skips.
  Regression cases cover expiring large prices, subnormal values, extreme mixed
  magnitudes and maximum finite averages across stack transfers.
- AddressSanitizer and UndefinedBehaviorSanitizer: all 17 test cases passed.
- Independent 1000-digit decimal oracle: 23,488 window means over 256 cases
  covering binary exponents -1074 through 1023 agreed within eight ULP.
- Google Benchmark: all **6 cases completed**; original JSON is in the local
  ignored `build/benchmark.json` file.
- Doxygen: HTML regenerated successfully without warnings from both the main
  build and the standalone documentation build.
- The previous browser review verified the homepage and function reference
  navigation. Current generated pages include the updated API complexity notes.
- README-link helper: checked normal repository URLs, owner-level Pages URLs and
  invalid repository input in a temporary copy, preserving the local README.

Example CPU timings from this run (one iteration calculates the full input):

| Input prices | Window | Rolling SMA | Naive SMA |
| --- | --- | --- | --- |
| 1,024 | 20 | 19,457 ns | 4,471 ns |
| 16,384 | 20 | 315,889 ns | 73,036 ns |
| 65,536 | 100 | 1,299,667 ns | 2,058,676 ns |

These are measurements from one local run, not performance guarantees. Google
Benchmark emitted macOS CPU-frequency and affinity metadata warnings; it explicitly
reported that the missing frequency metadata does not affect measurements.
The stable queue's bookkeeping is slower than the baseline for the smaller
windows in this run; it becomes faster for the 100-observation window.

The old generated build directories contained paths from before the project was
renamed. They were preserved outside the source tree, and both local build
directories were regenerated using the current path. README build commands now
work from the project root.

The public repository `chenhaozhe123/Chen_haozhe_hw` exists, and README now contains
its corresponding Pages URL. **Not yet verified:** uploading this project,
hosted Ubuntu/macOS GitHub Actions runs and GitHub Pages deployment. Follow
[PUBLISHING.md](PUBLISHING.md) and verify these before submitting the assignment.
