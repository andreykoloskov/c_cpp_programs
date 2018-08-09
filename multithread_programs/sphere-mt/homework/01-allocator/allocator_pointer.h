#ifndef ALLOCATOR_POINTER
#define ALLOCATOR_POINTER

#include <iostream>

// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Allocator;

class Pointer {
private:
    Allocator *allocator;
    uint64_t id;
public:
    Pointer(Allocator *allocator = NULL, uint64_t id = 0);

    void* get_pointer() const;
    uint64_t get_id() const;
    size_t get_size() const;
    void *get_allocator();
    void* get() const;

    operator void*();

    void *operator->();

    std::ptrdiff_t operator-(Pointer p);

    std::ptrdiff_t operator-(void *p);

    bool operator==(Pointer p);
};

#endif //ALLOCATOR_POINTER
