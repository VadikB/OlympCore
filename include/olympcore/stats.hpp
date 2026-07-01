#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace olympcore {

struct Summary {
    std::size_t count{};
    double min{};
    double max{};
    double mean{};
    double median{};
    double variance_sample{};
    double stdev_sample{};
};

struct RegressionResult {
    double slope{};
    double intercept{};
    double r_squared{};
};

struct Outlier {
    std::size_t index{};
    double value{};
    double z_score{};
};

Summary summarize(std::vector<double> values);
double mean(const std::vector<double>& values);
double median(std::vector<double> values);
double variance_sample(const std::vector<double>& values);
double variance_population(const std::vector<double>& values);
double stdev_sample(const std::vector<double>& values);
double stdev_population(const std::vector<double>& values);
double percentile_inc(std::vector<double> values, double percentile);
double percentile_exc(std::vector<double> values, double percentile);
double covariance_sample(const std::vector<double>& xs, const std::vector<double>& ys);
double pearson_correlation(const std::vector<double>& xs, const std::vector<double>& ys);
double skewness_sample(const std::vector<double>& values);
double kurtosis_excess_sample(const std::vector<double>& values);
std::vector<double> rank_average(const std::vector<double>& values);
std::vector<double> standardize(const std::vector<double>& values);

RegressionResult linear_regression(const std::vector<double>& xs, const std::vector<double>& ys);
double regression_forecast(const std::vector<double>& xs, const std::vector<double>& ys, double x);
double regression_steyx(const std::vector<double>& xs, const std::vector<double>& ys);

double normal_pdf(double x, double mean = 0.0, double stdev = 1.0);
double normal_cdf(double x, double mean = 0.0, double stdev = 1.0);
double normal_inv_cdf(double probability, double mean = 0.0, double stdev = 1.0);

std::vector<double> moving_average_simple(const std::vector<double>& values, std::size_t window);
std::vector<double> moving_average_cumulative(const std::vector<double>& values);
std::vector<double> moving_average_exponential(const std::vector<double>& values, double alpha);
std::vector<Outlier> z_score_outliers(const std::vector<double>& values, double threshold = 3.0);

std::vector<double> parse_numbers(const std::string& text);
std::string to_json(const Summary& summary);
std::string to_json(const RegressionResult& regression);
std::string to_json(const std::vector<double>& values);
std::string to_json(const std::vector<Outlier>& values);

} // namespace olympcore
