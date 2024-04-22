#include <iostream>
#include <sstream>
#include <dlib/compress_stream.h>

using namespace std;
using namespace dlib;

// Function to compress a string
string compressString(const string& input) {
    stringstream compressed_data;
    istringstream input_stream(input);
    compress_stream::kernel_1a compressor;
    compressor.compress(input_stream, compressed_data);
    return compressed_data.str();
}

// Function to decompress a string
string decompressString(const string& compressed_input) {
    stringstream decompressed_data(compressed_input);
    stringstream output;
    compress_stream::kernel_1a decompressor;
    decompressor.decompress(decompressed_data, output);
    return output.str();
}

int main() {
    cout << "Enter the original string to compress:" << endl;
    stringstream input_stream;
    char c;
    while (cin.get(c)) {
        input_stream << c;
    }
    string original_string = input_stream.str();

    // Compress the string
    string compressed_string = compressString(original_string);
    cout << "Compressed string:" << endl;
    cout << compressed_string << endl;

    // Decompress the string
    string decompressed_string = decompressString(compressed_string);
    cout << "Decompressed string: " << decompressed_string << endl;

    // Calculate compression ratio
    double compression_ratio = static_cast<double>(original_string.length()) / compressed_string.length();
    cout << "Compression ratio: " << compression_ratio << endl;

    return 0;
}
