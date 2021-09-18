#include <iostream>
#include <string>

using namespace std;

struct Specialization {
	string value;

	explicit Specialization(const string& new_value) {
		value = new_value;
	}
};

struct Course {
	string value;

	explicit Course(const string& new_value) {
		value = new_value;
	}
};

struct Week {
	string value;

	explicit Week(const string& new_value) {
		value = new_value;
	}
};

struct LectureTitle {
  string specialization;
  string course;
  string week;

  LectureTitle(const Specialization& new_specialization,
  	  	  const Course& new_course, const Week& new_week) {
	  specialization = new_specialization.value;
	  course = new_course.value;
	  week = new_week.value;
  }
};

int
main()
{
LectureTitle title1(
    Specialization("C++"),
    Course("White belt"),
    Week("4th")
);

//LectureTitle title2("C++", "White belt", "4th");

//LectureTitle title3(string("C++"), string("White belt"), string("4th"));

//LectureTitle title4 = {"C++", "White belt", "4th"};

//LectureTitle title5 = {{"C++"}, {"White belt"}, {"4th"}};

/*
LectureTitle title6(
    Course("White belt"),
    Specialization("C++"),
    Week("4th")
);
*/

/*
LectureTitle title7(
    Specialization("C++"),
    Week("4th"),
    Course("White belt")
);
*/
}
