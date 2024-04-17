#include <iostream>
#include <string>
#include "../include/easy_compress_dlib/compression_profile.h"
#include "../include/easy_compress_dlib/compression.h"

using namespace easy_compress_dlib;

int main() {
    // Compress a file using easy_compress
    int kernel_index = easy_compress("input_file.txt", "output_file", "text", 0.7);
    std::cout << "Compressed file using kernel " << kernel_index << std::endl;

    // Decompress a file using easy_decompress
    easy_decompress("output_file.easy_compressed_1", "decompressed_file.txt");
    std::cout << "Decompressed file" << std::endl;

    // Create a compression profile and use it to compress a file
    using MyProfile = CompressionProfile<>;
    CompressionProfiles<MyProfile> profiles;

    MyProfile profile("MyProfile", "html", 0.8, 5);
    profiles.add_profile(std::move(profile));

    int profile_kernel_index = easy_compress_with_profile("input_file.html", "output_file_profile", "MyProfile");
    std::cout << "Compressed file using profile with kernel " << profile_kernel_index << std::endl;

    // List all available profiles
    profiles.list_profiles();

    // Save and load profiles
    profiles.save_profiles("compression_profiles.csv");
    profiles.load_profiles("compression_profiles.csv");

    profiles.list_profiles();

    return 0;
}