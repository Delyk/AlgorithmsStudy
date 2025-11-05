#include "heap.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <vector>

template <typename T> class HeapTest : public ::testing::Test {
protected:
  T heap;
  T heap_init{1, 2, 3};
};

typedef ::testing::Types<binary_heap<int>> Impl;

TYPED_TEST_SUITE(HeapTest, Impl);

TYPED_TEST(HeapTest, ListConstructor) {
  EXPECT_EQ(this->heap_init.extractMax(), 3);
}

TYPED_TEST(HeapTest, BasicHeap) {
  TypeParam heap{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(heap.extractMax(), 9);
}

TYPED_TEST(HeapTest, CopyConstructor) {
  TypeParam copy{this->heap_init};
  EXPECT_EQ(copy.extractMax(), 3);
}

TYPED_TEST(HeapTest, MoveConstructor) {
  TypeParam tmp{7, 8, 9};
  TypeParam modev{std::move(tmp)};
  EXPECT_EQ(modev.extractMax(), 9);
}

TYPED_TEST(HeapTest, AssignmentOperatorInitializerList) {
  this->heap = {10, 11, 12};
  EXPECT_EQ(this->heap.extractMax(), 12);
}

TYPED_TEST(HeapTest, CopyAssignmentOperator) {
  TypeParam copy;
  copy = this->heap_init;
  EXPECT_EQ(copy.extractMax(), 3);
}

TYPED_TEST(HeapTest, MoveAssignmentOperator) {
  TypeParam temp{13, 14, 15};
  TypeParam moved;
  moved = std::move(temp);
  EXPECT_EQ(moved.extractMax(), 15);
}

TYPED_TEST(HeapTest, InsertTest) {
  this->heap.insert(10);
  this->heap.insert(20);
  this->heap.insert(15);
  EXPECT_EQ(this->heap.extractMax(), 20);
  EXPECT_EQ(this->heap.extractMax(), 15);
  EXPECT_EQ(this->heap.extractMax(), 10);
}

TYPED_TEST(HeapTest, InreaseKey) {
  this->heap.insert(5);
  this->heap.insert(7);
  this->heap.increase_key(0, 10);
  EXPECT_EQ(this->heap.extractMax(), 10);
}

TEST(HeapSort, Heapsort) {
  binary_heap<int> heap;
  std::vector<int> data = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  heap.heapsort(data);
  for (size_t i = 1; i < data.size(); i++) {
    EXPECT_LE(data[i - 1], data[i]);
  }
}

class bin_heap : public ::testing::Test {
protected:
  binomial_heap<int> heap;
};

TEST_F(bin_heap, test_name) {
  heap.insert(9);
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.getMin(), 2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
