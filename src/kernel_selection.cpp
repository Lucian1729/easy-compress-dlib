#ifndef EASY_COMPRESS_KERNEL_SELECTION_CPP
#define EASY_COMPRESS_KERNEL_SELECTION_CPP

#include "../include/easy_compress_dlib/kernel_selection.h"
#include <bits/stdc++.h>
#include <concepts>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <iterator>   

#define KERNAL_COUNT 11
namespace easy_compress_dlib {
// Custom container for KernelMetrics
// Custom container for KernelMetrics
// template <std::size_t N>
// struct KernelMetrics {
//     std::array<std::string, N> file_types;
//     std::array<double, N> bpbs;
//     std::array<double, N> compression_times;
// };

// Kernel Metrics Initialization


const std::array<std::string, 11> kernel_names = {
    "text", "play", "html", "Csrc", "list", "Excl", "tech", "poem", "fax", "SPRC","man"
};

const std::vector<std::array<double, 11>> time_values = {
    {875, 875, 875, 875, 875, 875, 875, 875, 875, 875, 875}, 
    {844, 844, 844, 844, 844, 844, 844, 844, 844, 844, 844}, 
    {1031, 1031, 1031, 1031, 1031, 1031, 1031, 1031, 1031, 1031, 1031}, 
    {1812, 1812, 1812, 1812, 1812, 1812, 1812, 1812, 1812, 1812, 1812}, 
    {2296, 2296, 2296, 2296, 2296, 2296, 2296, 2296, 2296, 2296, 2296}, 
    {3062, 3062, 3062, 3062, 3062, 3062, 3062, 3062, 3062, 3062, 3062}, 
    {4875, 4875, 4875, 4875, 4875, 4875, 4875, 4875, 4875, 4875, 4875}, 
    {5484, 5484, 5484, 5484, 5484, 5484, 5484, 5484, 5484, 5484, 5484}, 
    {4641, 4641, 4641, 4641, 4641, 4641, 4641, 4641, 4641, 4641, 4641}, 
    {547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547}, 
    {734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734}
};

const std::vector<std::array<double, 11>> bpb_values = {
    {4.576, 4.82062, 5.27058, 5.08269, 4.78151, 3.42421, 4.65552, 4.53897, 1.16966, 5.3682, 5.03998},
    {3.48033, 3.48761, 3.79173, 3.39587, 3.50228, 2.66821, 3.5305, 3.39085, 0.843731, 3.82992, 3.97445},
    {2.72525, 2.8121, 2.79706, 2.43874, 2.73475, 1.84252, 2.75737, 2.82208, 0.845336, 3.17782, 3.33665},
    {2.39754, 2.71176, 2.51839, 2.27085, 2.66165, 1.51397, 2.09653, 2.45897, 0.874096, 2.98075, 3.28555},
    {2.51658, 2.88029, 2.57334, 2.35265, 2.7541, 1.6337, 2.17129, 2.61234, 0.880362, 3.02636, 3.36882},
    {2.14059, 2.39152, 2.2303, 2.00323, 2.33056, 1.21892, 1.9272, 2.26894, 0.796686, 2.63389, 2.89567},
    {2.1156, 2.39344, 2.21729, 1.98744, 2.33271, 1.23088, 1.87917, 2.26042, 0.785229, 2.59142, 2.89189},
    {2.12334, 2.41478, 2.22022, 1.94439, 2.33056, 1.26447, 1.88376, 2.30955, 0.783701, 2.55628, 2.90135},
    {2.9275, 3.17293, 2.71349, 2.52126, 2.96479, 0.564156, 2.72575, 3.25586, 0.775346, 2.77259, 3.51455},
    {6.2395, 6.63121, 4.57408, 4.16502, 4.60521, 3.03748, 5.81782, 7.22449, 1.81285, 4.82762, 6.01278},
    {5.86693, 6.48396, 4.52433, 4.13274, 4.64391, 3.16557, 5.46419, 6.93805, 1.77918, 4.85021, 5.97871}
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1a_metrics = {
    std::make_tuple("text", 4.576, 875),
    std::make_tuple("play", 4.82062, 875),
    std::make_tuple("html", 5.27058, 875),
    std::make_tuple("Csrc", 5.08269, 875),
    std::make_tuple("list", 4.78151, 875),
    std::make_tuple("Excl", 3.42421, 875),
    std::make_tuple("tech", 4.65552, 875), 
    std::make_tuple("poem", 4.53897, 875),
    std::make_tuple("fax",  1.16966, 875),
    std::make_tuple("SPRC", 5.3682,  875),
    std::make_tuple("man",  5.03998, 875)
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1b_metrics = {
    std::make_tuple("text", 3.48033, 844),
    std::make_tuple("play", 3.48761, 844),
    std::make_tuple("html", 3.79173, 844),
    std::make_tuple("Csrc", 3.39587, 844),
    std::make_tuple("list", 3.50228, 844),
    std::make_tuple("Excl", 2.66821, 844), 
    std::make_tuple("tech", 3.5305,  844),
    std::make_tuple("poem", 3.39085, 844),
    std::make_tuple("fax",  0.843731,844),
    std::make_tuple("SPRC", 3.82992, 844),
    std::make_tuple("man",  3.97445, 844) 
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1c_metrics ={
    std::make_tuple("text", 2.72525, 1031),
    std::make_tuple("play", 2.8121,  1031),
    std::make_tuple("html", 2.79706, 1031), 
    std::make_tuple("Csrc", 2.43874, 1031),
    std::make_tuple("list", 2.73475, 1031), 
    std::make_tuple("Excl", 1.84252, 1031),
    std::make_tuple("tech", 2.75737, 1031),
    std::make_tuple("poem", 2.82208, 1031),
    std::make_tuple("fax",  0.845336,1031),
    std::make_tuple("SPRC", 3.17782, 1031), 
    std::make_tuple("man",  3.33665, 1031)
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1da_metrics ={ 
    std::make_tuple("text", 2.39754, 1812), 
    std::make_tuple("play", 2.71176, 1812),
    std::make_tuple("html", 2.51839, 1812), 
    std::make_tuple("Csrc", 2.27085, 1812),
    std::make_tuple("list", 2.66165, 1812),
    std::make_tuple("Excl", 1.51397, 1812),
    std::make_tuple("tech", 2.09653, 1812), 
    std::make_tuple("poem", 2.45897, 1812),
    std::make_tuple("fax",  0.874096,1812),
    std::make_tuple("SPRC", 2.98075, 1812),
    std::make_tuple("man",  3.28555, 1812) 
}; 

const std::array<std::tuple<std::string, double, double>, 11> kernel_1db_metrics ={
    std::make_tuple("text", 2.51658, 2296),
    std::make_tuple("play", 2.88029, 2296), 
    std::make_tuple("html", 2.57334, 2296),
    std::make_tuple("Csrc", 2.35265, 2296),
    std::make_tuple("list", 2.7541,  2296), 
    std::make_tuple("Excl", 1.6337,  2296), 
    std::make_tuple("tech", 2.17129, 2296), 
    std::make_tuple("poem", 2.61234, 2296),
    std::make_tuple("fax",  0.880362,2296),
    std::make_tuple("SPRC", 3.02636, 2296),
    std::make_tuple("man",  3.36882, 2296)
}; 

const std::array<std::tuple<std::string, double, double>, 11> kernel_1ea_metrics ={
    std::make_tuple("text", 2.14059, 3062),
    std::make_tuple("play", 2.39152, 3062), 
    std::make_tuple("html", 2.2303,  3062), 
    std::make_tuple("Csrc", 2.00323, 3062), 
    std::make_tuple("list", 2.33056, 3062),
    std::make_tuple("Excl", 1.21892, 3062),
    std::make_tuple("tech", 1.9272,  3062), 
    std::make_tuple("poem", 2.26894, 3062), 
    std::make_tuple("fax",  0.796686,3062),
    std::make_tuple("SPRC", 2.63389, 3062),
    std::make_tuple("man",  2.89567, 3062)
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1eb_metrics ={
    std::make_tuple("text", 2.1156,  4875),
    std::make_tuple("play", 2.39344, 4875), 
    std::make_tuple("html", 2.21729, 4875),
    std::make_tuple("Csrc", 1.98744, 4875),
    std::make_tuple("list", 2.33271, 4875),
    std::make_tuple("Excl", 1.23088, 4875), 
    std::make_tuple("tech", 1.87917, 4875), 
    std::make_tuple("poem", 2.26042, 4875), 
    std::make_tuple("fax",  0.785229,4875),
    std::make_tuple("SPRC", 2.59142, 4875),
    std::make_tuple("man",  2.89189, 4875) 
};

const std::array<std::tuple<std::string, double, double>, 11> kernel_1ec_metrics ={
    std::make_tuple("text", 2.12334, 5484),
    std::make_tuple("play", 2.41478, 5484),
    std::make_tuple("html", 2.22022, 5484),
    std::make_tuple("Csrc", 1.94439, 5484), 
    std::make_tuple("list", 2.33056, 5484), 
    std::make_tuple("Excl", 1.26447, 5484),
    std::make_tuple("tech", 1.88376, 5484),
    std::make_tuple("poem", 2.30955, 5484), 
    std::make_tuple("fax",  0.783701,5484),
    std::make_tuple("SPRC", 2.55628, 5484), 
    std::make_tuple("man",  2.90135, 5484)
}; 

const std::array<std::tuple<std::string, double, double>, 11> kernel_2a_metrics ={
    std::make_tuple("text", 2.9275,  4641), 
    std::make_tuple("play", 3.17293, 4641),
    std::make_tuple("html", 2.71349, 4641), 
    std::make_tuple("Csrc", 2.52126, 4641),
    std::make_tuple("list", 2.96479, 4641), 
    std::make_tuple("Excl", 0.564156,4641),
    std::make_tuple("tech", 2.72575, 4641), 
    std::make_tuple("poem", 3.25586, 4641), 
    std::make_tuple("fax",  0.775346,4641),
    std::make_tuple("SPRC", 2.77259, 4641),
    std::make_tuple("man",  3.51455, 4641) 
}; 

const std::array<std::tuple<std::string, double, double>, 11> kernel_3a_metrics ={
    std::make_tuple("text", 6.2395,  547),
    std::make_tuple("play", 6.63121, 547),
    std::make_tuple("html", 4.57408, 547),
    std::make_tuple("Csrc", 4.16502, 547), 
    std::make_tuple("list", 4.60521, 547),
    std::make_tuple("Excl", 3.03748, 547),
    std::make_tuple("tech", 5.81782, 547),
    std::make_tuple("poem", 7.22449, 547),
    std::make_tuple("fax",  1.81285, 547),
    std::make_tuple("SPRC", 4.82762, 547),
    std::make_tuple("man",  6.01278, 547)
}; 

const std::array<std::tuple<std::string, double, double>, 11> kernel_3b_metrics ={
    std::make_tuple("text", 5.86693, 734),
    std::make_tuple("play", 6.48396, 734), 
    std::make_tuple("html", 4.52433, 734),
    std::make_tuple("Csrc", 4.13274, 734),
    std::make_tuple("list", 4.64391, 734), 
    std::make_tuple("Excl", 3.16557, 734),
    std::make_tuple("tech", 5.46419, 734),
    std::make_tuple("poem", 6.93805, 734), 
    std::make_tuple("fax",  1.77918, 734),
    std::make_tuple("SPRC", 4.85021, 734),
    std::make_tuple("man",  5.97871, 734)
}; 



// Function to get the BPB for a specific file type and kernel
double get_bpb_for_file_type(const KernelMetrics& kernel_metrics, std::string_view file_type) {
    auto it = std::find(kernel_metrics.file_types.begin(), kernel_metrics.file_types.end(), file_type);
    if (it != kernel_metrics.file_types.end()) {
        // Calculate the index based on the iterator position
        size_t index = std::distance(kernel_metrics.file_types.begin(), it);
        // Use the index to access the corresponding bits per byte (BPB) value
        return kernel_metrics.bpbs[index];
    }
    return -1.0; // Return -1.0 if file_type is not found
}

std::vector<KernelMetrics> kernels(11);

// Function to select the best kernel based on user-specified alpha and file type
std::size_t select_best_kernel(std::string_view file_type, double alpha,const std::vector<KernelMetrics>& kernels) {
    // Calculate average BPB and compression time for the given file type across all kernels
    double total_bpb = 0.0;
    double total_compression_time = 0.0;
    int count = 0;
    for (const auto& kernel : kernels) {
        double bpb = get_bpb_for_file_type(kernel, file_type);
        if (bpb != -1.0) {
            auto it = std::find(kernel.file_types.begin(), kernel.file_types.end(), file_type);
            double compression_time = kernel.compression_times[std::distance(kernel.file_types.begin(), it)];
            total_bpb += bpb;
            total_compression_time += compression_time;
            ++count;
        }
    }
    double avg_bpb = (count > 0) ? (total_bpb / count) : 0.0;
    double avg_compression_time = (count > 0) ? (total_compression_time / count) : 0.0;

    std::vector<double> performance_measures;

    // Calculate the performance measure for each kernel
    for (const auto& kernel : kernels) {
        double bpb = get_bpb_for_file_type(kernel, file_type);
        if (bpb == -1.0) {
            std::cerr << "Error: File type '" << file_type << "' not found in kernel metrics." << std::endl;
            continue;
        }

        auto it = std::find(kernel.file_types.begin(), kernel.file_types.end(), file_type);
        double compression_time = kernel.compression_times[std::distance(kernel.file_types.begin(), it)];
        double compression_ratio = 8.0 / avg_bpb;
        double time_factor = avg_compression_time / compression_time;
        double performance_measure = alpha * compression_ratio + (1 - alpha) * time_factor;

        performance_measures.push_back(performance_measure);
    }

    // Find the index of the kernel with the maximum performance measure
    auto best_kernel_index = std::distance(performance_measures.begin(), std::max_element(performance_measures.begin(), performance_measures.end()));
    return best_kernel_index+1;
}

// Overload that does not require a vector
std::size_t select_best_kernel(std::string_view file_type, double alpha) {
    for (int i = 0; i < 11; ++i) {
        kernels[i].file_types = kernel_names; // Assigning file types to each element
        kernels[i].bpbs = bpb_values[i];       // Assigning specific BPB values
        kernels[i].compression_times = time_values[i]; // Assigning specific compression times
    }
    return select_best_kernel(file_type, alpha, kernels);
}

// auto select_best_kernel_wrapper = [&kernels](std::string& file_type, double alpha) {
//     return select_best_kernel(kernels, file_type, alpha);
// };

// std::size_t best_kernel_index = select_best_kernel_wrapper(file_type, alpha);

} // namespace easy_compress_dlib

#endif