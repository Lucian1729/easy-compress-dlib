#include <bits/stdc++.h>
#include <sstream>
#include <string>
#include "../dlib/lz77_buffer/lz77_buffer_kernel_1.h"
#include "../dlib/lz77_buffer/lz77_buffer_kernel_2.h"
#include "../dlib/lz77_buffer/lz77_buffer_kernel_abstract.h"
#include "../dlib/lz77_buffer/lz77_buffer_kernel_c.h"
#include "sliding_buffer.h" 
#include "lz77_buffer_kernel_1_wrapper.h"

// Define the total_limit and lookahead_limit values
const unsigned long total_limit = 16;  // Example value, adjust as needed
const unsigned long lookahead_limit = 32;  // Example value, adjust as needed

template<typename T>
void compress_and_decompress(std::string& input_data) {
    try {
        // Create instances of lz77_buffer for compression and decompression
        T compressor(total_limit, lookahead_limit);
        T decompressor(total_limit, lookahead_limit);

        std::stringstream compressed_data_stream;
        std::stringstream decompressed_data_stream;

        // Compress the input data
        for (char c : input_data) {
            compressor.add(static_cast<unsigned char>(c));
            unsigned long index, length;
            compressor.find_match(index, length, 3);  // Minimum match length of 3
            if (length > 0) {
                // Encode the match (index, length)
                compressed_data_stream << static_cast<unsigned char>((index >> 8) & 0xFF); // High byte of index
                compressed_data_stream << static_cast<unsigned char>(index & 0xFF);        // Low byte of index
                compressed_data_stream << static_cast<unsigned char>(length);              // Length
            } else {
                // Encode the literal character
                compressed_data_stream << c;
            }
            compressor.shift_buffers(length);
        }

        std::string compressed_data = compressed_data_stream.str();

        // Decompress the compressed data
        for (size_t i = 0; i < compressed_data.size();) {
            unsigned char c = compressed_data[i++];
            if (c & 0x80) { // Check if high bit is set
                // Decode the match (index, length)
                unsigned long index = ((c & 0x7F) << 8) | compressed_data[i++];
                unsigned long length = compressed_data[i++];
                // Add the match to the decompressed data
                for (unsigned long j = 0; j < length; ++j) {
                    decompressed_data_stream << decompressor.history_buffer(index - j);
                }
            } else {
                // Decode the literal character
                decompressed_data_stream << c;
                decompressor.add(c); // Add to the decompressor
            }
        }

        std::string decompressed_data = decompressed_data_stream.str();

        // Output the results
        std::cout << "Original Data:\n" << input_data << std::endl;
        std::cout << "Compressed Data:\n" << compressed_data << std::endl;
        std::cout << "Decompressed Data:\n" << decompressed_data << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    }
}


int main() {
    std::string input_data;
    char buffer[1024];

    // Read data from standard input
    while (std::cin.read(buffer, sizeof(buffer))) {
        input_data.append(buffer, std::cin.gcount());
    }

    // Call the compress_and_decompress function with different template arguments
    compress_and_decompress<dlib::lz77_buffer_kernel_1<sliding_buffer>>(input_data);
    compress_and_decompress<dlib::lz77_buffer_kernel_2<sliding_buffer>>(input_data);
    compress_and_decompress<dlib::lz77_buffer_kernel_c<dlib::lz77_buffer_kernel_2<sliding_buffer>>>(input_data);

    return 0;
}

//g++ -o compression_example src\compression_lz77.cpp -I.
