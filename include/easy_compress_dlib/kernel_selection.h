#ifndef EASY_COMPRESS_DLIB_KERNEL_SELECTION_H
#define EASY_COMPRESS_DLIB_KERNEL_SELECTION_H

#include <string>
#include <vector>
#include <concepts>

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
    void load_from_csv(const std::string& csv_file);
    const std::vector<CompressionMetricEntry>& get_entries() const;
    double get_average_time() const;
};

// Function to calculate the performance measure and select the best kernel
template <typename KernelContainer>
requires std::ranges::range<KernelContainer>
auto select_best_kernel(const KernelContainer& kernels, const CompressionMetrics& metrics, double alpha);

} // namespace easy_compress_dlib

#endif // EASY_COMPRESS_DLIB_KERNEL_SELECTION_H
