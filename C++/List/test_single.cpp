#include "list.h"
#include <algorithm>
#include <gtest/gtest.h>
using namespace list;

TEST(SimpleTest, Basic) {
  single_list<int> list = {1, 2, 3};
  for (auto i : list) {
    std::cout << i << std::endl;
  }
}

// Пустой конструктор
TEST(ConstructorTest, EmptyConstructor) {
  single_list<int> list{};
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_TRUE(list.empty());
}

// Конструктор одного элемента
TEST(ConstructorTest, OneConstructor) {
  single_list<int> list = 42;
  EXPECT_EQ(list.getSize(), 1);
  EXPECT_EQ(*list.begin(), 42);
}

// Конструктор списка
TEST(ConstructorTest, ListConstructor) {
  single_list<int> list{1, 2, 3, 4};
  int control[4]{1, 2, 3, 4};
  EXPECT_EQ(list.getSize(), 4);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Конструктор копирования
TEST(ConstructorTest, CopyConstructor) {
  single_list<int> list{1, 2, 3, 4};
  single_list<int> list1{list};

  int control[4]{1, 2, 3, 4};
  EXPECT_EQ(list.getSize(), 4);
  int i = 0;
  for (int j : list1) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Конструктор пермещения
TEST(ConstructorTest, MoveConstructor) {
  single_list<int> list{1, 2, 3, 4};
  single_list<int> list1 = std::move(list);

  int control[4]{1, 2, 3, 4};
  EXPECT_EQ(list1.getSize(), 4);
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_TRUE(list.empty());
  int i = 0;
  for (int j : list1) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Оператор присваивания списка
TEST(AssignmentTest, ListAssignment) {
  single_list<int> list{1, 2, 3, 4};
  list = {1, 2};

  int control[]{1, 2};
  EXPECT_EQ(list.getSize(), 2);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Присваивание с копированием
TEST(AssignmentTest, CopyAssignment) {
  single_list<int> list{1, 2, 3, 4};
  single_list<int> list1{1, 2};
  list = list1;

  int control[]{1, 2};
  EXPECT_EQ(list.getSize(), 2);
  EXPECT_EQ(list1.getSize(), 2);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Присваивание с пермещением
TEST(AssignmentTest, MoveAssignment) {
  single_list<int> list{1, 2, 3, 4};
  single_list<int> list1{1, 2};
  list = std::move(list1);

  int control[]{1, 2};
  EXPECT_EQ(list.getSize(), 2);
  EXPECT_EQ(list1.getSize(), 0);
  EXPECT_EQ(list1.begin(), list1.end());
  EXPECT_TRUE(list1.empty());
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Push тест
TEST(InsertDeleteTest, Push) {
  single_list<int> list;
  list.push(1);
  list.push(2);
  list.push(3);
  list.push(4);

  int control[]{1, 2, 3, 4};
  EXPECT_EQ(list.getSize(), 4);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Pop тест
TEST(InsertDeleteTest, Pop) {
  int control[]{1, 2};
  single_list<int> list;
  list.push(1);
  list.push(2);
  list.push(3);
  list.push(4);

  EXPECT_EQ(list.pop(), 4);
  EXPECT_EQ(list.pop(), 3);
  EXPECT_EQ(list.getSize(), 2);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
  EXPECT_EQ(list.pop(), 2);
  EXPECT_EQ(list.pop(), 1);
  EXPECT_ANY_THROW(list.pop());
  EXPECT_EQ(list.getSize(), 0);
}

// unshift тест
TEST(InsertDeleteTest, Unshift) {
  int control[]{1, 2};
  single_list<int> list;
  list.unshift(2);
  list.unshift(1);

  EXPECT_EQ(list.getSize(), 2);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Shift тест
TEST(InsertDeleteTest, Shift) {
  int control[]{2};
  single_list<int> list;
  list.unshift(2);
  list.unshift(1);
  list.shift();

  EXPECT_EQ(list.getSize(), 1);
  int i = 0;
  for (int j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
  list.shift();
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_ANY_THROW(list.shift());
}

// Итератор
TEST(IteratorTest, BasicIteration) {
  single_list<int> list{1, 2, 3, 4};

  auto print = [](const int &n) { std::cout << n << std::endl; };
  std::for_each(list.begin(), list.end(), print);
  auto it = list.begin();
  EXPECT_TRUE(it == list.begin());
  EXPECT_EQ(*it, 1);
  it++;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  it++;
  it++;
  EXPECT_EQ(it, list.end());
}

// Сортировка
TEST(SortTest, Sort) {
  int control[]{1, 2, 3, 4, 5};
  int control1[]{-3, 1, 4, 6, 6, 8};
  single_list<int> list{5, 4, 3, 2, 1};
  single_list<int> list1{-3, 6, 6, 4, 1, 8};

  list.sort();
  list1.sort();

  int i = 0;
  for (auto j : list) {
    EXPECT_EQ(control[i], j);
    i++;
  }
  i = 0;
  for (auto j : list1) {
    EXPECT_EQ(control1[i], j);
    i++;
  }
}

// Сортировка пустого списка
TEST(SortTest, SortEmpty) {
  single_list<int> list;

  list.sort();

  EXPECT_TRUE(list.empty());
}

// Cортировка одного элемента
TEST(SortTest, SortSingleElement) {
  single_list<int> single{42};
  single.sort();
  auto it = single.begin();
  ASSERT_NE(it, single.end());
  EXPECT_EQ(*it, 42);
  ++it;
  EXPECT_EQ(it, single.end());
}

// Уже отсортировано
TEST(SortTest, SortAlreadySorted) {
  int control[] = {1, 2, 3, 4, 5};
  single_list<int> sortedList{1, 2, 3, 4, 5};
  sortedList.sort();

  int i = 0;
  for (auto val : sortedList) {
    EXPECT_EQ(val, control[i++]);
  }
}

// Одинаковые элементы
TEST(SortTest, SortAllEqual) {
  int control[]{7, 7, 7, 7};
  single_list<int> equalList{7, 7, 7, 7};
  equalList.sort();

  int i = 0;
  for (auto j : equalList) {
    EXPECT_EQ(control[i], j);
    i++;
  }
}

// Сортировка большого списка
TEST(SortTest, SortLargeList) {
  const int N = 10000;
  single_list<int> bigList;
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
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
