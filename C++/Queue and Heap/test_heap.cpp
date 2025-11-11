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
  binomial_heap<int> heap_init{5, 4, -1, 2, 3, 1, 0};
};

TEST_F(bin_heap, GetMin) { EXPECT_ANY_THROW(heap.getMin()); }

TEST_F(bin_heap, OneInsert) {
  heap.insert(10);
  EXPECT_EQ(heap.getMin(), 10);
}

TEST_F(bin_heap, InsertTest) {
  heap.insert(9);
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.getMin(), 2);
}
TEST_F(bin_heap, InsertTestMinFirst) {
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  heap.insert(4);
  heap.insert(5);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestMinLast) {
  heap.insert(5);
  heap.insert(4);
  heap.insert(3);
  heap.insert(2);
  heap.insert(1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestMinMiddle) {
  heap.insert(3);
  heap.insert(2);
  heap.insert(-1);
  heap.insert(4);
  heap.insert(5);
  EXPECT_EQ(heap.getMin(), -1);
}

TEST_F(bin_heap, InsertTestSomeEqual) {
  heap.insert(3);
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.insert(5);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestAllEqual) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestSomeEqualFirst) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestSomeEqualLast) {
  heap.insert(2);
  heap.insert(3);
  heap.insert(1);
  heap.insert(1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, InsertTestBig) {
  for (int i = 100; i >= -1; i--) {
    heap.insert(i);
  }
  EXPECT_EQ(heap.getMin(), -1);
}

TEST_F(bin_heap, ExtractEmpty) { EXPECT_ANY_THROW(heap.extractMin()); }

TEST_F(bin_heap, OneExtract) {
  heap.insert(10);
  EXPECT_FALSE(heap.empty());
  EXPECT_EQ(heap.extractMin(), 10);
  EXPECT_TRUE(heap.empty());
}

TEST_F(bin_heap, BasicExtract) {
  heap.insert(9);
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.extractMin(), 2);
  EXPECT_EQ(heap.getMin(), 3);
}

TEST_F(bin_heap, ExtractMinFirst) {
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  heap.insert(4);
  heap.insert(5);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.getMin(), 2);
}

TEST_F(bin_heap, ExtractMinLast) {
  heap.insert(5);
  heap.insert(4);
  heap.insert(3);
  heap.insert(2);
  heap.insert(1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.getMin(), 2);
}

TEST_F(bin_heap, ExtractSome) {
  heap.insert(3);
  heap.insert(2);
  heap.insert(-1);
  heap.insert(4);
  heap.insert(5);
  EXPECT_EQ(heap.extractMin(), -1);
  EXPECT_EQ(heap.extractMin(), 2);
  EXPECT_EQ(heap.extractMin(), 3);
  EXPECT_EQ(heap.getMin(), 4);
}

TEST_F(bin_heap, ExtractSomeEqual) {
  heap.insert(3);
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.insert(5);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.getMin(), 3);
}

TEST_F(bin_heap, ExtractAll) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_TRUE(heap.empty());
  EXPECT_ANY_THROW(heap.getMin());
}

TEST_F(bin_heap, ExtractSomeEqualFirst) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.getMin(), 2);
}

TEST_F(bin_heap, ExtractSomeEqualLast) {
  heap.insert(2);
  heap.insert(3);
  heap.insert(1);
  heap.insert(1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.extractMin(), 1);
  EXPECT_EQ(heap.getMin(), 2);
}

TEST_F(bin_heap, ExtractAllBig) {
  for (int i = 100; i >= -1; i--) {
    heap.insert(i);
  }
  for (int i = -1; i <= 100; i++) {
    EXPECT_EQ(heap.extractMin(), i);
  }
  EXPECT_TRUE(heap.empty());
}

TEST_F(bin_heap, decreaseKeyBasic) {
  heap_init.decrease_key(-10, heap_init.find(5));
  EXPECT_EQ(heap_init.extractMin(), -10);
  EXPECT_EQ(heap_init.extractMin(), 0);
  EXPECT_EQ(heap_init.getMin(), 1);
}

TEST_F(bin_heap, decreaseKeyEqual) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.decrease_key(-1, heap.find(1));
  EXPECT_EQ(heap.extractMin(), -1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, decreaseKeySameMid) {
  heap.insert(3);
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.insert(3);
  heap.decrease_key(-1, heap.find(1));
  EXPECT_EQ(heap.extractMin(), -1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, decreaseKeySameEnd) {
  heap.insert(3);
  heap.insert(2);
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.decrease_key(-1, heap.find(1));
  EXPECT_EQ(heap.extractMin(), -1);
  EXPECT_EQ(heap.getMin(), 1);
}

TEST_F(bin_heap, decreaseKeySameBegin) {
  heap.insert(1);
  heap.insert(1);
  heap.insert(1);
  heap.insert(3);
  heap.insert(2);
  heap.decrease_key(-1, heap.find(1));
  EXPECT_EQ(heap.extractMin(), -1);
  EXPECT_EQ(heap.getMin(), 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
