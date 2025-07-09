#include "binary_search.h"
#include <gtest/gtest.h>
#include <vector>
using namespace std;

// Поиск первого
TEST(BinaryTest, First) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search(arr, 1), 0);
}

// Поиск в середине
TEST(BinaryTest, Middle) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search(arr, 3), 2);
}

// Поиск последнего
TEST(BinaryTest, Last) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search(arr, 5), 4);
}

// Отсутствует
TEST(BinaryTest, NotFound) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search(arr, 6), -1);
  EXPECT_EQ(binary_search(arr, 0), -1);
}

// Пустой массив
TEST(BinaryTest, Empty) {
  vector<int> arr;
  EXPECT_EQ(binary_search(arr, 1), -1);
}

// Массив из одного элемента
TEST(BinaryTest, Single) {
  vector<int> arr = {1};
  EXPECT_EQ(binary_search(arr, 1), 0);
  EXPECT_EQ(binary_search(arr, 10), -1);
  EXPECT_EQ(binary_search(arr, -10), -1);
}

// Повторяющиеся элементы
TEST(BinaryTest, Copyes) {
  vector<int> arr = {1, 1, 1, 1, 1};
  int result = binary_search(arr, 1);
  EXPECT_GE(result, 0);
  EXPECT_LE(result, 4);
}

// Большой массив
TEST(BinaryTest, BigArray) {
  vector<int> arr(1000);
  for (int i = 0; i < 1000; i++) {
    arr[i] = i * 2;
  }
  EXPECT_EQ(binary_search(arr, 500 * 2), 500);
  EXPECT_EQ(binary_search(arr, 999 * 2), 999);
  EXPECT_EQ(binary_search(arr, -1), -1);
  EXPECT_EQ(binary_search(arr, 1000 * 2), -1);
}

// Тестирование рекурсивного алгоритма

// Поиск первого
TEST(RecursiveTest, First_Recursive) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_recursive(arr, 1), 0);
}

// Поиск в середине
TEST(RecursiveTest, Middle_Recursive) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_recursive(arr, 3), 2);
}

// Поиск последнего
TEST(RecursiveTest, Last_Recursive) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_recursive(arr, 5), 4);
}

// Отсутствует
TEST(RecursiveTest, NotFound_Recursive) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_recursive(arr, 6), -1);
  EXPECT_EQ(binary_search_recursive(arr, 0), -1);
}

// Пустой массив
TEST(RecursiveTest, Empty_Recursive) {
  vector<int> arr;
  EXPECT_EQ(binary_search_recursive(arr, 1), -1);
}

// Массив из одного элемента
TEST(RecursiveTest, Single_Recursive) {
  vector<int> arr = {1};
  EXPECT_EQ(binary_search_recursive(arr, 1), 0);
  EXPECT_EQ(binary_search_recursive(arr, 10), -1);
  EXPECT_EQ(binary_search_recursive(arr, -10), -1);
}

// Повторяющиеся элементы
TEST(RecursiveTest, Copyes_Recursive) {
  vector<int> arr = {1, 1, 1, 1, 1};
  int result = binary_search_recursive(arr, 1);
  EXPECT_GE(result, 0);
  EXPECT_LE(result, 4);
}

// Большой массив
TEST(RecursiveTest, BigArray_Recursive) {
  vector<int> arr(1000);
  for (int i = 0; i < 1000; i++) {
    arr[i] = i * 2;
  }
  EXPECT_EQ(binary_search_recursive(arr, 500 * 2), 500);
  EXPECT_EQ(binary_search_recursive(arr, 999 * 2), 999);
  EXPECT_EQ(binary_search_recursive(arr, -1), -1);
  EXPECT_EQ(binary_search_recursive(arr, 1000 * 2), -1);
}

// Тесты диапазона

// Простой тест
TEST(RangeTest, Simple) {
  vector<int> arr = {1, 2, 3, 3, 3, 4, 5};
  EXPECT_EQ(binary_search_count(arr, 3), 3);
}

// Единичное вхождение
TEST(RangeTest, SingleEnter) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_count(arr, 3), 1);
}

// Массив одного элемента
TEST(RangeTest, SizeOne) {
  vector<int> arr = {1};
  EXPECT_EQ(binary_search_count(arr, 1), 1);
}

// Пустой массив
TEST(RangeTest, Empty) {
  vector<int> arr;
  EXPECT_EQ(binary_search_count(arr, 1), -1);
}

// Отсутствует
TEST(RangeTest, NotFound) {
  vector<int> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(binary_search_count(arr, 0), -1);
}

// Только дубли
TEST(RangeTest, Copyes) {
  vector<int> arr = {1, 1, 1, 1, 1};
  EXPECT_EQ(binary_search_count(arr, 1), 5);
}

// Дубли у левой границы
TEST(RangeTest, Left) {
  vector<int> arr = {1, 1, 2, 3, 4};
  EXPECT_EQ(binary_search_count(arr, 1), 2);
}

// Дубли у правой границы
TEST(RangeTest, Right) {
  vector<int> arr = {1, 2, 2, 3, 3};
  EXPECT_EQ(binary_search_count(arr, 3), 2);
}

// Большой массив
TEST(RangeTest, BigArray) {
  vector<int> arr(1000);
  int index_first = 500, index_last = 505;

  for (int i = 0; i < 1000; i++) {
    if ((i >= index_first) && (i < index_last)) {
      arr[i] = index_first * 2;
    } else {
      arr[i] = i * 2;
    }
  }

  EXPECT_EQ(binary_search_count(arr, index_first * 2), 5);
  EXPECT_EQ(binary_search_count(arr, 0), 1);
  EXPECT_EQ(binary_search_count(arr, 999 * 2), 1);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
