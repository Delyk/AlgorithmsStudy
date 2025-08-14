#include "SomeRecursiveFunctions.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
using namespace recursive;
TEST(RecursiveVectorTests, SumBasic) {
  std::vector<int> v{1, 2, 3, 4, 5};
  EXPECT_EQ(recursive::sum(v), 15);
  EXPECT_EQ(recursive::sum(std::vector<int>{}), 0);
}

TEST(RecursiveVectorTests, CountBasic) {
  std::vector<int> v{10, 20, 30};
  EXPECT_EQ(recursive::count(v), 3);
  EXPECT_EQ(recursive::count(std::vector<int>{}), 0);
}

TEST(RecursiveVectorTests, MaxBasic) {
  std::vector<int> v{5, 3, 8, 2, 7};
  EXPECT_EQ(recursive::max(v), 8);

  std::vector<int> single{42};
  EXPECT_EQ(recursive::max(single), 42);

  std::vector<int> empty;
  EXPECT_THROW(recursive::max(empty), std::invalid_argument);
}

TEST(RecursiveVectorTests, MinBasic) {
  std::vector<int> v{5, 3, 8, 2, 7};
  EXPECT_EQ(recursive::min(v), 2);

  std::vector<int> single{42};
  EXPECT_EQ(recursive::min(single), 42);

  std::vector<int> empty;
  EXPECT_THROW(recursive::min(empty), std::invalid_argument);
}

TEST(RecursiveVariadicTests, SumVariadic) {
  EXPECT_EQ(recursive::sum(5, 1, 2, 3, 4, 5), 15);
  EXPECT_EQ(recursive::sum(0), 0); // Сумма пустого набора
}

TEST(RecursiveVariadicTests, CountVariadic) {
  EXPECT_EQ(recursive::count(3, 10, 20, 30), 3);
  EXPECT_EQ(recursive::count(0), 0);
}

TEST(RecursiveVariadicTests, MaxVariadic) {
  EXPECT_EQ(recursive::max(4, 1, 5, 2, 4), 5);
  EXPECT_EQ(recursive::max(1, 100), 100);
}

TEST(RecursiveVariadicTests, MinVariadic) {
  EXPECT_EQ(recursive::min(4, 1, 5, 2, 4), 1);
  EXPECT_EQ(recursive::min(1, 100), 100);
}
TEST(RecursiveVariadicTests, SumVariadicEmpty) {
  EXPECT_EQ(recursive::sum(0), 0);
}

TEST(RecursiveVariadicTests, CountVariadicEmpty) {
  EXPECT_EQ(recursive::count(0), 0);
}

TEST(RecursiveVariadicTests, MaxVariadicEmpty) {
  // В реализации max для пустого набора ожидается исключение
  EXPECT_THROW(recursive::max(0), std::invalid_argument);
}

TEST(RecursiveVariadicTests, MinVariadicEmpty) {
  // Аналогично для min
  EXPECT_THROW(recursive::min(0), std::invalid_argument);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
