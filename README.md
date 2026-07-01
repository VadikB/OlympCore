# OlympCore

This is a portable open-source starting point for the legacy `Stats` C/C++ methods.

The original `Stats/` directory is kept intact. It still references the old Microsoft internal runtime headers (`TCTypes.h`, `TCArray.h`, `TCMath.h`, `TCReduce.h`, MKL wrappers, MPI pieces), so it cannot be rebuilt as-is from this folder alone.

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

## Porting Notes

The next practical migration step is to replace legacy `TCArray`-based entry points module by module with `std::span`/`std::vector` based equivalents, while keeping tests beside each ported method.
