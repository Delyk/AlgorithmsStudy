#include "list.hpp"
#include "gtest/gtest.h"
#include <algorithm>
#include <gtest/gtest.h>
using namespace list;

// Пустой конструктор
TEST(ConstructorTest, EmptyConstructor) {
  double_list<int> list;
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_TRUE(list.empty());
}

// Конструктор одного числа
TEST(ConstructorTest, OneConstructor) {
  double_list<int> list{42};
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_EQ(*list.begin(), 42);
}

//Конструктор копирования
TEST(ConstructorTest, CopyConstructor) {
  double_list<int> one{1, 2, 3, 4, 5};
  double_list<int> two = one;
  EXPECT_EQ(two.getSize(), one.getSize());
  for (auto it_one = one.begin(), it_two = two.begin();
       it_one != one.end() && it_two != two.end(); it_one++, it_two++) {
    EXPECT_EQ(*it_one, *it_two);
  }
}

//Конструктор перемещения
TEST(ConstructorTest, MoveConstructor) {
  int control[]{1, 2, 3, 4, 5};
  double_list<int> one{1, 2, 3, 4, 5};
  double_list<int> two = std::move(one);
  EXPECT_EQ(one.getSize(), 0);
  EXPECT_EQ(one.begin(), one.end());
  EXPECT_TRUE(one.empty());
  int i = 0;
  for (auto j : two) {
    EXPECT_EQ(control[i++], j);
  }
}

//Конструктор списком
TEST(ConstructorTest, ListConstructor) {
  double_list<int> list{1, 2, 3, 4, 5};
  int control[5]{1, 2, 3, 4, 5};

  int j = 0;
  for (auto i : list) {
    EXPECT_EQ(control[j++], i);
  }

  for (auto it = list.end(); it != list.begin(); it--) {
    std::cout << *it << std::endl;
  }
  std::cout << *list.begin() << std::endl;
}

// Присваивание списка
TEST(AssignmentTest, ListAssignment) {
  int control[]{-1, -2, -3, -4, -5};
  double_list<int> list{1, 2, 3};
  list = {-1, -2, -3, -4, -5};
  EXPECT_EQ(list.getSize(), 5);
  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i++], j);
  }
}

// Присваивание с копированием
TEST(AssignmentTest, CopyAssignment) {
  double_list<int> one = {1, 2};
  double_list<int> two{-1, 0, -2};
  two = one;
  EXPECT_EQ(two.getSize(), one.getSize());
  for (auto it_one = one.begin(), it_two = two.begin();
       it_one != one.end() && it_two != two.end(); it_one++, it_two++) {
    EXPECT_EQ(*it_one, *it_two);
  }
}

// Присваивание с перемещением
TEST(AssignmentTest, MoveAssignment) {
  int control[]{1, 2};
  double_list<int> one = {1, 2};
  double_list<int> two{-1, 0, -2};
  two = std::move(one);
  EXPECT_EQ(one.getSize(), 0);
  EXPECT_EQ(one.begin(), one.end());
  EXPECT_TRUE(one.empty());
  int i = 0;
  for (auto j : two) {
    EXPECT_EQ(control[i++], j);
  }
}

// Добавить в конец
TEST(InsertTest, Push) {
  int control[]{1, 2, 3};
  double_list<int> list;
  list.push(1);
  list.push(2);
  list.push(3);
  EXPECT_EQ(list.getSize(), 3);
  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i++], j);
  }
}

//Удалить из конца
TEST(InsertTest, Pop) {
  double_list<int> list{1, 2, 3};
  EXPECT_EQ(3, list.pop());
  EXPECT_EQ(2, list.pop());
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_EQ(1, list.pop());
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_TRUE(list.empty());
}

// Добавить в начало
TEST(InsertTest, Unshift) {
  int control[]{3, 2, 1};
  double_list<int> list;
  list.unshift(1);
  list.unshift(2);
  list.unshift(3);
  EXPECT_EQ(list.getSize(), 3);
  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i++], j);
  }
}

//Удалить из начала
TEST(InsertTest, Shift) {
  double_list<int> list{1, 2, 3};
  EXPECT_EQ(1, list.shift());
  EXPECT_EQ(2, list.shift());
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_EQ(3, list.shift());
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_TRUE(list.empty());
}

//Сортировка пустого списка
TEST(SortTest, EmptySort) {
  double_list<int> list;
  list.sort();
  EXPECT_TRUE(list.empty());
}

//Сортировка одного элемента
TEST(SortTest, SortSingleElement) {
  double_list<int> list{42};
  list.sort();
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_EQ(*list.begin(), 42);
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_FALSE(list.empty());
}

//Сортировка
TEST(SortTest, SortBasic1) {
  int control[]{1, 2, 3};
  double_list<int> list{2, 1, 3};
  list.sort();
  EXPECT_EQ(list.getSize(), 3);
  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i++], j);
  }

  for (auto it = list.end(); it != list.begin(); it--) {
    EXPECT_EQ(control[i--], *it);
  }
  EXPECT_EQ(control[0], *list.begin());
}

TEST(SortTest, SortBasic2) {
  int control[]{-2, -1, 0, 1, 2, 3, 4, 5};
  double_list<int> list{5, 1, -2, 3, 4, 0, -1, 2};
  list.sort();
  EXPECT_EQ(list.getSize(), 8);
  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i++], j);
  }
  for (auto it = list.end(); it != list.begin(); it--) {
    EXPECT_EQ(control[i--], *it);
  }
  EXPECT_EQ(control[0], *list.begin());
}

// Уже отсортировано
TEST(SortTest, SortAlreadySorted) {
  int control[] = {1, 2, 3, 4, 5};
  double_list<int> list{1, 2, 3, 4, 5};
  list.sort();

  int i = 0;
  for (auto val : list) {
    EXPECT_EQ(val, control[i++]);
  }
  for (auto it = list.end(); it != list.begin(); it--) {
    EXPECT_EQ(control[i--], *it);
  }
  EXPECT_EQ(control[0], *list.begin());
}

// Одинаковые элементы
TEST(SortTest, SortAllEqual) {
  int control[]{7, 7, 7, 7};
  double_list<int> list{7, 7, 7, 7};
  list.sort();

  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
  for (auto it = list.end(); it != list.begin(); it--) {
    EXPECT_EQ(control[i--], *it);
  }
  EXPECT_EQ(control[0], *list.begin());
}

//Тест производительности сортировки
TEST(SortTest, SortLargeList) {
  const int N = 10000;
  double_list<int> bigList;
  std::vector<int> vec;

  for (int i = N; i > 0; --i) {
    bigList.push(i);
    vec.push_back(i);
  }

  bigList.sort();
  std::sort(vec.begin(), vec.end());

  int i = 0;
  for (auto val : bigList) {
    EXPECT_EQ(val, vec[i++]);
  }

  auto vec_it = vec.rbegin();
  auto list_it = bigList.end();
  for (; vec_it != vec.rend() && list_it != bigList.end();
       vec_it++, list_it--) {
    EXPECT_EQ(*vec_it, *list_it);
  }
  EXPECT_EQ(*vec.begin(), *bigList.begin());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
