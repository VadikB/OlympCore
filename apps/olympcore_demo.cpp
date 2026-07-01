#include "olympcore/stats.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void usage()
{
    std::cout
        << "olympcore_demo commands:\n"
        << "  summary \"1,2,3,4\"\n"
        << "  regression \"1,2,3\" \"2,4,6\"\n"
        << "  normal x mean stdev\n"
        << "  moving-average window \"1,2,3,4\"\n"
        << "  outliers threshold \"1,2,100,3\"\n";
}

double to_double(const char* text)
{
    char* end = nullptr;
    const double value = std::strtod(text, &end);
    if (end == text || *end != '\0') {
        throw std::invalid_argument(std::string("invalid number: ") + text);
    }
    return value;
}

std::size_t to_size(const char* text)
{
    char* end = nullptr;
    const auto value = std::strtoull(text, &end, 10);
    if (end == text || *end != '\0') {
        throw std::invalid_argument(std::string("invalid size: ") + text);
    }
    return static_cast<std::size_t>(value);
}

} // namespace

int main(int argc, char** argv)
{
    try {
        if (argc < 2) {
            usage();
            return 0;
        }

        const std::string command = argv[1];
        if (command == "summary" && argc == 3) {
            std::cout << olympcore::to_json(olympcore::summarize(olympcore::parse_numbers(argv[2]))) << "\n";
            return 0;
        }

        if (command == "regression" && argc == 4) {
            const auto xs = olympcore::parse_numbers(argv[2]);
            const auto ys = olympcore::parse_numbers(argv[3]);
            std::cout << olympcore::to_json(olympcore::linear_regression(xs, ys)) << "\n";
            return 0;
        }

        if (command == "normal" && argc == 5) {
            const double x = to_double(argv[2]);
            const double mean = to_double(argv[3]);
            const double stdev = to_double(argv[4]);
            std::cout << "{\"pdf\":" << olympcore::normal_pdf(x, mean, stdev)
                      << ",\"cdf\":" << olympcore::normal_cdf(x, mean, stdev) << "}\n";
            return 0;
        }

        if (command == "moving-average" && argc == 4) {
            const auto window = to_size(argv[2]);
            const auto values = olympcore::parse_numbers(argv[3]);
            std::cout << olympcore::to_json(olympcore::moving_average_simple(values, window)) << "\n";
            return 0;
        }

        if (command == "outliers" && argc == 4) {
            const auto threshold = to_double(argv[2]);
            const auto values = olympcore::parse_numbers(argv[3]);
            std::cout << olympcore::to_json(olympcore::z_score_outliers(values, threshold)) << "\n";
            return 0;
        }

        usage();
        return 2;
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }
}
