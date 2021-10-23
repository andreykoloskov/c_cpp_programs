#pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
    typedef typename array<T, N>::iterator iterator;
    typedef typename array<T, N>::const_iterator const_iterator;

    explicit StackVector(size_t a_size = 0);

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    iterator begin() { return data.begin(); }
    iterator end() { return data.begin() + size_; }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.begin() + size_; }

    size_t Size() const { return size_; }
    size_t Capacity() const { return data.size(); }

    void PushBack(const T& value);
    T PopBack();

private:
    array<T, N> data;
    size_t size_;
};


template <typename T, size_t N>
StackVector<T, N>::StackVector(size_t a_size)
    : size_(a_size)
{
    if (a_size > N) {
        throw invalid_argument("");
    }
}

template <typename T, size_t N>
void StackVector<T, N>::PushBack(const T& value)
{
    if (size_ == N) {
        throw overflow_error("");
    }

    data[size_++] = value;
}

template <typename T, size_t N>
T StackVector<T, N>::PopBack()
{
    if (size_ == 0) {
        throw underflow_error("");
    }

    --size_;
    return data[size_];
}

