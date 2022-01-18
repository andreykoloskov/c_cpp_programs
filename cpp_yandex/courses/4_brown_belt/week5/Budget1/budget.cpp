#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <utility>

using namespace std;

class Date {
public:
  explicit Date(int year = 0, int month = 0, int day = 0)
      : year_(year), month_(month), day_(day), timestamp_(0) {
    if (year != 0 || month != 0 || day != 0) {
      timestamp_ = CalculateDayCount();
    }
  }

  Date(const Date& date) {
    CopyDate(date);
  }

  Date& operator=(const Date& date) {
    if (&date != this) {
      CopyDate(date);
    }
    return *this;
  }

  int GetYear() const { return year_; }
  int GetMonth() const { return month_; }
  int GetDay() const { return day_; }
  int GetTimeStamp() const { return timestamp_; }

private:
  void CopyDate(const Date& date) {
    year_ = date.year_;
    month_ = date.month_;
    day_ = date.day_;
    timestamp_ = CalculateDayCount();
  }

  int CalculateDayCount() {
    tm t1 = {0, 0, 0, day_, month_ - 1, year_ - 1900 };
    t1.tm_isdst = -1;
    auto tp1 = chrono::system_clock::from_time_t(mktime(&t1));

    tm t2 = {0, 0, 0, 0, 0, 2000 - 1900 };
    t2.tm_isdst = -1;
    auto tp2 = chrono::system_clock::from_time_t(mktime(&t2));

    return chrono::duration_cast<chrono::hours>(tp1 - tp2).count() / 24;
  }

private:
  int year_ = 0;
  int month_ = 0;
  int day_ = 0;
  int timestamp_ = 0;
};

istream& operator>>(istream& in, Date& date) {
  int year, month, day;
  char delim;
  in >> year >> delim >> month >> delim >> day;
  date = Date{ year, month, day };
  return in;
}

class BudgetCalculator {
public:
  enum class Command {
    ComputeIncome
    , Earn
    , PayTax
    , Spend
  };

  struct FullCommand {
    Command command;
    Date from;
    Date to;
    double value;
  };

  BudgetCalculator() {
    incomes.fill(0.0);
    costs.fill(0.0);
  }

  void ApplyCommand(ostream& os, const FullCommand& command) {
    switch (command.command) {
      case Command::ComputeIncome:
        os.precision(25);
        os << ComputeIncome(command) << endl;
        break;
      case Command::Earn:
        ComputeEarn(command);
        break;
      case Command::PayTax:
        ComputePayTax(command);
        break;
      case Command::Spend:
        ComputeSpend(command);
        break;
      default:
        break;
    }
  }

  FullCommand ParseCommand(istream& in, const string& str) const {
    FullCommand fullCommand;
    string command;
    in >> command >> fullCommand.from >> fullCommand.to;

    if (command == "Earn") {
      fullCommand.command = Command::Earn;
      in >> fullCommand.value;
    }
    else if (command == "ComputeIncome") {
      fullCommand.command = Command::ComputeIncome;
    }
    else if (command == "PayTax") {
      fullCommand.command = Command::PayTax;
      in >> fullCommand.value;
    }
    else if (command == "Spend") {
      fullCommand.command = Command::Spend;
      in >> fullCommand.value;
    }

    return fullCommand;
  }

private:
  double ComputeIncome(const FullCommand& command) {
    auto from = command.from.GetTimeStamp();
    auto to = command.to.GetTimeStamp();
    double incom = 0, cost = 0;
    incom = accumulate(&incomes[from], &incomes[to] + 1, incom);
    cost = accumulate(&costs[from], &costs[to] + 1, cost);
    return incom - cost;
  }

  void ComputeEarn(const FullCommand& command) {
    auto from = command.from.GetTimeStamp();
    auto to = command.to.GetTimeStamp();
    double day_incom = command.value / (static_cast<double>(to) - from + 1.0);
    transform(&incomes[from], &incomes[to] + 1, &incomes[from],
        [day_incom](double a) -> double {
          return a + day_incom;
        });
  }

  void ComputePayTax(const FullCommand& command) {
    auto from = command.from.GetTimeStamp();
    auto to = command.to.GetTimeStamp();
    transform(&incomes[from], &incomes[to] + 1, &incomes[from],
        [&command](double a) -> double {
          return a * (100.0 - command.value) / 100.0;
        });
  }

  void ComputeSpend(const FullCommand& command) {
    auto from = command.from.GetTimeStamp();
    auto to = command.to.GetTimeStamp();
    double day_spend = command.value / (static_cast<double>(to) - from + 1.0);
    transform(&costs[from], &costs[to] + 1, &costs[from],
        [day_spend](double a) -> double {
          return a + day_spend;
        });
  }

private:
  std::array<double, 100 * 12 * 31 + 1> incomes;
  std::array<double, 100 * 12 * 31 + 1> costs;
};


int main() {
  BudgetCalculator budget;
  int count;
  cin >> count;
  for (int i = 0; i < count; ++i) {
    string str;
    getline(cin, str);
    budget.ApplyCommand(cout, budget.ParseCommand(cin, str));
  }

}