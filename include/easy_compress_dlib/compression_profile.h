#ifndef COMPRESSION_PROFILES_H
#define COMPRESSION_PROFILES_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <concepts>

namespace easy_compress_dlib {

// Type traits to check if a type is a tuple
template <typename T>
struct is_tuple : std::false_type {};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

// Concept to check if a type is a valid compression profile
template <typename T>
concept valid_compression_profile = is_tuple<T>::value && std::tuple_size<T>::value == 4 &&
                                   std::is_same_v<std::tuple_element_t<0, T>, std::string> &&
                                   std::is_same_v<std::tuple_element_t<1, T>, std::string> &&
                                   std::is_floating_point_v<std::tuple_element_t<2, T>> &&
                                   std::is_integral_v<std::tuple_element_t<3, T>>;

// Kernel selection function (placeholder) - get from kernel_selection
// template <std::string file_type, double alpha>
// int kernel_selection() {
//     // Implement your kernel selection logic here
//     return 123;
// }

// Compression profile class
template <typename ProfileName = std::string, typename FileType= std::string, typename Alpha = double, typename Kernel = int>
class CompressionProfile {
public:
    CompressionProfile(ProfileName&& profile_name, FileType&& file_type, Alpha alpha)
        : profile_name_(std::forward<ProfileName>(profile_name)),
          file_type_(std::forward<FileType>(file_type)),
          alpha_(alpha),
          kernel_(kernel_selection<FileType, Alpha>(file_type_, alpha_)) {}

    const ProfileName& get_profile_name() const { return profile_name_; }
    const FileType& get_file_type() const { return file_type_; }
    Alpha get_alpha() const { return alpha_; }
    Kernel get_kernel() const { return kernel_; }

private:
    ProfileName profile_name_;
    FileType file_type_;
    Alpha alpha_;
    Kernel kernel_;
};

// Compression profiles container
template <typename Profile>
class CompressionProfiles {
public:
    // Add a new profile
    void add_profile(Profile&& profile) {
        profiles_.emplace(profile.get_profile_name(), std::forward<Profile>(profile));
    }

    // Get kernel for a profile
    template <typename ProfileName>
    auto get_kernel_for_profile(const ProfileName& profile_name) const {
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
                    std::string profile_name = values[0];
                    std::string file_type = values[1];
                    double alpha = std::stod(values[2]);
                    int kernel = std::stoi(values[3]);

                    // Create a temporary profile and add it to the profiles_ map
                    CompressionProfile profile(profile_name, file_type, alpha);
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
