#include <vector>

class sliding_buffer {
public:
    sliding_buffer() : buffer_(1024) {} // Initialize with a reasonable default size

    void set_size(size_t size) {
        buffer_.resize(size);
    }

    void rotate_left(size_t n) {
        std::rotate(buffer_.begin(), buffer_.begin() + n, buffer_.end());
    }

    size_t get_element_id(size_t index) const {
        return index;
    }

    size_t size() const {
        return buffer_.size();
    }

    unsigned char& operator[](size_t index) {
        return buffer_[index];
    }

    const unsigned char& operator[](size_t index) const {
        return buffer_[index];
    }

private:
    std::vector<unsigned char> buffer_;
};
