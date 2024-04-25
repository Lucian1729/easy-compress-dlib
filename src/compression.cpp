#ifndef EASY_COMPRESS_DLIB_COMPRESSION_CPP
#define EASY_COMPRESS_DLIB_COMPRESSION_CPP

#include "../include/easy_compress_dlib/compression.h"
#include <dlib/compress_stream.h>
#include <dlib/entropy_encoder_model.h>
#include <dlib/entropy_decoder_model.h>
#include <dlib/lz77_buffer.h>
#include <dlib/lzp_buffer.h>
#include <sstream>
#include <filesystem>
#include "../include/easy_compress_dlib/custom_vector.h"

#define KERNEL_COUNT 11

namespace easy_compress_dlib {

template <typename Kernel, typename InputIterator, typename OutputIterator>
void compress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin);

template <typename Kernel, typename InputIterator, typename OutputIterator>
void decompress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin);


// hash function :

template <typename... Args>
std::uint32_t checksum(Args... args) {
    std::uint32_t result = 0;
    // Fold expression to accumulate hash values
    ((result ^= std::hash<std::decay_t<Args>>{}(args)), ...);
    return result;
}

// parameter packs
template <typename... Strings>
void hashFileContents(const std::string& inputFilePath, const std::string& outputFilePath, Strings... extraStrings) {
    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    std::string line;
    while (getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            auto wordHash = checksum(word, extraStrings...);  // Compute the hash for each word
            outputFile << wordHash << " ";  // Write the hash to the output file
        }
    }

    inputFile.close();
    outputFile.close();
}

template <typename... Strings>
void dehashFileWords(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& originalTextFilePath, Strings... extraStrings) {
    std::ifstream inputFile(inputFilePath);
    std::ifstream originalFile(originalTextFilePath);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile.is_open() || !outputFile.is_open() || !originalFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    std::string line, originalLine;
    while (getline(inputFile, line) && getline(originalFile, originalLine)) {
        std::istringstream iss(line), issOriginal(originalLine);
        std::string hashString, originalWord;
        while (iss >> hashString && issOriginal >> originalWord) {
            auto hash = std::stoul(hashString);
            auto originalHash = checksum(originalWord, extraStrings...);
            if (hash == originalHash) {
                outputFile << originalWord << " ";
            }
        }
        outputFile << std::endl;
    }

    inputFile.close();
    originalFile.close();
    outputFile.close();
}






// Benchmark function

template <typename Kernel, typename InputIterator, typename OutputIterator>
auto benchmark_kernel(Kernel& kernel, InputIterator input_begin, InputIterator input_end,
                      OutputIterator output_begin, int iterations = 100) {
    using namespace std::chrono;
    
    static_assert(std::is_invocable_r_v<void, decltype(&Kernel::compress), Kernel&, std::istream&, std::ostream&>,
                  "Kernel must have a compress(istream&, ostream&) member function");
    static_assert(std::is_invocable_r_v<void, decltype(&Kernel::decompress), Kernel&, std::istream&, std::ostream&>,
                  "Kernel must have a decompress(istream&, ostream&) member function");

    auto measure_compress_time = [&](auto& k, auto i_begin, auto i_end, auto o_begin) {
        auto start = high_resolution_clock::now();
        compress(k, i_begin, i_end, o_begin);
        auto end = high_resolution_clock::now();
        return duration_cast<microseconds>(end - start).count();
    };

    // Lambda template to measure decompression time (modified)
    auto measure_decompress_time = [&](auto& kernel, auto compressed_begin, auto compressed_end, auto output_begin) {
        auto start = high_resolution_clock::now();
        decompress(kernel, compressed_begin, compressed_end, output_begin);
        auto end = high_resolution_clock::now();
        return duration_cast<microseconds>(end - start).count();
    };

    // Perform benchmarks and calculate average times
    double compress_time_total = 0;
    double decompress_time_total = 0;
    std::vector<char> compressed_data;  // Store compressed data

    for (int i = 0; i < iterations; ++i) {
        compressed_data.clear(); // Clear for each iteration
        compress_time_total += measure_compress_time(kernel, input_begin, input_end, std::back_inserter(compressed_data));
        decompress_time_total += measure_decompress_time(kernel, compressed_data.begin(), compressed_data.end(), output_begin); 
    }

    return std::make_pair(compress_time_total / iterations, decompress_time_total / iterations);
}


template <typename Kernel>
void run_benchmark(const std::string& inputPath, Kernel& kernel,int ind) {
    std::ifstream ifs(inputPath);
    if (!ifs) {
        std::cerr << "Failed to open input file: " << inputPath << std::endl;
        return;
    }

    // Read input file into a string
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string inputData = buffer.str();

    // Prepare containers for compressed and decompressed data
    std::string compressedData;
    std::string decompressedData;

    // Run benchmark
    auto results = benchmark_kernel(kernel, inputData.begin(), inputData.end(), std::back_inserter(compressedData), 100);
    std::cout << "Average Compression Time for the kernel " << ind << ": " << results.first << " microseconds\n";
    std::cout << "Average Decompression Time for the kernel " << ind << ": " << results.second << " microseconds\n";
}

void select_and_run_benchmark(const std::string& inputPath) {
    dlib::compress_stream::kernel_1a kernel1;
    run_benchmark(inputPath, kernel1,1);

    dlib::compress_stream::kernel_1b kernel2;
    run_benchmark(inputPath, kernel2,2);

    dlib::compress_stream::kernel_1c kernel3;
    run_benchmark(inputPath, kernel3,3);
    
    dlib::compress_stream::kernel_1da kernel4;
    run_benchmark(inputPath, kernel4,4);

    dlib::compress_stream::kernel_1db kernel5;
    run_benchmark(inputPath, kernel5,5);

    dlib::compress_stream::kernel_1ea kernel6;
    run_benchmark(inputPath, kernel6,6);

    dlib::compress_stream::kernel_1eb kernel7;
    run_benchmark(inputPath, kernel7,7);

    dlib::compress_stream::kernel_1ec kernel8;
    run_benchmark(inputPath, kernel8,8);

    dlib::compress_stream::kernel_2a kernel9;
    run_benchmark(inputPath, kernel9,9);

    dlib::compress_stream::kernel_3a kernel10;
    run_benchmark(inputPath, kernel10,10);

    dlib::compress_stream::kernel_3b kernel11;
    run_benchmark(inputPath, kernel11,11);

}


// Pass the profiles object as a parameter to the easy_compress function
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
    int kernel_index = select_best_kernel(file_type, alpha);

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


// Pass the profiles object as a parameter to the easy_compress_with_profile function
int easy_compress_with_profile(const std::string& input_path, const std::string& output_path, const std::string& profile_name, CompressionProfiles<CompressionProfile<>>& profiles) {
    // Error handling
    if (!std::filesystem::exists(input_path)) {
        throw std::invalid_argument("Input file does not exist: " + input_path);
    }

    // Get the kernel from the profile
    const char* profile_name_0 = profile_name.c_str();
    int kernel_index = profiles.get_kernel_for_profile(profile_name_0);

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
void map_kernel_and_compress(const std::string& input_filepath, const std::string& output_filepath, int kernel_index) {
    // Read input file as text
    std::ifstream input_file(input_filepath);
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
    // Read input file as text
    std::ifstream input_file(input_filepath);
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

// Pass the profiles object as a parameter to the map_kernel_and_compress function

// Pass the profiles object as a parameter to the easy_decompress function
void easy_decompress(const std::string& input_filepath, const std::string& output_filepath) {
    // Error handling
    if (!std::filesystem::exists(input_filepath)) {
        throw std::invalid_argument("Input file does not exist: " + input_filepath);
    }

    std::string filename = input_filepath;
    size_t extension_start = filename.find_last_of('.');
    if (extension_start == std::string::npos || 
        !filename.compare(extension_start, 18, ".easy_compressed_")) {
        throw std::runtime_error("Invalid compressed file: missing or incorrect extension");
    }

    int kernel_index;
    try {
        kernel_index = std::stoi(filename.substr(extension_start + 17));
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

    template <typename... Args>
    void insert(Args&&... args) {
        (insert_single(std::forward<Args>(args)), ...);
    }

private:
    void insert_single(const T& value) {
        if (!contains(value)) {
            data_.push_back(value);
        }
    }
    Vector<T> data_;
};

// Function to check if the provided file type is valid
bool isValidFileType(const std::string& fileType) {
    MySet<std::string> validFileTypes;
    validFileTypes.insert("text","play","html","Csrc","Excl","tech","poem","fax","SPRC","man");
    return validFileTypes.contains(fileType);
}
template <typename Kernel, typename InputIterator, typename OutputIterator>
void compress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin) {
    std::istringstream input_stream(std::string(input_begin, input_end));
    std::ostringstream output_stream;
    kernel.compress(input_stream, output_stream);
    if (!output_stream) {
        throw std::runtime_error("Compression failed");
    }
    std::string compressed_data = output_stream.str();
    std::copy(compressed_data.begin(), compressed_data.end(), output_begin);
}

template <typename Kernel, typename InputIterator, typename OutputIterator>
void decompress(Kernel& kernel, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin) {
    std::istringstream input_stream(std::string(input_begin, input_end));
    std::ostringstream output_stream;
    kernel.decompress(input_stream, output_stream);
    if (!output_stream) {
        throw std::runtime_error("Decompression failed");
    }
    std::string decompressed_data = output_stream.str();
    std::copy(decompressed_data.begin(), decompressed_data.end(), output_begin);
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

#endif