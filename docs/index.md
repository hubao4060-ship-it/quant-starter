# Quant Starter

A small C++17 library demonstrating a reproducible development workflow.

## Analytics

- `quant::simple_returns`: consecutive fractional returns, computed as
  `price[i] / price[i-1] - 1`. For `[100, 110, 99]` the result is `[0.1, -0.1]`.
- `quant::moving_average`: the arithmetic mean for each complete window.
  For `[1, 2, 3, 4, 5]` and window 3 the result is `[2, 3, 4]`.

Prices must be finite and strictly positive. Moving-average inputs may be
negative but must be finite. An invalid input throws `std::invalid_argument`;
unrepresentable calculations throw `std::overflow_error`. Both functions run
in O(n) time and return newly allocated vectors. Floating-point results are
approximate; this educational example does not implement trading or exact
decimal accounting.

## Development workflow

CMake builds the library, demo, Catch2 tests and Google Benchmark executable.
CTest runs the unit tests. Benchmarks measure the complete function, including
validation and allocation, for 1,000 and 100,000 observations. Input generation
is outside the timed loop. Results are hardware-dependent measurements, not
performance guarantees.

On every push or pull request, GitHub Actions builds in Release mode, runs
tests, prints benchmark results, saves benchmark JSON and generates this API
reference. Default-branch pushes also publish it to GitHub Pages after all
checks pass. Pull requests never deploy the public documentation.

## Build and view locally

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
cmake --build build --config Release --target docs
```

Install Doxygen before configuring for the `docs` target to appear. Open
`build/docs/html/index.html` in a browser. That output path stays the same
for single- and multi-configuration generators.

## Online documentation

Before upload, run `python scripts/set_repository.py OWNER/REPOSITORY` to put
your actual Pages link in the root README. Enable Settings > Pages > Source >
GitHub Actions in the public repository. The initial local package has no
published documentation URL; the Markdown file you are reading is the source
page that Doxygen turns into the documentation home page.

See the public API under **Namespaces > quant** for parameter and exception
contracts.
