# Agent instructions

## Purpose and layout
QuantStarter is a small C++17 educational project for price-series analysis.
`include/quant/` contains documented public APIs; `src/` contains the library and
demo; `tests/` contains doctest unit tests; `benchmarks/` contains Google Benchmark
comparisons; `docs/` contains documentation sources; `.github/workflows/` contains
build, test, benchmark and GitHub Pages automation. Dependencies are vendored in
`third_party/`; do not edit their upstream source.

## Commands (run from the repository root)
- Configure: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
- Build: `cmake --build build --parallel 2`
- Demo: `./build/quant_demo`
- Tests: `ctest --test-dir build --output-on-failure`
- Benchmark: `./build/benchmarks/quant_benchmark --benchmark_min_time=0.05s`
- Enable docs: `cmake -S . -B build -DQUANT_BUILD_DOCS=ON`
- Generate docs: `cmake --build build --target docs` (requires Doxygen).

## Conventions and limits
- Use C++17, four spaces, snake_case for functions and variables, and `.clang-format`.
- Document public functions with Doxygen, including inputs, outputs and errors.
- Keep calculation code separate from console I/O; add tests for behavioral changes.
- Before finishing code changes, build the project and run tests. Run the benchmark
  for performance-related changes and regenerate docs after public API changes.
- Do not change CI/CD, publish a repository, push commits or deploy without a user request.
- Never add credentials, fabricated CI results or machine-independent speed claims.
- Preserve assignment source files if present. Keep generated build files and
  HTML out of version control.
