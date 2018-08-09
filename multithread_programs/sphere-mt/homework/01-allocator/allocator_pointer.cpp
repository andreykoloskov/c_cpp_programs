#include "allocator_pointer.h"
#include "allocator.h"


Pointer::Pointer(Allocator *allocator, uint64_t id):
        allocator(allocator), id(id) {}

void *Pointer::get_pointer() const { return allocator->getCursor(id); }

uint64_t Pointer::get_id() const { return id; }

size_t Pointer::get_size() const { return allocator->getSize(id); }

void *Pointer::get_allocator() { return allocator; };

void *Pointer::get() const { return get_pointer(); }

Pointer::operator void*() { return get_pointer(); }

void *Pointer::operator->() { return get_pointer(); }

std::ptrdiff_t Pointer::operator-(Pointer p) {
    return (char *) get_pointer() - (char *) p.get_pointer();
}

std::ptrdiff_t Pointer::operator-(void *p) {
    return (char *) get_pointer() - (char *) p;
}

bool Pointer::operator==(Pointer p) {
    return (char *) get_pointer() == (char *) p.get_pointer();
}
