#include "stack.h"
#include <gtest/gtest.h>
using namespace stack;

//Тесты стека на основе массива
//Тест простого заполения
TEST(PushTest, BasicPush) {
  array_stack<int> stack;
  EXPECT_TRUE(stack.isEmpty());
  stack.push(1);
  stack.push(2);
  stack.push(3);
  EXPECT_EQ(stack.peek(), 3);
  EXPECT_EQ(stack.getSize(), 3);
  EXPECT_FALSE(stack.isEmpty());
}

// Тест переполнения
TEST(PushTest, Overflow) {
  array_stack<int, 2> stack;
  stack.push(1);
  stack.push(2);
  EXPECT_ANY_THROW(stack.push(3));
  EXPECT_EQ(stack.getSize(), 2);
  EXPECT_FALSE(stack.isEmpty());
  stack.print();
}

// Tест pop
TEST(PopTest, BasicPop) {
  array_stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  EXPECT_EQ(stack.pop(), 3);
  EXPECT_EQ(stack.getSize(), 2);
  EXPECT_EQ(stack.pop(), 2);
  EXPECT_EQ(stack.getSize(), 1);
  EXPECT_FALSE(stack.isEmpty());
  EXPECT_EQ(stack.pop(), 1);
  EXPECT_EQ(stack.getSize(), 0);
  EXPECT_ANY_THROW(stack.pop());
  EXPECT_TRUE(stack.isEmpty());
  stack.push(42);
  EXPECT_FALSE(stack.isEmpty());
  EXPECT_EQ(stack.getSize(), 1);
  EXPECT_EQ(stack.pop(), 42);
  EXPECT_EQ(stack.getSize(), 0);
  EXPECT_TRUE(stack.isEmpty());
}

//Тест peek
TEST(PeekTest, Basic) {
  array_stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  EXPECT_EQ(stack.peek(), 3);
  EXPECT_EQ(stack.getSize(), 3);
  stack.pop();
  EXPECT_EQ(stack.peek(), 2);
  EXPECT_EQ(stack.getSize(), 2);
  stack.pop();
  stack.pop();
  EXPECT_ANY_THROW(stack.peek());
}

// Тесты стека на основе списка
TEST(ListStackTest, IsEmptyInitially) {
  list_stack<int> stack;
  EXPECT_TRUE(stack.isEmpty());
  EXPECT_EQ(stack.getSize(), 0);
}

// Тест push и peek
TEST(ListStackTest, PushAndPeek) {
  list_stack<int> stack;
  stack.push(10);
  EXPECT_FALSE(stack.isEmpty());
  EXPECT_EQ(stack.getSize(), 1);
  EXPECT_EQ(stack.peek(), 10);
}

// Тест pop
TEST(ListStackTest, PushPop) {
  list_stack<int> stack;
  stack.push(5);
  stack.push(15);
  EXPECT_EQ(stack.peek(), 15);
  EXPECT_EQ(stack.pop(), 15);
  EXPECT_EQ(stack.peek(), 5);
  EXPECT_EQ(stack.pop(), 5);
  EXPECT_TRUE(stack.isEmpty());
}

// Тест исключений
TEST(ListStackTest, PushPopExceptions) {
  list_stack<int> stack;
  EXPECT_ANY_THROW(stack.pop());
  EXPECT_ANY_THROW(stack.peek());
}

// Тест getMax и getMin при find_min_max = true
TEST(ListStackTest, MinMaxTracking) {
  list_stack<int> stack(true);
  stack.push(10);
  EXPECT_EQ(stack.getMax(), 10);
  EXPECT_EQ(stack.getMin(), 10);

  stack.push(5);
  EXPECT_EQ(stack.getMax(), 10);
  EXPECT_EQ(stack.getMin(), 5);

  stack.push(20);
  EXPECT_EQ(stack.getMax(), 20);
  EXPECT_EQ(stack.getMin(), 5);

  stack.pop(); // удаляем 20
  EXPECT_EQ(stack.getMax(), 10);
  EXPECT_EQ(stack.getMin(), 5);

  stack.pop(); // удаляем 5
  EXPECT_EQ(stack.getMax(), 10);
  EXPECT_EQ(stack.getMin(), 10);
}

// Тест getMax и getMin при find_min_max = false
TEST(ListStackTest, MinMaxWithoutTracking) {
  list_stack<int> stack;
  stack.push(10);
  EXPECT_ANY_THROW(stack.getMax());
  EXPECT_ANY_THROW(stack.getMin());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
