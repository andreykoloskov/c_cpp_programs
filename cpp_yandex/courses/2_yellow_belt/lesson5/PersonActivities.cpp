#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name, const string& type) : Name(name), Type(type) {}

    const string getName() const { return Name; }
    const string getType() const { return Type; }
    const string getTypeName() const { return Type + ": " + Name; }

    virtual void Walk(const string& destination) const {
        cout << getTypeName() << " walks to: " << destination << endl;
    }

private:
    const string Name;
    const string Type;
};

class Student : public Person {
public:

    Student(const string& name, const string& favouriteSong) :
            Person(name, "Student"), FavouriteSong(favouriteSong) {}

    void Learn() const {
        cout << getTypeName() << " learns" << endl;
    }

    void SingSong() const {
        cout << getTypeName() << " sings a song: " << FavouriteSong << endl;
    }

    void Walk(const string& destination) const {
        Person::Walk(destination);
        SingSong();
    }

private:
    const string FavouriteSong;
};


class Teacher : public Person {
public:

    Teacher(const string& name, const string& subject) :
            Person(name, "Teacher"), Subject(subject) {}

    void Teach() const {
        cout << getTypeName() << " teaches: " << Subject << endl;
    }

private:
    const string Subject;
};


class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name, "Policeman") {}

    void Check(const Person& p) const {
        cout << getTypeName() << " checks " << p.getType() << ". "
                << p.getType() << "'s name is: " << p.getName() << endl;
    }
};

void VisitPlaces(Person& person, const vector<string>& places) {
    for (auto p : places) {
        person.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
