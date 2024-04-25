#ifndef COMPRESSION_PROFILES_H
#define COMPRESSION_PROFILES_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <concepts>
#include "../../src/kernel_selection.cpp"

namespace easy_compress_dlib {


// Compression profile class
template <typename Alpha = double, typename Kernel = int>
class CompressionProfile {
public:
    // type traits for alpha and kernel
    static_assert(std::is_same<Alpha, double>::value, "Alpha must be of type double");
    static_assert(std::is_integral<Kernel>::value, "Kernel must be an integral type");
    CompressionProfile(const char* profile_name, const char* file_type, Alpha alpha)
        : profile_name_(profile_name),
          file_type_(file_type),
          alpha_(alpha),
          kernel_(select_best_kernel(file_type_, alpha_)) {}

    const char* get_profile_name() const { return profile_name_; }
    const char* get_file_type() const { return file_type_; }
    Alpha get_alpha() const { return alpha_; }
    Kernel get_kernel() const { return kernel_; }
    
private:
    const char* profile_name_;
    const char* file_type_;
    Alpha alpha_;
    Kernel kernel_;
};


template<typename T>
concept IsCompressionProfile = requires(T profile) {
    { profile.get_profile_name() } -> std::same_as<const char*>;
    { profile.get_kernel() } -> std::convertible_to<int>; 
    { profile.get_file_type() } -> std::same_as<const char*>;
    { profile.get_alpha() } -> std::convertible_to<double>;
};


// Compression profiles container
template <typename Profile>
requires IsCompressionProfile<Profile>
class CompressionProfiles {
public:
    // Add a new profile
    void add_profile(Profile&& profile) {
        profiles_.emplace(profile.get_profile_name(), std::forward<Profile>(profile));
    }

    // Get kernel for a profile
    auto get_kernel_for_profile(const char* profile_name) const {
        auto it = profiles_.find(profile_name);
        if (it != profiles_.end()) {
            return it->second.get_kernel();
        } else {
            throw std::runtime_error("Profile not found");
        }
    }

    // List all profiles
    void list_profiles() const {
        for (const auto& [name, profile] : profiles_) {
            std::cout << "Profile Name: " << name
                      << ", File Type: " << profile.get_file_type()
                      << ", Alpha: " << profile.get_alpha()
                      << ", Kernel: " << profile.get_kernel() << std::endl;
        }
    }

    // Save profiles to a file
    void save_profiles(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& [name, profile] : profiles_) {
                file << name << "," << profile.get_file_type() << ","
                     << profile.get_alpha() << "," << profile.get_kernel() << std::endl;
            }
        } else {
            throw std::runtime_error("Failed to open file for writing");
        }
    }

    // Load profiles from a file
    void load_profiles(const std::string& filename) {
        std::ifstream input_file(filename);
        if (!input_file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::string line;
        int line_number = 1; // Track line number for error reporting
        while (std::getline(input_file, line)) {
            std::istringstream line_stream(line); // Changed to std::istringstream
            std::string cell;
            std::vector<std::string> values;

        // Extract comma-separated values
        while (std::getline(line_stream, cell, ',')) {
            values.push_back(cell);
        }

            if (values.size() == 4) {
                try {
                    std::string profile_name_0 = values[0];
                    std::string file_type_0 = values[1];
                    const char *profile_name = profile_name_0.c_str();
                    const char *file_type = file_type_0.c_str();
                    double alpha = std::stod(values[2]);
                    int kernel = std::stoi(values[3]);

                    // Create a temporary profile and add it to the profiles_ map
                    CompressionProfile profile(profile_name,file_type, alpha);
                    profiles_.emplace(profile_name, std::move(profile));
                } catch (const std::exception& e) {
                    // Handle conversion errors (e.g., invalid alpha or kernel)
                    std::cerr << "Error on line " << line_number << ": " << e.what() << std::endl;
                }
            } else {
                // Handle lines with incorrect number of values
                std::cerr << "Incorrect format on line " << line_number << ": Expected 4 comma-separated values" << std::endl;
            }

            line_number++;
        }
    }

private:
    std::map<std::string, Profile> profiles_;
};

}

#endif // COMPRESSION_PROFILES_H 