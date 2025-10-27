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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
