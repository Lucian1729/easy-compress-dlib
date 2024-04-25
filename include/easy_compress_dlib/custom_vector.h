#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void check_index(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
    }

public:
    // Constructors
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    explicit Vector(size_t initial_capacity) 
        : data_(initial_capacity ? new T[initial_capacity] : nullptr), size_(0), capacity_(initial_capacity) {}

    Vector(const Vector& other) 
        : data_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    Vector(Vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector& operator=(Vector other) {
        swap(other);
        return *this;
    }

    ~Vector() {
        delete[] data_;
    }

    void swap(Vector& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

    T* begin() {
        return data_;
    }

    const T* begin() const {
        return data_;
    }

    T* end() {
        return data_ + size_;
    }

    const T* end() const {
        return data_ + size_;
    }
    
    // Accessors
    size_t getSize() const { return size_; }
    size_t getCapacity() const { return capacity_; }
    T& operator[](size_t index) {
        check_index(index);
        return data_[index];
    }
    const T& operator[](size_t index) const {
        check_index(index);
        return data_[index];
    }

    // Modifiers
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = new T[new_capacity];
            std::move(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // Utility
    void clear() {
        size_ = 0;  // Optionally destroy elements if T is a complex type
    }
};

#endif  // VECTOR_H
