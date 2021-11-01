#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
    SimpleVector() {
        data = nullptr;
        end_ = nullptr;
        size_ = capacity_ = 0;
    }

    explicit SimpleVector(size_t size) {
        data = new T[size];
        end_ = data + size;
        size_ = capacity_ = size;
    }

    SimpleVector(const SimpleVector& that)
        : data(new T[that.Capacity()])
        , end_(data + that.Size())
        , size_(that.Size())
        , capacity_(that.Capacity())
    {
        copy(that.begin(), that.end(), begin());
    }

    SimpleVector& operator=(const SimpleVector& that) {
        if (&that == this) {
            return *this;
        }

        delete[] data;
        data = new T[that.Capacity()];
        copy(that.begin(), that.end(), begin());
        end_ = data + that.Size();
        size_ = that.Size();
        capacity_ = that.Capacity();
        
        return *this;
    }

    ~SimpleVector() { delete[] data; }
    T& operator[](size_t index) { return data[index]; }

    T* begin() { return data; }
    T* end() { return end_; }
    const T* begin() const { return data; }
    const T* end() const { return end_; }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }

    void PushBack(const T& value) {
        if (size_ == 0) {
            size_ = capacity_ = 1;
            data = new T[size_];
            data[0] = value;
            end_ = data + size_;
        }
        else if (size_ == capacity_) {
            capacity_ *= 2;
            T* new_data = new T[capacity_];
            copy(data, end_, new_data);
            delete[] data;
            data = new_data;
            data[size_] = value;
            ++size_;
            end_ = data + size_;
        }
        else {
            data[size_] = value;
            ++size_;
            ++end_;
        }
    }

private:
    T* data;
    T* end_;
    size_t size_;
    size_t capacity_;
};