#include <string>
#include <list>
#include <array>
#include <algorithm>
#include "test_runner.h"

using namespace std;


class Editor {
 public:
    Editor()
        : cursor_(text_.begin())
        , current_buffer_size_(0)
    {
    }

    void Left()
    {
        if (cursor_ !=  text_.begin()) {
            --cursor_;
        }

    }

    void Right()
    {
        if (cursor_ != text_.end()) {
            ++cursor_;
        }
    }

    void Insert(char token)
    {
        cursor_ = text_.insert(cursor_, token);
        ++cursor_;
    }

    void Cut(size_t tokens = 1)
    {
        Copy(tokens);

        auto finish = cursor_;
        for (auto i = 0; finish != text_.end() && i < tokens; ++i) {
            ++finish;
        }

        if (finish != cursor_) {
            cursor_ = text_.erase(cursor_, finish);
        }
    }

    void Copy(size_t tokens = 1)
    {
        current_buffer_size_ = 0;

        auto finish = cursor_;
        size_t i = 0;
        for (i; finish != text_.end() && i < tokens; ++i) {
            ++finish;
        }

        if (finish != cursor_) {
            copy(cursor_, finish, buffer_.begin());
            current_buffer_size_ = i;
        }
    }

    void Paste()
    {
        for (size_t i = 0; i < current_buffer_size_; ++i) {
            Insert(buffer_[i]);
        }
    }

    string GetText() const
    {
        return string(text_.begin(), text_.end());
    }

private:
    list<char> text_;
    list<char>::iterator cursor_;
    size_t current_buffer_size_;
    array<char, 1'000'000> buffer_;
};


void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}