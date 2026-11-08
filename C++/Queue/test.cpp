#include "queue.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
using namespace queues;

template <typename T> class QueueTest : public ::testing::Test {
protected:
  T q_default;
  T q_list{1, 2, 3};
};

typedef ::testing::Types<queue<int>, queue_stack<int>, deque<int>> Impl;

TYPED_TEST_SUITE(QueueTest, Impl);

TYPED_TEST(QueueTest, DefaultConstructorCreatesEmptyQueue) {
  EXPECT_TRUE(this->q_default.empty());
}

TYPED_TEST(QueueTest, InitializerListConstructorCreatesQueue) {
  EXPECT_FALSE(this->q_list.empty());
  EXPECT_EQ(this->q_list.front(), 1);
}

TYPED_TEST(QueueTest, CopyConstructorCopiesQueue) {
  TypeParam q_copy(this->q_list);
  EXPECT_EQ(q_copy.front(), this->q_list.front());
  EXPECT_FALSE(q_copy.empty());
}

TYPED_TEST(QueueTest, MoveConstructorMovesQueue) {
  TypeParam q_temp{4, 5};
  TypeParam q_moved(std::move(q_temp));
  EXPECT_FALSE(q_moved.empty());
}

TYPED_TEST(QueueTest, InitializerListAssignmentWorks) {
  TypeParam q;
  q = {7, 8, 9};
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 7);
}

TYPED_TEST(QueueTest, CopyAssignmentWorks) {
  TypeParam q;
  q = this->q_list;
  EXPECT_EQ(q.front(), this->q_list.front());
}

TYPED_TEST(QueueTest, MoveAssignmentWorks) {
  TypeParam q_temp{10, 11};
  TypeParam q;
  q = std::move(q_temp);
  EXPECT_FALSE(q.empty());
}

TYPED_TEST(QueueTest, EnqueueAddsElement) {
  this->q_default.enqueue(42);
  this->q_default.enqueue(52);
  this->q_default.enqueue(1);
  this->q_default.enqueue(42);
  EXPECT_FALSE(this->q_default.empty());
  EXPECT_EQ(this->q_default.front(), 42);
}

TYPED_TEST(QueueTest, DequeueReturnsElementsFIFO) {
  this->q_default.enqueue(1);
  this->q_default.enqueue(2);
  this->q_default.enqueue(42);
  this->q_default.enqueue(52);
  EXPECT_EQ(this->q_default.dequeue(), 1);
  EXPECT_EQ(this->q_default.dequeue(), 2);
  EXPECT_EQ(this->q_default.dequeue(), 42);
  EXPECT_EQ(this->q_default.dequeue(), 52);
  EXPECT_TRUE(this->q_default.empty());
}

TYPED_TEST(QueueTest, DequeueReturnsElementsFIFO2) {
  this->q_default.enqueue(1);
  this->q_default.enqueue(2);
  EXPECT_EQ(this->q_default.dequeue(), 1);
  this->q_default.enqueue(42);
  this->q_default.enqueue(52);
  EXPECT_EQ(this->q_default.dequeue(), 2);
  EXPECT_EQ(this->q_default.dequeue(), 42);
  this->q_default.enqueue(1);
  EXPECT_EQ(this->q_default.dequeue(), 52);
  EXPECT_EQ(this->q_default.dequeue(), 1);
  EXPECT_TRUE(this->q_default.empty());
}

TYPED_TEST(QueueTest, FrontReturnsCurrentFirstElement) {
  this->q_default.enqueue(5);
  EXPECT_EQ(this->q_default.front(), 5);
  this->q_default.enqueue(6);
  EXPECT_EQ(this->q_default.front(), 5);
  this->q_default.dequeue();
  EXPECT_EQ(this->q_default.front(), 6);
}

class Deque : public ::testing::Test {
protected:
  deque<int> q_default;
  deque<int> q_list{1, 2, 3};
};

TEST_F(Deque, EnqueueFrontAddsElementAtFront) {
  q_default.enqueue_front(10);
  q_default.enqueue_front(20);
  EXPECT_EQ(q_default.back(), 10);
  EXPECT_EQ(q_default.front(), 20);
}

TEST_F(Deque, DequeueBackRemovesAndReturnsLastElement) {
  q_default.enqueue_front(5);
  q_default.enqueue_front(15);
  EXPECT_EQ(q_default.front(), 15);
  int last = q_default.dequeue_back();
  EXPECT_EQ(last, 5);
  int new_last = q_default.back();
  EXPECT_EQ(new_last, 15);
  EXPECT_EQ(q_default.front(), 15);
}

TEST_F(Deque, BackReturnsLastElementWithoutRemoving) {
  q_default.enqueue_front(7);
  q_default.enqueue_front(14);
  int last = q_default.back();
  EXPECT_EQ(last, 7);
  EXPECT_EQ(q_default.back(), last);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
