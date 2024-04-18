#ifndef EASY_COMPRESS_DLIB_COMPRESSION_H
#define EASY_COMPRESS_DLIB_COMPRESSION_H

#include <string>

namespace easy_compress_dlib {

// Compression and decompression functions using various kernels

// compress_stream_kernel_1 implementations
void compress_kernel_1a(const std::string& input, std::string& output);
void decompress_kernel_1a(const std::string& input, std::string& output);
void compress_kernel_1b(const std::string& input, std::string& output);
void decompress_kernel_1b(const std::string& input, std::string& output);
void compress_kernel_1c(const std::string& input, std::string& output);
void decompress_kernel_1c(const std::string& input, std::string& output);
void compress_kernel_1da(const std::string& input, std::string& output);
void decompress_kernel_1da(const std::string& input, std::string& output);
void compress_kernel_1db(const std::string& input, std::string& output);
void decompress_kernel_1db(const std::string& input, std::string& output);
void compress_kernel_1ea(const std::string& input, std::string& output);
void decompress_kernel_1ea(const std::string& input, std::string& output);
void compress_kernel_1eb(const std::string& input, std::string& output);
void decompress_kernel_1eb(const std::string& input, std::string& output);
void compress_kernel_1ec(const std::string& input, std::string& output);
void decompress_kernel_1ec(const std::string& input, std::string& output);

// compress_stream_kernel_2 implementation
void compress_kernel_2a(const std::string& input, std::string& output);
void decompress_kernel_2a(const std::string& input, std::string& output);

// compress_stream_kernel_3 implementations
void compress_kernel_3a(const std::string& input, std::string& output);
void decompress_kernel_3a(const std::string& input, std::string& output);
void compress_kernel_3b(const std::string& input, std::string& output);
void decompress_kernel_3b(const std::string& input, std::string& output);

// remaining functions

int easy_compress(const std::string& input_filepath, const std::string& output_filepath, const std::string& file_type, double alpha);
int easy_compress_with_profile(const std::string& input_path, const std::string& output_path, const std::string& profile_name);
void easy_decompress(const std::string& input_filepath, const std::string& output_filepath);
void map_kernel_and_compress(const std::string& input_filepath, const std::string& output_filepath, int kernel_index);
void map_kernel_and_decompress(const std::string& input_filepath, const std::string& output_filepath, int kernel_index);
bool isValidFileType(const std::string& fileType);

} // namespace easy_compress_dlib

#endif // EASY_COMPRESS_DLIB_COMPRESSION_H
