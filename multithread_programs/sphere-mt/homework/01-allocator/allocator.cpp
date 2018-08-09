#include <algorithm>
#include "allocator.h"
#include "allocator_error.h"

Allocator::Allocator(void* base, size_t size): base(base), size(size),
        max_id(0) {}

void * Allocator::getCursor(uint64_t id) {
    if (active.count(id) > 0)
        return active[id].first;
    else
        return nullptr;
}

size_t Allocator::getSize(uint64_t id) {
    if (active.count(id) > 0)
        return active[id].second;
    else
        return 0;
}

Pointer Allocator::alloc(size_t N) {
    void *p = base;
    int free_point = 0;

    if (vacant.size() > 0) {
        auto it = std::find_if(vacant.begin(), vacant.end(),
                [&](std::pair<void*, size_t> t) { return t.second >= N; });
        if (it != vacant.end()) {
            p = it->first;
            free_point = 1;
        }
    }

    if (!free_point) {
        if (active.size() > 0) {
            auto it = std::max_element(active.begin(), active.end(),
                    [](std::pair<uint64_t, std::pair<void *, size_t>> t1,
                            std::pair<uint64_t, std::pair<void *, size_t>> t2) {
                        return (char *) t1.second.first
                                < (char *) t2.second.first;
            });

            p = (char *) it->second.first + it->second.second;
        }

        if ((char *) p - (char *) base + N > size) {
            throw AllocError(AllocErrorType::NoMemory, "No memory");
        }
    }

    active[max_id] = std::make_pair(p, N);
    uint64_t id = max_id;
    max_id++;

    return Pointer(this, id);
}

void Allocator::free(Pointer &p) {
    if (!p || p.get_pointer() == NULL) {
        return;
    }

    if (active.count(p.get_id()) > 0) {
        vacant.push_back(std::make_pair(p.get_pointer(),
                active[p.get_id()].second));
        active.erase(p.get_id());
    }
}

void Allocator::defrag() {
    for (auto v : vacant) {
        memcpy((char *) v.first, (char *) v.first + v.second,
               size - ((char *) v.first - (char *) base) - v.second);

        for (auto it = active.begin(); it != active.end(); it++) {
            if ((char *) it->second.first >= (char *) v.first + v.second) {
                it->second.first = (char *) it->second.first - v.second;
            }
        }
    }

    vacant.clear();
}

void Allocator::realloc(Pointer &p, size_t N) {
    if (p.get_allocator()) {
        void *pointer = p.get_pointer();
        size_t size = 0;
        size = p.get_size();
        free(p);
        p = alloc(N);
        memcpy(p.get_pointer(), pointer, std::min(size, N));
    } else {
        p = alloc(N);
    }
}

std::string Allocator::dump() const { return ""; }
