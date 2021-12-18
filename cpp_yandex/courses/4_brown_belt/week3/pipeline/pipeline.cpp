#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
};

istream& operator>>(istream& in, Email& email) {
  getline(in, email.from);
  getline(in, email.to);
  getline(in, email.body);
  return in;
}

ostream& operator<<(ostream& out, Email& email) {
  out << email.from << '\n' << email.to << '\n' << email.body << '\n';
  return out;
}

class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (next_) {
      next_->Process(move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next) {
    if (next_) {
      next_->SetNext(move(next));
    }
    else {
      next_ = move(next);
    }
  }

private:
  unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
  // реализуйте класс
  Reader(istream& in) : in_(in) {
  }

  void Process(unique_ptr<Email> email) override {
    PassOn(move(email));
  }

  void Run() override {
    Email email;
    while (in_ >> email) {
      //Передавать дальше
      Process(make_unique<Email>(email));
    }
  }

private:
  istream& in_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  // реализуйте класс
  Filter(Function func) : func_(func) {
  }

  void Process(unique_ptr<Email> email) override {
    if (func_(*email)) {
      PassOn(move(email));
    }
  }

private:
  Function func_;
};


class Copier : public Worker {
public:
  // реализуйте класс
  Copier(string recipient) : recipient_(recipient) {
  }

  void Process(unique_ptr<Email> email) override {
    Email e = *email;
    PassOn(move(email));
    if (e.to != recipient_) {
      PassOn(make_unique<Email>(Email{ e.from, recipient_, e.body }));
    }
  }

private:
  string recipient_;
};


class Sender : public Worker {
public:
  // реализуйте класс
  Sender(ostream& out) : out_(out) {
  }

  void Process(unique_ptr<Email> email) override {
    out_ << *email;
    PassOn(move(email));
  }

private:
  ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in) : worker_(make_unique<Reader>(in)) {
  }

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter) {
    worker_->SetNext(make_unique<Filter>(filter));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient) {
    worker_->SetNext(make_unique<Copier>(recipient));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out) {
    worker_->SetNext(make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build() {
    return move(worker_);
  }

private:
  unique_ptr<Worker> worker_;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}