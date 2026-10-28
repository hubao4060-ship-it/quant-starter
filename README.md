# Quant Starter

A small **C++17** project for the software engineering course in quantitative
analysis and algorithmic trading. It computes consecutive simple returns and
simple moving averages. It contains no market connection or trading system.

<!-- repository-links:start -->
[Repository](https://github.com/hubao4060-ship-it/quant-starter) | [CI and benchmark logs](https://github.com/hubao4060-ship-it/quant-starter/actions/workflows/ci.yml) | [Published API documentation](https://hubao4060-ship-it.github.io/quant-starter/)

The documentation link becomes live after enabling Pages and a successful deployment.
<!-- repository-links:end -->

## Structure

```text
include/quant/   Public documented API
src/            Analytics implementation
app/            Executable example
tests/         Catch2 unit tests
benchmarks/     Google Benchmark cases
third_party/    Pinned upstream source archives and licenses
docs/          Doxygen main page and design notes
scripts/        Repository-link setup helper
.github/workflows/ci.yml  Build, tests, benchmarks, documentation and Pages
AGENTS.md       Coding-agent instructions
```

## Build and run

Requirements: a C++17 compiler, CMake 3.20 or newer, and optionally Doxygen
for documentation. Catch2 v3.7.1 and Google Benchmark v1.9.1 source archives
are included and verified by SHA-256 during CMake configuration. No dependency
download or system-wide library installation is needed to build the program.

On Linux, macOS, or a shell with a single-configuration generator:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
./build/quant_demo
./build/quant_benchmark --benchmark_min_time=0.1s --benchmark_repetitions=3 --benchmark_out=build/benchmark.json --benchmark_out_format=json
cmake --build build --target docs
```

On Windows with Visual Studio 2022 C++ tools:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
.\build\Release\quant_demo.exe
.\build\Release\quant_benchmark.exe --benchmark_min_time=0.1s --benchmark_repetitions=3 --benchmark_out=build/benchmark.json --benchmark_out_format=json
cmake --build build --config Release --target docs
```

If Doxygen was installed after CMake configuration, run the configure command
again. Open `build/docs/html/index.html` for the generated API documentation.
To build only the demo/library, configure with `-DBUILD_TESTING=OFF
-DQUANT_BUILD_BENCHMARKS=OFF` on the same command line.

The demonstration uses prices `[100, 110, 99, 108.9, 119.79]`. Expected returns
are `[0.1, -0.1, 0.1, 0.1]`, and the window-3 averages are
`[103, 105.9666667, 109.23]`. A return of `0.1` means 10 percent.

## Automation and first publication

Every push, pull request and manual workflow dispatch builds the project,
runs CTest, executes the benchmark and generates Doxygen HTML. Benchmark
results appear in **Actions > CI > build > Run benchmark** and are also saved
as a JSON artifact. Generated HTML is another downloadable artifact.

For a new **public** GitHub repository:

1. Set the real links with `python scripts/set_repository.py OWNER/REPO`.
2. Upload all project files, including `.github/workflows/ci.yml` and
   `third_party/`, and push your default branch.
3. In **Settings > Pages > Build and deployment > Source**, select
   **GitHub Actions**. Enable this before the publishing run. If the first
   run failed because Pages was not yet enabled, enable it and rerun the workflow.
4. Check that both `build` and `deploy` jobs pass, then open the documentation
   link written by the setup script. Allow a little time for the first deployment.

The deployment job runs only on the default branch, on push or manual dispatch.
Feature branches and pull requests still generate documentation but cannot
overwrite the published site. GitHub-hosted Linux runners install Doxygen
through Ubuntu's package manager; no personal access token is required.

**Remote validation completed:** [CI run #1](https://github.com/hubao4060-ship-it/quant-starter/actions/runs/40720263852)
passed the Release build, all 16 unit tests, benchmarks and documentation
publication. [The Doxygen site](https://hubao4060-ship-it.github.io/quant-starter/)
is publicly available. The benchmark console log and JSON artifact are in
the linked workflow run. Subsequent pushes automatically repeat the checks.

## API behavior

- `simple_returns`: all prices are finite and greater than zero. Empty or
  single-price input produces no returns. Each result is a fraction.
- `moving_average`: values are finite; `1 <= window <= values.size()`;
  exactly `values.size()-window+1` complete windows are returned.
- Invalid data throws `std::invalid_argument`; unrepresentable arithmetic
  throws `std::overflow_error`. No input vector is modified.
- Both functions take O(n) time. Benchmarks include validation and output
  allocation. Machine load and hardware affect timing; CI measures speed
  without enforcing an arbitrary performance threshold.

## По-русски

Учебный проект на C++17 вычисляет простую доходность соседних цен и простое
скользящее среднее. Исходный код, unit-тесты Catch2 и бенчмарки Google Benchmark
разнесены по отдельным каталогам. Сборка выполняется через CMake, тесты через
CTest. GitHub Actions автоматически собирает проект, запускает тесты и
бенчмарки, выводит измерения в журнал и формирует документацию Doxygen.
После успешной проверки основной ветки документация публикуется в GitHub Pages.

Команды сборки и запуска приведены выше. При использовании Visual Studio
добавьте `--config Release` при сборке и `-C Release` при запуске CTest;
исполняемые файлы находятся в `build/Release/`.

Перед первой публикацией необходимо указать настоящий адрес репозитория
через `scripts/set_repository.py` и выбрать GitHub Actions в настройках Pages.
Локальный архив сам по себе не подтверждает успешный запуск CI/CD.
