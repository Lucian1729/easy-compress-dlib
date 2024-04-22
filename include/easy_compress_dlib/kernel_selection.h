#ifndef EASY_COMPRESS_DLIB_KERNEL_SELECTION_H
#define EASY_COMPRESS_DLIB_KERNEL_SELECTION_H

#include <string>
#include <vector>
#include <concepts>
#include <array>  // Include this header for std::array

namespace easy_compress_dlib {

// Custom data structure to store compression metrics
struct CompressionMetricEntry {
    std::string file_name;
    std::size_t original_size;
    std::size_t compressed_size;
    double bits_per_byte;
    bool corruption;
};

struct KernelMetrics {
    std::array<std::string, 11> file_types; // Uses std::array
    std::array<double, 11> bpbs;            // Uses std::array
    std::array<double, 11> compression_times; // Uses std::array
};

class CompressionMetrics {
private:
    std::vector<CompressionMetricEntry> entries;
    double average_time;

public:
    void load_from_csv(const std::string& csv_file);
    const std::vector<CompressionMetricEntry>& get_entries() const;
    double get_average_time() const;
};

// Function to calculate the performance measure and select the best kernel
std::size_t select_best_kernel(std::string_view file_type, double alpha, const std::vector<KernelMetrics>& kernels );
std::size_t select_best_kernel(std::string_view file_type, double alpha);

}

#endif // EASY_COMPRESS_DLIB_KERNEL_SELECTION_H
