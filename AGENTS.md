# Agent instructions

## Purpose and layout

Quant Starter is an educational C++17 library for simple returns and moving
averages. Public API: include/quant/analytics.hpp. Implementation: src/.
Demo: app/. Catch2 tests: tests/. Google Benchmark: benchmarks/.
Doxygen configuration: Doxyfile; overview: docs/index.md.
CI and Pages publication: .github/workflows/ci.yml.
Pinned third-party source archives and license notices: third_party/.

## Commands

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
./build/quant_demo
./build/quant_benchmark --benchmark_min_time=0.1s --benchmark_repetitions=3
cmake --build build --config Release --target docs
```

For Visual Studio, executables are build/Release/quant_demo.exe and
build/Release/quant_benchmark.exe. Doxygen must be installed before configure.

## Style and restrictions

- Use C++17, four spaces, snake_case for functions and variables, PascalCase
  for new types, and small functions with explicit error handling.
- Document public interfaces, parameters, results and exceptions with Doxygen.
- Keep domain code independent of benchmark and test frameworks.
- Before finishing a code change, build and run CTest. For a benchmark or
  documentation change, also run the corresponding target. Report actual
  results and distinguish local checks from remote CI.
- Write a regression test for a changed calculation or error-handling rule.
- Do not change CI/CD, deployment settings, pinned dependencies or their
  checksums unless the user explicitly requests that area of work.
- Do not add credentials, trading accounts, private datasets or build outputs
  to version control. Do not edit or unpack-and-modify third-party archives.
- Do not remove failing tests to make the build pass. Do not claim that
  Pages is published until the actual URL has been checked.
