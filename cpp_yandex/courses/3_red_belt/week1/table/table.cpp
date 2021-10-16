#include <vector>
#include <utility>
#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table

template<typename T>
class Table
{
public:
    Table(size_t row = 0, size_t col = 0): iRow(row), iCol(col)
    {
        if (iRow == 0 || iCol == 0) {
            return;
        }
        iTable.resize(iRow, vector<T>(iCol));
    }

    const vector<T>& operator[](size_t row) const
    {
        return iTable[row];
    }

    vector<T>& operator[](size_t row)
    {
        return iTable[row];
    }

    void Resize(size_t row, size_t col)
    {
        iRow = row;
        iCol = col;
        iTable.resize(row);
        for (auto& r : iTable) {
            r.resize(col);
        }
    }

    pair<size_t, size_t> Size() const
    {
        if (iRow == 0 || iCol == 0) {
            return { 0, 0 };
        }
        return { iRow, iCol };
    }

private:
    size_t iRow;
    size_t iCol;
    vector<vector<T>>iTable;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

void TestTable2() {
    Table<int> t(1, 1);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t[0][0], 42);
    t[2][3] = 25;
    ASSERT_EQUAL(t[2][3], 25);
    t.Resize(1, 1);
    ASSERT_EQUAL(t[0][0], 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    RUN_TEST(tr, TestTable2);
    return 0;
}