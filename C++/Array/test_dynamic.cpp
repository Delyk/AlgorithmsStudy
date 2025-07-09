#include "arrays.hpp"
#include <gtest/gtest.h>
using namespace arrays;

// Выделение размера
TEST(BaseTest, SizeGen) {
  array<int> dynamic_array(10);
  EXPECT_EQ(dynamic_array.getSize(), 10);
  EXPECT_EQ(dynamic_array.getCapacity(), 20);
}

// Конструктор по умолчанию
TEST(BaseTest, Default) {
  array<int> dynamic_array;
  EXPECT_EQ(dynamic_array.getSize(), 0);
  EXPECT_EQ(dynamic_array.getCapacity(), 0);
}

// Список нулевой длины
TEST(BaseTest, NullList) {
  array<int> dynamic_array;
  dynamic_array = {};
  EXPECT_EQ(dynamic_array.getSize(), 0);
  EXPECT_EQ(dynamic_array.getCapacity(), 0);
  array<int> second_array{};
  EXPECT_EQ(dynamic_array.getSize(), 0);
  EXPECT_EQ(dynamic_array.getCapacity(), 0);
}

// Оператор индексирования
TEST(BaseTest, Index) {
  array<int> dynamic_array(5);
  dynamic_array[0] = 1;
  dynamic_array[3] = 9;
  EXPECT_EQ(dynamic_array[0], 1);
  EXPECT_EQ(dynamic_array[3], 9);
}

// Out of bounds
TEST(BaseTest, IndexException) {
  array<int> dynamic_array(10);
  EXPECT_ANY_THROW(dynamic_array[20]);
}

// Простое хранение данных
TEST(BaseTest, SimpleStore) {
  int static_array[5] = {1, 2, 3, 4, 5};
  array<int> dynamic_array = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(static_array[i], dynamic_array[i]);
  }
}

// Конструктор копирования
TEST(ConstructorsTest, CopyConstructor) {
  array<int> arr1 = {1, 2, 3};
  array<int> arr2(arr1);
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

// Конструктор копирования с изменением элемента
TEST(ConstructorsTest, CopyConstructorChange) {
  array<int> arr1 = {1, 2, 3};
  array<int> arr2(arr1);
  arr1[0] = 100;
  EXPECT_EQ(arr2[0], 1);
}

// Move-конструктор
TEST(ConstructorsTest, MoveConstructor) {
  array<int> arr1 = {1, 2, 3};
  array<int> control(arr1);
  array<int> arr2 = std::move(arr1);
  EXPECT_ANY_THROW(arr1[0]);
  EXPECT_EQ(arr1.getSize(), 0);
  for (t_size i = 0; i < arr2.getSize(); i++) {
    EXPECT_EQ(control[i], arr2[i]);
  }
}

// Присваивание
TEST(AssignmentTest, SimpleAssignment) {
  array<int> arr1{1, 2, 3};
  array<int> arr2;
  arr2 = arr1;
  EXPECT_EQ(arr1.getSize(), arr2.getSize());
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

// Присваивание 2
TEST(AssignmentTest, SimpleAssignment2) {
  array<int> arr1{1, 2, 3};
  array<int> arr2{1};
  arr2 = arr1;
  EXPECT_EQ(arr1.getSize(), arr2.getSize());
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

// Присваивание 3
TEST(AssignmentTest, SimpleAssignment3) {
  array<int> arr1{1, 2, 3};
  array<int> arr2{1, 2, 3, 4, 5, 6};
  arr2 = arr1;
  EXPECT_EQ(arr1.getSize(), arr2.getSize());
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

// Присваивание с независимым изменением
TEST(AssignmentTest, SimpleAssignmentChange) {
  array<int> arr1 = {1, 2, 3};
  array<int> arr2;
  arr2 = arr1;
  arr1[0] = 100;
  EXPECT_EQ(arr2[0], 1);
}

// Присваивание со списком
TEST(AssignmentTest, ListAssignment) {
  array<int> arr1;
  arr1 = {1, 2, 3};
  int control[3] = {1, 2, 3};
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], control[i]);
  }
}

// Присваивание со списком 2
TEST(AssignmentTest, ListAssignment2) {
  array<int> arr1 = {1, 2, 3};
  arr1 = {1, 2};
  int control[2] = {1, 2};
  EXPECT_EQ(arr1.getSize(), 2);
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], control[i]);
  }
}

// Присваивание со списком 3
TEST(AssignmentTest, ListAssignment3) {
  array<int> arr1{1, 2};
  arr1 = {1, 2, 3, 4};
  int control[4] = {1, 2, 3, 4};
  EXPECT_EQ(arr1.getSize(), 4);
  for (t_size i = 0; i < arr1.getSize(); i++) {
    EXPECT_EQ(arr1[i], control[i]);
  }
}

// Move-присваивание
TEST(AssignmentTest, MoveAssignment) {
  array<int> arr1 = {1, 2, 3};
  array<int> arr2;
  arr2 = std::move(arr1);
  int control[3] = {1, 2, 3};
  EXPECT_ANY_THROW(arr1[0]);
  EXPECT_EQ(arr1.getSize(), 0);
  for (t_size i = 0; i < arr2.getSize(); i++) {
    EXPECT_EQ(arr2[i], control[i]);
  }
}

// push_back тест
TEST(BaseTest, Push_Back) {
  array<int> dynamic_array;
  int array[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    dynamic_array.push_back(array[i]);
  }

  EXPECT_EQ(dynamic_array.getSize(), 5);
  EXPECT_GT(dynamic_array.getCapacity(), dynamic_array.getSize());
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(dynamic_array[i], array[i]);
  }
}
// push_back тест 2
TEST(BaseTest, Push_Back2) {
  array<int> dynamic_array(10);
  int array[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    dynamic_array.push_back(array[i]);
  }

  EXPECT_EQ(dynamic_array.getSize(), 15);
  EXPECT_GT(dynamic_array.getCapacity(), dynamic_array.getSize());
  for (t_size i = 10, j = 0; i < dynamic_array.getSize(); i++, j++) {
    EXPECT_EQ(dynamic_array[i], array[j]);
  }
}

// Тест самоповтора
TEST(BaseTest, SelfRepeat) {
  array<int> arr = {1, 2, 3};
  arr = arr;
  int control[3] = {1, 2, 3};
  EXPECT_EQ(arr.getSize(), 3);
  EXPECT_EQ(arr.getCapacity(), 6);
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(arr[i], control[i]);
  }
}

// Тест двумерного массива
TEST(BaseTest, Matrix) {
  array<array<int>> array = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  int control[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      EXPECT_EQ(array[i][j], control[i][j]);
    }
  }
}

// Big тест
TEST(BaseTest, BigArray) {
  array<int> array(10000);
  EXPECT_EQ(array.getSize(), 10000);
  EXPECT_GT(array.getCapacity(), array.getSize());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
