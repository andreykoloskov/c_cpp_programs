#include <iostream>

template <class Type>
class SmartPointerExample {
private:
    Type *pointer;

public:
    SmartPointerExample(Type *p): pointer(p) {}
    operator Type*() { return pointer; }
    Type *operator->() {
        if (!pointer) {
            pointer = new Type();
            std::cerr << "Bad pointer!" << std::endl;
        }
        return pointer;
    }
    std::ptrdiff_t operator-(SmartPointerExample<Type> p) {
        return pointer - p;
    }
    std::ptrdiff_t operator-(void *p) {
        return pointer - p;
    }
};

class Foo {
private:
    int a, b;

public:
    Foo(): a(0), b(0) {}
    Foo(int a, int b): a(a), b(b) {}
    int Sum() { return a + b; }
};

int main(int argc, char **argv) {
    SmartPointerExample<Foo> sp(new Foo(2, 2));
    std::cout << sp->Sum() << std::endl;

    return 0;
}