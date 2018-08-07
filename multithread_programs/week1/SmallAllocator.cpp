#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cstring>

class SmartPointer {
private:
    void *pointer;

public:
    SmartPointer(void *p): pointer(p) {}
    operator void*() { return pointer; }
    void *operator->() { return pointer; }
    std::ptrdiff_t operator-(SmartPointer p) {
        return (char *) pointer - (char *) ((void *) p);
    }
    std::ptrdiff_t operator-(void *p) {
        return (char *) pointer - (char *) p;
    }
};

class SmallAllocator {
private:
    static const int capacity = 1048576;
    char Memory[capacity];
    std::vector<std::pair<char*, char*>> Pointers;

public:
    void *Alloc(unsigned int size) {
        char *pointer = Memory;
        if (Pointers.size() > 0) {
            pointer = Pointers[Pointers.size() - 1].second;
        }

        if (pointer - Memory + size >= capacity) {
            return NULL;
        }

        Pointers.push_back(std::make_pair(pointer, pointer + size));

        return pointer;
    }

    void *ReAlloc(void *pointer, unsigned int size) {
        auto it = Pointers.begin();
        for (it = Pointers.begin(); it != Pointers.end(); it++) {
            if (pointer == it->first) {
                break;
            }
        }

        if (Pointers.size() == 0 || it == Pointers.end()) {
            return NULL;
        }

        char *new_pointer = Pointers[Pointers.size() - 1].second;
        if (new_pointer - Memory + size >= capacity) {
            Free(it->first);
            return NULL;
        }

        int old_size = it->second - it->first;
        int copy_size = size > old_size ? old_size : size;
        memcpy(new_pointer, it->first, copy_size);
        Pointers.push_back(std::make_pair(new_pointer, new_pointer + size));
        Free(it->first);

        return new_pointer;
    }

    void Free(void *pointer) {
        auto it = Pointers.begin();
        for (it = Pointers.begin(); it != Pointers.end(); it++) {
            if (pointer == it->first) {
                break;
            }
        }

        if (Pointers.size() == 0 || it == Pointers.end()) {
            return;
        }

        Pointers.erase(it);
    }
};

int main(int argc, char **argv) {
    SmallAllocator A1;
    int *A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int *A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P1[i] != i)
            std::cout << "ERROR 1" << std::endl;
    int *A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P1[i] != i)
            std::cout << "ERROR 2" << std::endl;
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
            std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for (unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 20; i++)
        if (A2_P1[i] != i)
            std::cout << "ERROR 4" << std::endl;
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
            std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for (unsigned int i = 0; i < 5; i++)
        if (A2_P1[i] != i)
            std::cout << "ERROR 6" << std::endl;
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
            std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
}