#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

class ExpressionValue: public Expression {
public:
  ExpressionValue(int value)
      : value_(value) {
  }

  int Evaluate() const override {
    return value_;
  }

  std::string ToString() const override {
    return to_string(value_);
  }

private:
  int value_;
};

class ExpressionSum : public Expression {
public:
  ExpressionSum(ExpressionPtr left, ExpressionPtr right)
      : left_(move(left))
      , right_(move(right)) {
  }

  int Evaluate() const override {
    return left_->Evaluate() + right_->Evaluate();
  }

  std::string ToString() const override {
    return "(" + left_->ToString() +  ")+(" + right_->ToString() + ")";
  }

private:
  ExpressionPtr left_;
  ExpressionPtr right_;
};

class ExpressionProduct : public Expression {
public:
  ExpressionProduct(ExpressionPtr left, ExpressionPtr right)
      : left_(move(left))
      , right_(move(right)) {
  }

  int Evaluate() const override {
    return left_->Evaluate() * right_->Evaluate();
  }

  std::string ToString() const override {
    return "(" + left_->ToString() + ")*(" + right_->ToString() + ")";
  }

private:
  ExpressionPtr left_;
  ExpressionPtr right_;
};

ExpressionPtr Value(int value) {
  return make_unique<ExpressionValue>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<ExpressionSum>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<ExpressionProduct>(move(left), move(right));
}

string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}