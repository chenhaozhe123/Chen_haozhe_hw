# Vendored dependencies

Builds use these local copies and do not fetch dependencies over the network.
Do not edit upstream code. Preserve license notices when redistributing.

## doctest 2.4.11

- Upstream: https://github.com/doctest/doctest/tree/v2.4.11
- Header: `doctest/doctest.h`
- Source: https://raw.githubusercontent.com/doctest/doctest/v2.4.11/doctest/doctest.h
- SHA-256: `44faa038e9c3f9728efbda143748d01124ea0a27f4bf78f35a15d8fab2e039fb`
- License: MIT, copied into `doctest/LICENSE.txt` and included in the header.

## Google Benchmark 1.9.1

- Upstream: https://github.com/google/benchmark/tree/v1.9.1
- Archive: `archives/benchmark-1.9.1.tar.gz`
- Source: https://codeload.github.com/google/benchmark/tar.gz/refs/tags/v1.9.1
- SHA-256: `32131c08ee31eeff2c8968d7e874f3cb648034377dfc32a4c377fa8796d84981`
- License: Apache-2.0, preserved in the archive's `LICENSE` file and in
  `benchmark-LICENSE.txt` alongside this file.
- CMake checks the archive hash before extraction. Google Benchmark's own tests
  are disabled; the project tests use doctest, so GoogleTest is not required.
