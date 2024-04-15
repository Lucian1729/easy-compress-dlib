#ifndef VECTOR_H
#define VECTOR_H


#include <cstdlib>
#include <bits/stdc++.h>
#include <type_traits>
#include <algorithm>

// Concept to ensure that the type T is copyable
template <typename T>
concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

// Concept to ensure that the type T is move-constructible and move-assignable
template <typename T>
concept MoveAble = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

template <typename T>
class Vector {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    // Constructors
    Vector() : data(nullptr), size(0), capacity(0) {}
    explicit Vector(size_t initial_capacity) : data(new T[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Variadic template constructor to support uniform initialization
    template <typename... Args>
    explicit Vector(Args&&... args) : data(new T[sizeof...(Args)]), size(sizeof...(Args)), capacity(sizeof...(Args)) {
        size_t index = 0;
        (void)std::initializer_list<int>{(data[index++] = std::forward<Args>(args), 0)...};
    }

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Copy constructor and assignment operator
    Vector(const Vector& other) : data(new T[other.capacity]), size(other.size), capacity(other.capacity) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.capacity];
            size = other.size;
            capacity = other.capacity;
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move constructor and assignment operator
    Vector(Vector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Template friend function to allow access to private members
    template <typename U>
    friend void swapElements(Vector<U>& v1, Vector<U>& v2, size_t index1, size_t index2);

    // Member functions
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }

    void push_back(const T& value) {
        if (size == capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < size; ++i) {
                new_data[i] = std::move(data[i]);
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    // Rotate the vector to the left by n positions
    void rotate(size_t n) {
        n %= size; // Handle cases where n is greater than size
        std::rotate(data, data + n, data + size);
    }
};

// Template friend function to swap elements in two vectors
template <typename U>
void swapElements(Vector<U>& v1, Vector<U>& v2, size_t index1, size_t index2) {
    std::swap(v1[index1], v2[index2]);
}
