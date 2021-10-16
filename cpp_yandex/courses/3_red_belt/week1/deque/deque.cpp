#include <vector>
#include <stdexcept>
//#include "test_runner.h"

using namespace std;

template<typename T>
class Deque
{
public:
	Deque() {}
	
	bool Empty() const { return front.empty() && back.empty(); }
	size_t Size() const { return front.size() + back.size(); }

	const T& ReturnElement(size_t index) const
	{
        return (index < front.size()) ? front[front.size() - index - 1] : back[index - front.size()];
	}

	T& ReturnElement(size_t index)
	{
        return (index < front.size()) ? front[front.size() - index - 1] : back[index - front.size()];
	}

	const T& operator[](size_t index) const { return ReturnElement(index); }
	T& operator[](size_t index) { return ReturnElement(index); }

	const T& At(size_t index) const
	{
		if (index >= Size()) {
			throw out_of_range("Out of range");
		}
        return ReturnElement(index);
	}

	T& At(size_t index)
	{
		if (index >= Size()) {
			throw out_of_range("Out of range");
		}
        return ReturnElement(index);
	}

	const T& BorderElement(const vector<T>& first, const vector<T>& second) const
	{
		if (first.empty()) {
			return second.at(0);
		}
		return first.back();
	}

	T& BorderElement(vector<T>& first, vector<T>& second)
	{
		if (first.empty()) {
			return second.at(0);
		}
		return first.back();
	}

	const T& Front() const { return BorderElement(front, back); }
	T& Front() { return BorderElement(front, back); }
	const T& Back() const { return BorderElement(back, front); }
	T& Back() { return BorderElement(back, front); }
	void PushFront(const T& value) { front.push_back(value); }
	void PushBack(const T& value) { back.push_back(value); }

private:
	vector<T> front;
	vector<T> back;
};

//void TestDeque() {
//	Deque<int> d;
//	ASSERT_EQUAL(d.Empty(), true);
//	ASSERT_EQUAL(d.Size(), 0u);
//	d.PushFront(2);
//	d.PushFront(1);
//	d.PushBack(3);
//	d.PushBack(4);
//	ASSERT_EQUAL(d.Empty(), false);
//	ASSERT_EQUAL(d.Size(), 4u);
//	ASSERT_EQUAL(d.Front(), 1u);
//	ASSERT_EQUAL(d.Back(), 4u);
//	ASSERT_EQUAL(d[0], 1u);
//	ASSERT_EQUAL(d[1], 2u);
//	ASSERT_EQUAL(d[2], 3u);
//	ASSERT_EQUAL(d[3], 4u);
//
//	ASSERT_EQUAL(d.At(1), 2u);
//	ASSERT_EQUAL(d.At(2), 3u);
//
//	try {
//		ASSERT_EQUAL(d.At(4), 3u);
//	}
//	catch (out_of_range& e) {
//		cout << e.what() << endl;
//	}
//}

int main()
{
	//TestRunner tr;
	//RUN_TEST(tr, TestDeque);
	return 0;
}