#include <iostream>
#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

bool IsPointInRectangle(Point point, Size size) {
  return point.x >= 0 
      && point.x < size.width
      && point.y >= 0 
      && point.y < size.height;
}

class Shape : public IShape {
public:
  void SetPosition(Point position) override {
    position_ = position;
  }

  Point GetPosition() const override {
    return position_;
  }

  void SetSize(Size size) override {
    size_ = size;
  }

  Size GetSize() const override {
    return size_;
  }

  void SetTexture(std::shared_ptr<ITexture> texture) override {
    texture_ = texture;
  }

  ITexture* GetTexture() const override {
    return texture_.get();
  }

  void Draw(Image& image) const override {
    for (int y = 0; y < image.size(); ++y) {
      for (int x = 0; x < image[y].size(); ++x) {
        if (IsPointInShape(Point{ x - position_.x, y - position_.y }, size_)) {
          if (texture_ && IsPointInRectangle(Point{ x - position_.x, y - position_.y }, texture_->GetSize())) {
            image[y][x] = texture_->GetImage()[y - position_.y][x - position_.x];
          }
          else {
            image[y][x] = '.';
          }
        }
      }
    }
  }

private:
  virtual bool IsPointInShape(Point point, Size size) const = 0;

private:
  Point position_;
  Size size_;
  shared_ptr<ITexture> texture_;
};

class Rectangle : public Shape {
public:
  std::unique_ptr<IShape> Clone() const {
    return make_unique<Rectangle>(*this);
  }
  
private:
  bool IsPointInShape(Point point, Size size) const {
    return IsPointInRectangle(point, size);
  }
};

class Ellipse : public Shape {
public:
  std::unique_ptr<IShape> Clone() const override {
    return make_unique<Ellipse>(*this);
  }

private:
  bool IsPointInShape(Point point, Size size) const {
    return IsPointInEllipse(point, size);
  }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	switch (shape_type) {
	case ShapeType::Rectangle:
		return make_unique<Rectangle>();
	case ShapeType::Ellipse:
		return make_unique<Ellipse>();
	}
	return nullptr;
}