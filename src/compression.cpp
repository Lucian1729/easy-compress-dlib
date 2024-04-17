#include "../include/easy_compress_dlib/compression.h"
#include "../include/easy_compress_dlib/kernel_selection.h"
#include "../include/easy_compress_dlib/compression_profile.h"

#include <dlib/compress_stream.h>
#include <dlib/entropy_encoder_model.h>
#include <dlib/entropy_decoder_model.h>
#include <dlib/lz77_buffer.h>
#include <dlib/lzp_buffer.h>
#include <sstream>
#include <filesystem>

#define KERNEL_COUNT 11

namespace easy_compress_dlib {

// Define a type alias for the CompressionProfile with all template arguments
using DefaultCompressionProfile = CompressionProfile<std::string, std::string, double, int>;

CompressionProfile<std::string, std::string, double, int> profile1("Profile1", "txt", 0.5);
    
// Instantiate another compression profile
CompressionProfile<std::string, std::string, double, int> profile2("Profile2", "jpg", 0.8);

// Instantiate a container for compression profiles
CompressionProfiles<CompressionProfile<std::string, std::string, double, int>> profiles;

// Add profiles to the container
profiles.add_profile(std::move(profile1));
profiles.add_profile(std::move(profile2));

int easy_compress(const std::string& input_filepath, const std::string& output_filepath, const std::string& file_type, double alpha) {
    // Error handling
    if (!std::filesystem::exists(input_filepath)) {
        throw std::invalid_argument("Input file does not exist: " + input_filepath);
    }
    if (alpha < 0 || alpha > 1) {
        throw std::invalid_argument("Alpha must be between 0 and 1");
    }
    if (!isValidFileType(file_type)) {
        throw std::invalid_argument("Invalid file type: " + file_type);
    }

    // Select kernel
    int kernel_index = select_kernel(file_type, alpha);

    std::string output_filename = output_filepath;
    if (output_filename.find('.') == std::string::npos) {
        // Add extension if not present
        output_filename += ".easy_compressed_";
    } else {
        // Replace existing extension
        output_filename = output_filename.substr(0, output_filename.find_last_of('.')) + ".easy_compressed_"; 
    }
    output_filename += std::to_string(kernel_index);

    // Compress using selected kernel
    map_kernel_and_compress(input_filepath, output_filename, kernel_index);

    return kernel_index;
}

int easy_compress_with_profile(const std::string& input_path, const std::string& output_path, const std::string& profile_name) {
    // Error handling
    if (!std::filesystem::exists(input_path)) {
        throw std::invalid_argument("Input file does not exist: " + input_path);
    }

    // Get the kernel from the profile
    int kernel_index = profiles.get_kernel_for_profile(profile_name);

    std::string output_filename = output_path;
    if (output_filename.find('.') == std::string::npos) {
        // Add extension if not present
        output_filename += ".easy_compressed_";
    } else {
        // Replace existing extension
        output_filename = output_filename.substr(0, output_filename.find_last_of('.')) + ".easy_compressed_"; 
    }
    output_filename += std::to_string(kernel_index);

    // Call map_kernel_and_compress with the kernel index
    map_kernel_and_compress(input_path, output_filename, kernel_index);

    return kernel_index;
}

void easy_decompress(const std::string& input_filepath, const std::string& output_filepath) {
    // Error handling
    if (!std::filesystem::exists(input_filepath)) {
        throw std::invalid_argument("Input file does not exist: " + input_filepath);
    }

    std::string filename = input_filepath;
    size_t extension_start = filename.find_last_of('.');
    if (extension_start == std::string::npos || 
        !filename.compare(extension_start, 15, ".easy_compressed_")) {
        throw std::runtime_error("Invalid compressed file: missing or incorrect extension");
    }

    int kernel_index;
    try {
        kernel_index = std::stoi(filename.substr(extension_start + 15));
    } catch (const std::exception& e) {
        throw std::runtime_error("Invalid compressed file: unable to parse kernel index"); 
    }

    // Validate kernel index
    if (kernel_index < 0 || kernel_index >= KERNEL_COUNT) {
        throw std::runtime_error("Invalid compressed file: invalid kernel index");
    }

    // Decompress using the specified kernel
    map_kernel_and_decompress(input_filepath, output_filepath, kernel_index);
}

void map_kernel_and_compress(const std::string& input_filepath, const std::string& output_filepath, int kernel_index) {
    // Read input file
    std::ifstream input_file(input_filepath, std::ios::binary);
    std::stringstream input_stream;
    input_stream << input_file.rdbuf();
    std::string input_data = input_stream.str();

    // Choose compression function based on kernel index
    std::string compressed_data;
    switch (kernel_index) {
        case 1:  compress_kernel_1a(input_data, compressed_data); break;
        case 2:  compress_kernel_1b(input_data, compressed_data); break;
        case 3:  compress_kernel_1c(input_data, compressed_data); break;
        case 4:  compress_kernel_1da(input_data, compressed_data); break;
        case 5:  compress_kernel_1db(input_data, compressed_data); break;
        case 6:  compress_kernel_1ea(input_data, compressed_data); break;
        case 7:  compress_kernel_1eb(input_data, compressed_data); break;
        case 8:  compress_kernel_1ec(input_data, compressed_data); break;
        case 9:  compress_kernel_2a(input_data, compressed_data); break;
        case 10: compress_kernel_3a(input_data, compressed_data); break;
        case 11: compress_kernel_3b(input_data, compressed_data); break;
        default: throw std::invalid_argument("Invalid kernel index");
    }

    // Write compressed data to output file
    std::ofstream output_file(output_filepath, std::ios::binary);
    output_file << compressed_data;
}

void map_kernel_and_decompress(const std::string& input_filepath, const std::string& output_filepath, int kernel_index) {
    // Read input file
    std::ifstream input_file(input_filepath, std::ios::binary);
    std::stringstream input_stream;
    input_stream << input_file.rdbuf();
    std::string compressed_data = input_stream.str();

    // Choose decompression function based on kernel index
    std::string decompressed_data;
    switch (kernel_index) {
        case 1:  decompress_kernel_1a(compressed_data, decompressed_data); break;
        case 2:  decompress_kernel_1b(compressed_data, decompressed_data); break;
        case 3:  decompress_kernel_1c(compressed_data, decompressed_data); break;
        case 4:  decompress_kernel_1da(compressed_data, decompressed_data); break;
        case 5:  decompress_kernel_1db(compressed_data, decompressed_data); break;
        case 6:  decompress_kernel_1ea(compressed_data, decompressed_data); break;
        case 7:  decompress_kernel_1eb(compressed_data, decompressed_data); break;
        case 8:  decompress_kernel_1ec(compressed_data, decompressed_data); break;
        case 9:  decompress_kernel_2a(compressed_data, decompressed_data); break;
        case 10: decompress_kernel_3a(compressed_data, decompressed_data); break;
        case 11: decompress_kernel_3b(compressed_data, decompressed_data); break;
        default: throw std::invalid_argument("Invalid kernel index");
    }

    // Write decompressed data to output file
    std::ofstream output_file(output_filepath, std::ios::binary);
    output_file << decompressed_data;
}

// Custom set implementation
template <typename T>
class MySet {
public:
    bool contains(const T& value) const {
        for (const T& existingValue : data_) {
            if (existingValue == value) {
                return true;
            }
        }
        return false;
    }

    void insert(const T& value) {
        if (!contains(value)) {
            data_.push_back(value);
        }
    }

private:
    std::vector<T> data_;
};

// Function to check if the provided file type is valid
bool isValidFileType(const std::string& fileType) {
    MySet<std::string> validFileTypes;
    validFileTypes.insert("text");
    validFileTypes.insert("play");
    validFileTypes.insert("html");
    validFileTypes.insert("Csrc");
    validFileTypes.insert("Excl");
    validFileTypes.insert("tech");
    validFileTypes.insert("poem");
    validFileTypes.insert("fax");
    validFileTypes.insert("SPRC");
    validFileTypes.insert("man");
    return validFileTypes.contains(fileType);
}

// Helper function to compress data using a compress_stream kernel
template <typename Kernel, typename InputIterator, typename OutputIterator>
void compress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin) {
    std::istringstream input_stream(std::string(input_begin, input_end));
    std::ostringstream output_stream;
    kernel.compress(input_stream, output_stream);
    std::copy(output_stream.str().begin(), output_stream.str().end(), output_begin);
}

// Helper function to decompress data using a compress_stream kernel
template <typename Kernel, typename InputIterator, typename OutputIterator>
void decompress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin) {
    std::istringstream input_stream(std::string(input_begin, input_end));
    std::ostringstream output_stream;
    kernel.decompress(input_stream, output_stream);
    std::copy(output_stream.str().begin(), output_stream.str().end(), output_begin);
}

// Compression and decompression functions using various kernels

// compress_stream_kernel_1 implementations
void compress_kernel_1a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1a kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1a kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1b(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1b kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1b(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1b kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1c(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1c kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1c(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1c kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1da(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1da kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1da(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1da kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1db(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1db kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1db(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1db kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1ea(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1ea kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1ea(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1ea kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1eb(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1eb kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1eb(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1eb kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_1ec(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1ec kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_1ec(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_1ec kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}


// compress_stream_kernel_2 implementation
void compress_kernel_2a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_2a kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_2a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_2a kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

// compress_stream_kernel_3 implementations
void compress_kernel_3a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_3a kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_3a(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_3a kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void compress_kernel_3b(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_3b kernel;
    compress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

void decompress_kernel_3b(const std::string& input, std::string& output) {
    dlib::compress_stream::kernel_3b kernel;
    decompress(kernel, input.begin(), input.end(), std::back_inserter(output));
}

} // namespace easy_compress_dlib