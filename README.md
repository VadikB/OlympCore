# OlympCore

This is a portable open-source starting point for the legacy `Stats` C/C++ methods.

The remaining `Stats/` directory is kept as a reference-only archive for porting algorithms. Old Visual Studio project wrappers, Client/Server projects, distributed/MPI sources, precompiled-header scaffolding, resources, and build artifacts were removed. The remaining legacy method sources still reference the old Microsoft internal runtime headers (`TCTypes.h`, `TCArray.h`, `TCMath.h`, `TCReduce.h`, MKL wrappers), so they cannot be rebuilt as-is from this folder alone.

The new `olympcore` layer provides a small dependency-free C++17 API that can be built on macOS, Linux, and Windows.

## Included Methods

- Summary statistics: count, min, max, mean, median, sample variance, sample standard deviation
- Inclusive percentile
- Linear regression: slope, intercept, R squared
- Normal distribution: PDF, CDF, inverse CDF
- Simple moving average
- Z-score outlier detection

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

If CMake is not installed, the same code can be checked directly with a C++17 compiler:

```sh
c++ -std=c++17 -Iinclude src/stats.cpp tests/test_stats.cpp -o /tmp/test_stats
/tmp/test_stats

c++ -std=c++17 -Iinclude src/stats.cpp apps/olympcore_demo.cpp -o /tmp/olympcore_demo
/tmp/olympcore_demo summary "1,2,3,4,5"
```

## CLI Examples

```sh
/tmp/olympcore_demo summary "1,2,3,4,5"
/tmp/olympcore_demo regression "1,2,3,4" "3,5,7,9"
/tmp/olympcore_demo normal 0 0 1
/tmp/olympcore_demo moving-average 3 "1,2,3,4,5"
/tmp/olympcore_demo outliers 1.5 "10,11,10,9,100"
```

## Visual Demo

Open `web/index.html` in a browser. It is a static UI with no server dependency.
The demo contains interactive examples for the first ported methods and a grouped catalog of the legacy methods still waiting to be ported.
The web UI is organized as a method presentation system:

- `web/methods.js` describes methods, groups, statuses, sources, sample inputs, and input schemas.
- `web/panels.js` renders parameter panels from method schemas.
- `web/calculators.js` contains browser-side calculators for already ported methods.
- `web/app.js` connects navigation, language switching, calculation, charts, and JSON output.

For local HTTP testing:

```sh
python3 -m http.server 8765
```

Then open:

```text
http://localhost:8765/web/index.html
```

`localhost` is your own computer. This URL works only while the local server is running on the same machine. It will not work as a public GitHub URL.

## VS Code

Open the workspace:

```sh
code OlympCore.code-workspace
```

Useful tasks are available from `Terminal > Run Task`:

- `Build tests`
- `Run tests`
- `Build CLI demo`
- `Run CLI demo`
- `Serve web demo`
- `Open web demo URL`

The workspace uses `c++` directly, so CMake is optional for now.

## Porting Notes

The next practical migration step is to replace legacy `TCArray`-based entry points module by module with `std::span`/`std::vector` based equivalents, while keeping tests beside each ported method.
