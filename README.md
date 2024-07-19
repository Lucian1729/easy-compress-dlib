# easy-compress-dlib

## Project Description

easy-compress-dlib is a data compression library built with dlibc++. Our main aim is to provide intelligent compression kernel selection based on input data characteristics and compression speed, with user-adjustable importance for these factors. As the project was made as part of the Generic Programming course, we also use a lot of GP features.

Key features:
- Multiple compression algorithms, including LZ77
- Automatic kernel selection based on data characteristics and user preferences
- Compression profiles for optimal kernel selection based on file type
- Extensive use of Generic Programming principles through C++ templates

This library is designed for developers building compression applications, offering a balance between efficient storage utilization and data integrity.

## Generic Programming Features

1. Variadic templates
   - `CompressionProfiles` class for variable number of compression profiles
   - Parameter packs for multiple kernel passing
   - Support for uniform initialization

2. Template specialization
   - Specialized templates for known kernel metrics sizes at compile time

3. Lambda templates
   - Alternative implementation for compression/decompression functions

4. Fold expressions
   - Used in kernel metrics creation

5. Type traits
   - Accepting tuples as part of compression profiles

6. Template friendship
   - Allowing access to private members of custom vector class
   - Enabling element swapping between vectors

7. Concepts and Constraints
   - Creating concepts for valid compression profiles
   - Checking if vector types are copyable, move-constructible, and move-assignable

8. Non-type template parameters
   - Used for generic data structures storing metrics and specifying sizes

## Contributors

- Nischal H S:
  - Generic compression and decompression wrappers
  - Kernel selection function
  - Metrics handling
  - Compression profiles

- Puneet Sheokand:
  - LZ77 kernel implementation
  - Kernel selection function
  - Metrics handling
