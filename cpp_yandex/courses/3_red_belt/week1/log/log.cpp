#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
  explicit Logger(ostream& output_stream) : os(output_stream) {
  }

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file= value; }

  void Log(const string& message) { os << message; }
  
  bool isLogLine() { return log_line; }
  bool isLogFile() { return log_file; }

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
};

#define LOG(logger, message) {             \
    ostringstream os;                      \
    if (logger.isLogFile()) {              \
        os << __FILE__;                    \
        if (logger.isLogLine()) {          \
            os << ":" << __LINE__;         \
        }                                  \
        os << " ";                         \
    }                                      \
    else if (logger.isLogLine()) {         \
        os << "Line " << __LINE__ << " ";  \
    }                                      \
    os << message << endl;                 \
    logger.Log(os.str());                  \
}

void TestLog() {
/* ��� ��������� ����-������ � ���� ������ ��� ����� ����������� ����������
 * ������ ����� � ��������� �������� (��. ���������� expected ����). ����
 * �� ��������� �����-�� ��� ���� ������� TestLog, �� ��� ������ ����� ��������,
 * � ��� ���� �������� ������. ��� ��������.
 *
 * ����� ����� ��������, �� ���������� ����������� ������ #line
 * (http://en.cppreference.com/w/cpp/preprocessor/line), ������� ���������
 * �������������� ����� ������, � ����� ��� �����. ��������� ���, ������
 * ����� ������ ������� TestLog ����� ����������� ���������� �� ����, �����
 * ��� �� ��������� ����� ���*/
#line 1 "logger.cpp"

  ostringstream logs;
  Logger l(logs);
  LOG(l, "hello");

  l.SetLogFile(true);
  LOG(l, "hello");

  l.SetLogLine(true);
  LOG(l, "hello");

  l.SetLogFile(false);
  LOG(l, "hello");

  string expected = "hello\n";
  expected += "logger.cpp hello\n";
  expected += "logger.cpp:10 hello\n";
  expected += "Line 13 hello\n";
  ASSERT_EQUAL(logs.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLog);
}