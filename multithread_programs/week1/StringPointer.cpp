#include <iostream>
#include <string>

class StringPointer {
private:
    int allocate;
    std::string *pointer;

public:
    StringPointer(std::string *p): pointer(p), allocate(0) {}
    operator std::string*() {
        if (!pointer) {
            pointer = new std::string("");
            allocate = 1;
        }
        return pointer;
    }
    std::string *operator->() {
        if (!pointer) {
            pointer = new std::string("");
            allocate = 1;
        }
        return pointer;
    }
    ~StringPointer() {
        if (allocate) {
            delete(pointer);
        }
    }
    std::ptrdiff_t operator-(StringPointer p) {
        return pointer - p;
    }
    std::ptrdiff_t operator-(std::string *p) {
        return pointer - p;
    }
};

int main(int qrgc, char **argv) {
    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
}

/*
Тут задача очень простая. Вам нужно создать умный указатель на объект
 класса std::string.
 Он должен иметь защиту от разыменования NULL - при попытке это сделать он
 должен конструировать внутри себя пустую строку.

Следующий код должен работать.

std::string s1 = "Hello, world!";

StringPointer sp1(&s1);
StringPointer sp2(NULL);

std::cout << sp1->length() << std::endl;
std::cout << *sp1 << std::endl;
std::cout << sp2->length() << std::endl;
std::cout << *sp2 << std::endl;

N.B. Так как ваш указатель при определенных случаях может конструировать внутри
 себя пустую строку, не забудьте позаботиться об освобождении памяти в
 деструкторе. Система не проверяет корректность работы в памятью,
 так что это остается на вашу совесть :)
*/
