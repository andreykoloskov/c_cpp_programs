/*
пределите до конца тела классов, соблюдая следующие требования:

Класс Dog должен являться наследником класса Animal.

Конструктор класса Dog должен принимать параметр типа string и инициализировать им поле Name в классе Animal.
*/

#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
    Animal(const string& name) : Name(name) {}

    const string Name;
};


class Dog : public Animal {
public:
    Dog(const string& name) : Animal(name) {}

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

int main()
{
	return 0;
}
