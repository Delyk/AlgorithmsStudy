#include "list.hpp"
#include <gtest/gtest.h>
using namespace list;

TEST(SimpleTest, Basic) {
  double_list<int> list{1, 2, 3, 4, 5};
  int control[5]{1, 2, 3, 4, 5};

  int j = 0;
  for (auto i : list) {
    EXPECT_EQ(control[j], i);
    j++;
  }
}

// Пустой конструктор
TEST(ConstructorTest, Empty) {
  double_list<int> list;
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
}

// Конструктор одного числа
TEST(ConstructorTest, OneConstructor) {
  double_list<int> list{42};
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_EQ(*list.begin(), 42);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
