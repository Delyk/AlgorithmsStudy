#include "arrays.hpp"
#include <gtest/gtest.h>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>
using namespace arrays;

// Тест типа
TEST(ElementTest, Basic) {
  universal test = 42;
  ASSERT_EQ(test.type(), typeid(int));
  ASSERT_THROW(universal_cast<float>(test), std::bad_typeid);
  ASSERT_THROW(universal_cast<int>(new universal()), std::bad_typeid);
  ASSERT_EQ(universal_cast<int>(test), 42);
}

// Тест копирования типа
TEST(ElementTest, Copy) {
  universal one = 42;
  universal two = one;
  ASSERT_EQ(universal_cast<int>(one), 42);
  ASSERT_EQ(universal_cast<int>(two), 42);
}

// Тест конструктора перемещения типа
TEST(ElementTest, MoveConstructor) {
  universal test = 42;
  universal test1 = std::move(test);
  ASSERT_ANY_THROW(universal_cast<int>(test));
  ASSERT_EQ(universal_cast<int>(test1), 42);
}

// Присваивание с копированием
TEST(ElementTest, AssignmentTest) {
  universal test = 42;
  universal test2 = 6.4;
  test = test2;

  EXPECT_EQ(universal_cast<double>(test), 6.4);
  EXPECT_EQ(universal_cast<double>(test2), 6.4);
}

// Move-присваивание
TEST(ElementTest, AssignmentMoveTest) {
  universal test = 42;
  universal test2 = 6.4f;
  test = std::move(test2);

  EXPECT_EQ(universal_cast<float>(test), 6.4f);
  EXPECT_ANY_THROW(universal_cast<float>(test2));
}

// Присваивание значения
TEST(ElementTest, AssignmentTypeTest) {
  universal test = 42;
  test = 5.2;

  EXPECT_EQ(universal_cast<double>(test), 5.2);
}

// Самоприсваивание
TEST(ElementTest, SelfAssignmentTest) {
  universal test = 42;
  test = test;

  EXPECT_EQ(universal_cast<int>(test), 42);
}

// Type тест
TEST(ElementTest, Type) {
  universal testF = 3.14f;
  universal testD = 5.16;
  universal testS = std::string("String");
  universal testU = std::vector<int>();

  ASSERT_EQ(testF.type(), typeid(float));
  ASSERT_EQ(testD.type(), typeid(double));
  ASSERT_EQ(testS.type(), typeid(std::string));
  ASSERT_EQ(testU.type(), typeid(std::vector<int>));
}

// Базовый тест массива
TEST(ArrayTest, Basic) {
  array<universal> arr = {3.14, std::string("String"), 42, true};

  ASSERT_EQ(arr.getSize(), 4);
  EXPECT_DOUBLE_EQ(universal_cast<double>(arr[0]), 3.14);
  EXPECT_EQ(universal_cast<std::string>(arr[1]), "String");
  EXPECT_EQ(universal_cast<int>(arr[2]), 42);
  EXPECT_EQ(universal_cast<bool>(arr[3]), true);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
