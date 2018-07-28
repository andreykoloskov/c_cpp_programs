#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

using namespace std;

#define PI 3.14

class Figure {
public:
    virtual const string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
public:
    Triangle(const int a, const int b, const int c):
            name_("TRIANGLE"), a_(a), b_(b), c_(c) {}

    const string Name() const override {
        return name_;
    }

    double Perimeter() const override {
        return a_ + b_ + c_;
    }

    double Area() const override {
        double p = (a_ + b_ + c_) / 2.0;
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }

private:
    const string name_;
    const int a_;
    const int b_;
    const int c_;
};

class Rect : public Figure {
public:
    Rect(const int width, const int height):
            name_("RECT"), width_(width), height_(height) {}

    const string Name() const override {
        return name_;
    }

    double Perimeter() const override {
        return 2 * (width_ + height_);
    }

    double Area() const override {
        return width_ * height_;
    }

private:
    const string name_;
    const int width_;
    const int height_;
};

class Circle : public Figure {
public:
    Circle(const int r): name_("CIRCLE"), r_(r) {}

    const string Name() const override {
        return name_;
    }

    double Perimeter() const override {
        return 2 * PI * r_;
    }

    double Area() const override {
        return PI * r_ * r_;
    }

private:
    const string name_;
    const int r_;
};

shared_ptr<Figure> CreateFigure(istringstream& is) {
    string figure_type;
    is >> figure_type;
    shared_ptr<Figure> figure;
    if (figure_type == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        figure = make_shared<Triangle>(a, b, c);
    } else if (figure_type == "RECT") {
        int width, height;
        is >> width >> height;
        figure = make_shared<Rect>(width, height);
    } else if (figure_type == "CIRCLE") {
        int r;
        is >> r;
        figure = make_shared<Circle>(r);
    }

    return figure;
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}