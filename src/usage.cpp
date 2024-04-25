#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include "../include/easy_compress_dlib/custom_vector.h"
#include "../include/easy_compress_dlib/easy_compress.h"

using namespace easy_compress_dlib;

int main() {
 
    std::string inputPath = "input_file.txt";
    std::string outputPath = "output_file";
    std::string fileType = "text";
    double alpha = 0.1;

    std::string outputPath_hashed = "output_hashes.txt";
    hashFileContents(inputPath, outputPath_hashed,"abcd","asgdsada");
    std::cout << "\nHashing completed. Hash values stored in " << outputPath << std::endl;

    std::string dehashOutputPath = "dehashed_words.txt";
    dehashFileWords(outputPath_hashed, dehashOutputPath, inputPath,"abcd","asgdsada");
    std::cout << "\nDehashing completed. Original words stored in " << dehashOutputPath << std::endl;

    int kernel_index = easy_compress(inputPath, outputPath + ".easy_compressed_" + std::to_string(kernel_index), fileType, alpha);
    std::cout << "\nCompressed file using kernel " << kernel_index << std::endl;

    // Decompress the file using easy_decompress
    std::string output_file = outputPath + ".easy_compressed_" + std::to_string(kernel_index);
    easy_decompress(output_file, "decompressed_file.txt");
    std::cout << "\nDecompressed file" << std::endl << "BenchMark : \n";

    // Benchmarking compression and decompression

    select_and_run_benchmark(inputPath);

    // Create a compression profile and use it to compress a file
    using MyProfile = CompressionProfile<>;
    CompressionProfiles<MyProfile> profiles;

    MyProfile profile("MyProfile", "html", alpha);
    profiles.add_profile(std::move(profile));

    int profile_kernel_index = easy_compress_dlib::easy_compress_with_profile("input_file.html", "output_file_profile", "MyProfile",profiles);
    std::cout << "\nCompressed file using profile with kernel " << profile_kernel_index << std::endl;

    // List all available profiles
    profiles.list_profiles();

    // Save and load profiles
    profiles.save_profiles("compression_profiles.csv");
    profiles.load_profiles("compression_profiles.csv");
    std::cout << "\nAfter loading profile from file : \n";
    profiles.list_profiles();

    profile_kernel_index = easy_compress_dlib::easy_compress_with_profile("input_file.html", "output_file_profile", "MyProfile",profiles);
    std::cout << "Compressed file using profile with kernel after loading profile from file " << profile_kernel_index << std::endl;

    return 0;
}