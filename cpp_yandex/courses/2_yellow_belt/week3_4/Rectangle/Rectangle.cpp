/*
Вам дано полное определение класса Rectangle
Пришлите заголовочный файл rectangle.h, содержащий объявление класса Rectangle. Это должен быть полноценный заголовочный файл, который можно использовать в большом проекте. В частности, в нём должна быть решена проблема двойного включения.
*/

#include "Rectangle.h"

Rectangle::Rectangle(int width, int height) : width_(width), height_(height) {}

int Rectangle::GetArea() const {
    return width_ * height_;
}

int Rectangle::GetPerimeter() const {
    return 2 * (width_ + height_);
}

int Rectangle::GetWidth() const { return width_; }
int Rectangle::GetHeight() const { return height_; }

int main() {
    return 0;
}
