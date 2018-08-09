#ifndef ALLOCATOR
#define ALLOCATOR
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include "allocator_pointer.h"

// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Pointer;

/**
 * Wraps given memory area and provides defagmentation allocator interface on
 * the top of it.
 *
 *
 */
class Allocator {
private:
    void *base;
    size_t size;
    uint64_t max_id;
    std::map<uint64_t, std::pair<void*, size_t>> active;
    std::vector<std::pair<void*, size_t>> vacant;
public:
    Allocator(void* base, size_t size);

    void *getCursor(uint64_t id);

    size_t getSize(uint64_t id);

    /**
     * TODO: semantics
     * @param N size_t
     */
    Pointer alloc(size_t N);

    /**
     * TODO: semantics
     * @param p Pointer
     * @param N size_t
     */
    void realloc(Pointer& p, size_t N);

    /**
     * TODO: semantics
     * @param p Pointer
     */
    void free(Pointer& p);

    /**
     * TODO: semantics
     */
    void defrag();

    /**
     * TODO: semantics
     */
    std::string dump() const;
};

#endif // ALLOCATOR
