#include "queue.hpp"
#include <gtest/gtest.h>
using namespace queues;

class QueueTest : public ::testing::Test {
protected:
  queue<int> q_default;
  queue<int> q_list{1, 2, 3};
};

TEST_F(QueueTest, DefaultConstructorCreatesEmptyQueue) {
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueTest, InitializerListConstructorCreatesQueue) {
  EXPECT_FALSE(q_list.empty());
  EXPECT_EQ(q_list.front(), 1);
}

TEST_F(QueueTest, CopyConstructorCopiesQueue) {
  queue<int> q_copy(q_list);
  EXPECT_EQ(q_copy.front(), q_list.front());
  EXPECT_FALSE(q_copy.empty());
}

TEST_F(QueueTest, MoveConstructorMovesQueue) {
  queue<int> q_temp{4, 5};
  queue<int> q_moved(std::move(q_temp));
  EXPECT_FALSE(q_moved.empty());
}

TEST_F(QueueTest, InitializerListAssignmentWorks) {
  queue<int> q;
  q = {7, 8, 9};
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 7);
}

TEST_F(QueueTest, CopyAssignmentWorks) {
  queue<int> q;
  q = q_list;
  EXPECT_EQ(q.front(), q_list.front());
}

TEST_F(QueueTest, MoveAssignmentWorks) {
  queue<int> q_temp{10, 11};
  queue<int> q;
  q = std::move(q_temp);
  EXPECT_FALSE(q.empty());
}

TEST_F(QueueTest, EnqueueAddsElement) {
  q_default.enqueue(42);
  q_default.enqueue(52);
  q_default.enqueue(1);
  q_default.enqueue(42);
  EXPECT_FALSE(q_default.empty());
  EXPECT_EQ(q_default.front(), 42);
}

TEST_F(QueueTest, DequeueReturnsElementsFIFO) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueTest, DequeueReturnsElementsFIFO2) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  EXPECT_EQ(q_default.dequeue(), 1);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  q_default.enqueue(1);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueTest, FrontReturnsCurrentFirstElement) {
  q_default.enqueue(5);
  EXPECT_EQ(q_default.front(), 5);
  q_default.enqueue(6);
  EXPECT_EQ(q_default.front(), 5);
  q_default.dequeue();
  EXPECT_EQ(q_default.front(), 6);
}

class QueueStackTest : public ::testing::Test {
protected:
  queue_stack<int> q_default;
  queue_stack<int> q_list{1, 2, 3};
};

TEST_F(QueueStackTest, DefaultConstructorCreatesEmptyQueue) {
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueStackTest, InitializerListConstructorCreatesQueue) {
  EXPECT_FALSE(q_list.empty());
  EXPECT_EQ(q_list.front(), 1);
}

TEST_F(QueueStackTest, CopyConstructorCopiesQueue) {
  queue_stack<int> q_copy(q_list);
  EXPECT_EQ(q_copy.front(), q_list.front());
  EXPECT_FALSE(q_copy.empty());
}

TEST_F(QueueStackTest, MoveConstructorMovesQueue) {
  queue_stack<int> q_temp{4, 5};
  queue_stack<int> q_moved(std::move(q_temp));
  EXPECT_FALSE(q_moved.empty());
}

TEST_F(QueueStackTest, InitializerListAssignmentWorks) {
  queue_stack<int> q;
  q = {7, 8, 9};
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 7);
}

TEST_F(QueueStackTest, CopyAssignmentWorks) {
  queue_stack<int> q;
  q = q_list;
  EXPECT_EQ(q.front(), q_list.front());
}

TEST_F(QueueStackTest, MoveAssignmentWorks) {
  queue_stack<int> q_temp{10, 11};
  queue_stack<int> q;
  q = std::move(q_temp);
  EXPECT_FALSE(q.empty());
}

TEST_F(QueueStackTest, EnqueueAddsElement) {
  q_default.enqueue(42);
  q_default.enqueue(52);
  q_default.enqueue(1);
  q_default.enqueue(42);
  EXPECT_FALSE(q_default.empty());
  EXPECT_EQ(q_default.front(), 42);
}

TEST_F(QueueStackTest, DequeueReturnsElementsFIFO) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueStackTest, DequeueReturnsElementsFIFO2) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  EXPECT_EQ(q_default.dequeue(), 1);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  q_default.enqueue(1);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_TRUE(q_default.empty());
}

TEST_F(QueueStackTest, FrontReturnsCurrentFirstElement) {
  q_default.enqueue(5);
  EXPECT_EQ(q_default.front(), 5);
  q_default.enqueue(6);
  EXPECT_EQ(q_default.front(), 5);
  q_default.dequeue();
  EXPECT_EQ(q_default.front(), 6);
}

class Deque : public ::testing::Test {
protected:
  deque<int> q_default;
  deque<int> q_list{1, 2, 3};
};

TEST_F(Deque, DefaultConstructorCreatesEmptyQueue) {
  EXPECT_TRUE(q_default.empty());
}

TEST_F(Deque, InitializerListConstructorCreatesQueue) {
  EXPECT_FALSE(q_list.empty());
  EXPECT_EQ(q_list.front(), 1);
}

TEST_F(Deque, CopyConstructorCopiesQueue) {
  deque<int> q_copy(q_list);
  EXPECT_EQ(q_copy.front(), q_list.front());
  EXPECT_FALSE(q_copy.empty());
}

TEST_F(Deque, MoveConstructorMovesQueue) {
  deque<int> q_temp{4, 5};
  deque<int> q_moved(std::move(q_temp));
  EXPECT_FALSE(q_moved.empty());
}

TEST_F(Deque, InitializerListAssignmentWorks) {
  deque<int> q;
  q = {7, 8, 9};
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 7);
}

TEST_F(Deque, CopyAssignmentWorks) {
  deque<int> q;
  q = q_list;
  EXPECT_EQ(q.front(), q_list.front());
}

TEST_F(Deque, MoveAssignmentWorks) {
  deque<int> q_temp{10, 11};
  deque<int> q;
  q = std::move(q_temp);
  EXPECT_FALSE(q.empty());
}

TEST_F(Deque, EnqueueAddsElement) {
  q_default.enqueue(42);
  q_default.enqueue(52);
  q_default.enqueue(1);
  q_default.enqueue(42);
  EXPECT_FALSE(q_default.empty());
  EXPECT_EQ(q_default.front(), 42);
}

TEST_F(Deque, DequeueReturnsElementsFIFO) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_TRUE(q_default.empty());
}

TEST_F(Deque, DequeueReturnsElementsFIFO2) {
  q_default.enqueue(1);
  q_default.enqueue(2);
  EXPECT_EQ(q_default.dequeue(), 1);
  q_default.enqueue(42);
  q_default.enqueue(52);
  EXPECT_EQ(q_default.dequeue(), 2);
  EXPECT_EQ(q_default.dequeue(), 42);
  q_default.enqueue(1);
  EXPECT_EQ(q_default.dequeue(), 52);
  EXPECT_EQ(q_default.dequeue(), 1);
  EXPECT_TRUE(q_default.empty());
}

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

TEST_F(Deque, FrontReturnsCurrentFirstElement) {
  q_default.enqueue(5);
  EXPECT_EQ(q_default.front(), 5);
  q_default.enqueue(6);
  EXPECT_EQ(q_default.front(), 5);
  q_default.dequeue();
  EXPECT_EQ(q_default.front(), 6);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
