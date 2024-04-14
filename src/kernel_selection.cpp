#include "easy_compress_dlib/kernel_selection.h"
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <concepts>
#include <chrono>
#include <fstream>
#include <sstream>
#include "lz77_buffer_kernel_1_wrapper.h"

namespace easy_compress_dlib {

// Custom data structure to store compression metrics
struct CompressionMetricEntry {
    std::string file_name;
    std::size_t original_size;
    std::size_t compressed_size;
    double bits_per_byte;
    bool corruption;
};

class CompressionMetrics {
private:
    std::vector<CompressionMetricEntry> entries;
    double average_time;

public:
    void load_from_csv(const std::string& csv_file) {
        std::ifstream file(csv_file);
        std::string line;

        // Skip the first line (header)
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            CompressionMetricEntry entry;
            std::string token;

            // Extract file_name
            std::getline(iss, token, ':');
            entry.file_name = token;

            // Extract original_size
            std::getline(iss, token, ' ');
            entry.original_size = std::stoull(token);

            // Extract compressed_size
            std::getline(iss, token, ' ');
            entry.compressed_size = std::stoull(token);

            // Extract bits_per_byte
            std::getline(iss, token, ' ');
            entry.bits_per_byte = std::stod(token);

            // Extract corruption
            std::getline(iss, token);
            entry.corruption = (token == "no") ? false : true;

            entries.push_back(entry);
        }

        // Extract average_time from the last line
        std::getline(file, line);
        std::istringstream time_iss(line);
        std::string time_str;
        std::getline(time_iss, time_str, ':');
        std::getline(time_iss, time_str);
        average_time = std::stod(time_str);
    }

    const std::vector<CompressionMetricEntry>& get_entries() const {
        return entries;
    }

    double get_average_time() const {
        return average_time;
    }
};

// Function to calculate the performance measure and select the best kernel
template <typename KernelContainer>
requires std::ranges::range<KernelContainer>
auto select_best_kernel(const KernelContainer& kernels, const CompressionMetrics& metrics, double alpha) {
    std::vector<double> performance_measures;

    for (const auto& kernel : kernels) {
        double compression_ratio = 0.0;
        double total_original_size = 0;
        double total_compressed_size = 0;

        for (const auto& entry : metrics.get_entries()) {
            total_original_size += entry.original_size;
            total_compressed_size += entry.compressed_size;
        }

        compression_ratio = total_original_size / total_compressed_size;

        double time_factor = 1.0 / metrics.get_average_time();
        double performance_measure = alpha * compression_ratio + (1 - alpha) * time_factor;

        performance_measures.push_back(performance_measure);
    }

    auto best_kernel_it = std::max_element(kernels.begin(), kernels.end(),
                                           [&](const auto& lhs, const auto& rhs) {
                                               return performance_measures[std::distance(kernels.begin(), &lhs)] <
                                                      performance_measures[std::distance(kernels.begin(), &rhs)];
                                           });

    return *best_kernel_it;
}

} // namespace easy_compress_dlib
