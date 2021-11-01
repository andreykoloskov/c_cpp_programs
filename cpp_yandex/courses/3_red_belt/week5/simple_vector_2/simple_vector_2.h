#include <cstdint>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
    SimpleVector() = default;
    
    explicit SimpleVector(size_t size)
        : data_(new T[size])
        , end_(data_ + size)
        , size_(size)
        , capacity_(size)
    {
    }

    ~SimpleVector() { delete[] data_; }

    T& operator[](size_t index) { return data_[index]; }

    T* begin() { return data_; }
    T* end() { return end_; }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }

    void PushBack(T value)
    {
        if (size_ == 0) {
            capacity_ = 1;
            data_ = new T[capacity_];
        }
        else if (size_ == capacity_) {
            capacity_ *= 2;
            T* new_data = new T[capacity_];
            move(data_, end_, new_data);
            delete[] data_;
            data_ = new_data;
        }

        data_[size_] = move(value);
        ++size_;
        end_ = data_ + size_;
    }

    // При необходимости перегрузите
    // существующие публичные методы

private:
    // Добавьте сюда поля
    T* data_ = nullptr;
    T* end_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};