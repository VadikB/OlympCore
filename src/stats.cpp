#include "olympcore/stats.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace olympcore {
namespace {

constexpr double kPi = 3.141592653589793238462643383279502884;

void require_non_empty(const std::vector<double>& values, const char* name)
{
    if (values.empty()) {
        throw std::invalid_argument(std::string(name) + " must not be empty");
    }
}

void require_finite(double value, const char* name)
{
    if (!std::isfinite(value)) {
        throw std::invalid_argument(std::string(name) + " must be finite");
    }
}

std::string json_number(double value)
{
    if (!std::isfinite(value)) {
        return "null";
    }

    std::ostringstream out;
    out << std::setprecision(15) << value;
    return out.str();
}

} // namespace

double mean(const std::vector<double>& values)
{
    require_non_empty(values, "values");
    return std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(values.size());
}

double median(std::vector<double> values)
{
    require_non_empty(values, "values");
    std::sort(values.begin(), values.end());
    const auto n = values.size();
    if (n % 2 == 1) {
        return values[n / 2];
    }
    return (values[n / 2 - 1] + values[n / 2]) / 2.0;
}

double variance_sample(const std::vector<double>& values)
{
    if (values.size() < 2) {
        throw std::invalid_argument("values must contain at least two numbers");
    }

    const double avg = mean(values);
    double sum = 0.0;
    for (double value : values) {
        const double delta = value - avg;
        sum += delta * delta;
    }
    return sum / static_cast<double>(values.size() - 1);
}

double stdev_sample(const std::vector<double>& values)
{
    return std::sqrt(variance_sample(values));
}

double percentile_inc(std::vector<double> values, double percentile)
{
    require_non_empty(values, "values");
    require_finite(percentile, "percentile");
    if (percentile < 0.0 || percentile > 1.0) {
        throw std::invalid_argument("percentile must be in [0, 1]");
    }

    std::sort(values.begin(), values.end());
    if (values.size() == 1) {
        return values.front();
    }

    const double rank = percentile * static_cast<double>(values.size() - 1);
    const auto lower = static_cast<std::size_t>(std::floor(rank));
    const auto upper = static_cast<std::size_t>(std::ceil(rank));
    const double fraction = rank - static_cast<double>(lower);
    return values[lower] + (values[upper] - values[lower]) * fraction;
}

Summary summarize(std::vector<double> values)
{
    require_non_empty(values, "values");
    const auto [min_it, max_it] = std::minmax_element(values.begin(), values.end());
    Summary summary;
    summary.count = values.size();
    summary.min = *min_it;
    summary.max = *max_it;
    summary.mean = mean(values);
    summary.median = median(values);
    if (values.size() > 1) {
        summary.variance_sample = variance_sample(values);
        summary.stdev_sample = std::sqrt(summary.variance_sample);
    }
    return summary;
}

RegressionResult linear_regression(const std::vector<double>& xs, const std::vector<double>& ys)
{
    require_non_empty(xs, "xs");
    if (xs.size() != ys.size()) {
        throw std::invalid_argument("xs and ys must have the same length");
    }
    if (xs.size() < 2) {
        throw std::invalid_argument("regression requires at least two points");
    }

    const double x_mean = mean(xs);
    const double y_mean = mean(ys);
    double ss_xy = 0.0;
    double ss_xx = 0.0;
    double ss_yy = 0.0;
    for (std::size_t i = 0; i < xs.size(); ++i) {
        const double dx = xs[i] - x_mean;
        const double dy = ys[i] - y_mean;
        ss_xy += dx * dy;
        ss_xx += dx * dx;
        ss_yy += dy * dy;
    }
    if (ss_xx == 0.0) {
        throw std::invalid_argument("xs must not all be equal");
    }

    RegressionResult result;
    result.slope = ss_xy / ss_xx;
    result.intercept = y_mean - result.slope * x_mean;
    result.r_squared = ss_yy == 0.0 ? 1.0 : (ss_xy * ss_xy) / (ss_xx * ss_yy);
    return result;
}

double normal_pdf(double x, double mean_value, double stdev)
{
    require_finite(x, "x");
    require_finite(mean_value, "mean");
    require_finite(stdev, "stdev");
    if (stdev <= 0.0) {
        throw std::invalid_argument("stdev must be positive");
    }

    const double z = (x - mean_value) / stdev;
    return std::exp(-0.5 * z * z) / (stdev * std::sqrt(2.0 * kPi));
}

double normal_cdf(double x, double mean_value, double stdev)
{
    require_finite(x, "x");
    require_finite(mean_value, "mean");
    require_finite(stdev, "stdev");
    if (stdev <= 0.0) {
        throw std::invalid_argument("stdev must be positive");
    }

    return 0.5 * std::erfc(-(x - mean_value) / (stdev * std::sqrt(2.0)));
}

double normal_inv_cdf(double probability, double mean_value, double stdev)
{
    require_finite(probability, "probability");
    require_finite(mean_value, "mean");
    require_finite(stdev, "stdev");
    if (stdev <= 0.0) {
        throw std::invalid_argument("stdev must be positive");
    }
    if (probability <= 0.0 || probability >= 1.0) {
        throw std::invalid_argument("probability must be in (0, 1)");
    }

    const double a[] = {
        -3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
        1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00};
    const double b[] = {
        -5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
        6.680131188771972e+01, -1.328068155288572e+01};
    const double c[] = {
        -7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
        -2.549732539343734e+00, 4.374664141464968e+00, 2.938163982698783e+00};
    const double d[] = {
        7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
        3.754408661907416e+00};

    constexpr double p_low = 0.02425;
    constexpr double p_high = 1.0 - p_low;
    double x = 0.0;

    if (probability < p_low) {
        const double q = std::sqrt(-2.0 * std::log(probability));
        x = (((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
            ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1.0);
    } else if (probability <= p_high) {
        const double q = probability - 0.5;
        const double r = q * q;
        x = (((((a[0] * r + a[1]) * r + a[2]) * r + a[3]) * r + a[4]) * r + a[5]) * q /
            (((((b[0] * r + b[1]) * r + b[2]) * r + b[3]) * r + b[4]) * r + 1.0);
    } else {
        const double q = std::sqrt(-2.0 * std::log(1.0 - probability));
        x = -(((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
            ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1.0);
    }

    return mean_value + stdev * x;
}

std::vector<double> moving_average_simple(const std::vector<double>& values, std::size_t window)
{
    require_non_empty(values, "values");
    if (window == 0 || window > values.size()) {
        throw std::invalid_argument("window must be in [1, values.size()]");
    }

    std::vector<double> result;
    result.reserve(values.size() - window + 1);
    double rolling = std::accumulate(values.begin(), values.begin() + static_cast<std::ptrdiff_t>(window), 0.0);
    result.push_back(rolling / static_cast<double>(window));
    for (std::size_t i = window; i < values.size(); ++i) {
        rolling += values[i] - values[i - window];
        result.push_back(rolling / static_cast<double>(window));
    }
    return result;
}

std::vector<Outlier> z_score_outliers(const std::vector<double>& values, double threshold)
{
    require_non_empty(values, "values");
    require_finite(threshold, "threshold");
    if (threshold <= 0.0) {
        throw std::invalid_argument("threshold must be positive");
    }

    const double avg = mean(values);
    const double sd = values.size() > 1 ? stdev_sample(values) : 0.0;
    std::vector<Outlier> result;
    if (sd == 0.0) {
        return result;
    }

    for (std::size_t i = 0; i < values.size(); ++i) {
        const double z = (values[i] - avg) / sd;
        if (std::fabs(z) >= threshold) {
            result.push_back({i, values[i], z});
        }
    }
    return result;
}

std::vector<double> parse_numbers(const std::string& text)
{
    std::string normalized = text;
    for (char& ch : normalized) {
        if (ch == ',' || ch == ';' || ch == '\n' || ch == '\t') {
            ch = ' ';
        }
    }

    std::istringstream in(normalized);
    std::vector<double> values;
    double value = 0.0;
    while (in >> value) {
        values.push_back(value);
    }
    return values;
}

std::string to_json(const Summary& summary)
{
    std::ostringstream out;
    out << "{\"count\":" << summary.count
        << ",\"min\":" << json_number(summary.min)
        << ",\"max\":" << json_number(summary.max)
        << ",\"mean\":" << json_number(summary.mean)
        << ",\"median\":" << json_number(summary.median)
        << ",\"variance_sample\":" << json_number(summary.variance_sample)
        << ",\"stdev_sample\":" << json_number(summary.stdev_sample) << "}";
    return out.str();
}

std::string to_json(const RegressionResult& regression)
{
    std::ostringstream out;
    out << "{\"slope\":" << json_number(regression.slope)
        << ",\"intercept\":" << json_number(regression.intercept)
        << ",\"r_squared\":" << json_number(regression.r_squared) << "}";
    return out.str();
}

std::string to_json(const std::vector<double>& values)
{
    std::ostringstream out;
    out << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << json_number(values[i]);
    }
    out << "]";
    return out.str();
}

std::string to_json(const std::vector<Outlier>& values)
{
    std::ostringstream out;
    out << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << "{\"index\":" << values[i].index
            << ",\"value\":" << json_number(values[i].value)
            << ",\"z_score\":" << json_number(values[i].z_score) << "}";
    }
    out << "]";
    return out.str();
}

} // namespace olympcore
