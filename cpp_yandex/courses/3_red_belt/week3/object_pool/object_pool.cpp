#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <queue>
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();

    void Deallocate(T* object);

    ~ObjectPool();

private:
    set<T*> allocated_;
    queue<T*> deallocated_;
};

template<class T>
T* ObjectPool<T>::Allocate()
{
    T* object = TryAllocate();

    if (object == nullptr) {
        object = new T;
        allocated_.insert(object);
    }

    return object;
}

template<class T>
T* ObjectPool<T>::TryAllocate()
{
    T* object = nullptr;

    if (deallocated_.size() > 0) {
        object = deallocated_.front();
        allocated_.insert(object);
        deallocated_.pop();
    }

    return object;
}

template<class T>
void ObjectPool<T>::Deallocate(T* object)
{
    auto res = allocated_.find(object);
    if (res != allocated_.end()) {
        deallocated_.push(object);
        allocated_.erase(object);
    }
    else {
        throw invalid_argument("");
    }
}

template<class T>
ObjectPool<T>::~ObjectPool()
{
    for (auto object : allocated_) {
        delete object;
    }
    allocated_.clear();

    while (deallocated_.size() != 0) {
        delete deallocated_.front();
        deallocated_.pop();
    }
}

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}