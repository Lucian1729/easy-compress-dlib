#include <iostream>
#include <string>
#include <sstream>
#include "../include/easy_compress_dlib/custom_vector.h"
#include "../include/easy_compress_dlib/easy_compress.h"

using namespace easy_compress_dlib;

int main() {
    // Compress a file using easy_compress
    std::cout << "start";
    int kernel_index = easy_compress_dlib::easy_compress("input_html.html", "output_file", "text", 0.3);
    std::cout << "Compressed file using kernel " << kernel_index << std::endl;

    // Decompress a file using easy_decompress
    std::string  output_file = "output_file.easy_compressed_" + std::to_string(kernel_index);
    easy_compress_dlib::easy_decompress(output_file, "decompressed_file.txt");
    std::cout << "Decompressed file" << std::endl;

    // Create a compression profile and use it to compress a file
    using MyProfile = CompressionProfile<>;
    CompressionProfiles<MyProfile> profiles;

    MyProfile profile("MY Profile", "html", 0.8);
    profiles.add_profile(std::move(profile));

    int profile_kernel_index = easy_compress_dlib::easy_compress_with_profile("input_file.html", "output_file_profile", "MyProfile",profiles);
    std::cout << "Compressed file using profile with kernel " << profile_kernel_index << std::endl;

    // List all available profiles
    profiles.list_profiles();

    // Save and load profiles
    profiles.save_profiles("compression_profiles.csv");
    profiles.load_profiles("compression_profiles.csv");
    profiles.list_profiles();

    return 0;
}