#include "olympcore/stats.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void expect_near(double actual, double expected, double tolerance, const std::string& name)
{
    if (std::fabs(actual - expected) > tolerance) {
        throw std::runtime_error(name + " expected " + std::to_string(expected) + ", got " + std::to_string(actual));
    }
}

void expect_true(bool condition, const std::string& name)
{
    if (!condition) {
        throw std::runtime_error(name);
    }
}

} // namespace

int main()
{
    const std::vector<double> values{1.0, 2.0, 3.0, 4.0, 5.0};
    const auto summary = olympcore::summarize(values);
    expect_true(summary.count == 5, "summary count");
    expect_near(summary.mean, 3.0, 1e-12, "mean");
    expect_near(summary.median, 3.0, 1e-12, "median");
    expect_near(summary.variance_sample, 2.5, 1e-12, "variance");
    expect_near(olympcore::percentile_inc(values, 0.25), 2.0, 1e-12, "percentile");

    const auto regression = olympcore::linear_regression({1, 2, 3, 4}, {3, 5, 7, 9});
    expect_near(regression.slope, 2.0, 1e-12, "regression slope");
    expect_near(regression.intercept, 1.0, 1e-12, "regression intercept");
    expect_near(regression.r_squared, 1.0, 1e-12, "regression r2");

    expect_near(olympcore::normal_pdf(0.0), 0.3989422804014327, 1e-12, "normal pdf");
    expect_near(olympcore::normal_cdf(0.0), 0.5, 1e-12, "normal cdf");
    expect_near(olympcore::normal_inv_cdf(0.975), 1.959963986120195, 1e-8, "normal inv cdf");

    const auto moving = olympcore::moving_average_simple({1, 2, 3, 4, 5}, 3);
    expect_true(moving.size() == 3, "moving average size");
    expect_near(moving[0], 2.0, 1e-12, "moving average first");
    expect_near(moving[2], 4.0, 1e-12, "moving average last");

    const auto outliers = olympcore::z_score_outliers({10, 11, 10, 9, 100}, 1.5);
    expect_true(outliers.size() == 1, "outlier count");
    expect_true(outliers[0].index == 4, "outlier index");

    std::cout << "all tests passed\n";
    return 0;
}
