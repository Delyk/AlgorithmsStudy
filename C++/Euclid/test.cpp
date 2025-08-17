#include "euclid.h"
#include <gtest/gtest.h>
using namespace std;

// Тесты для классического алгоритма Евклида (вычитания)
TEST(EuclidClassicTest, BasicTests) {
  EXPECT_EQ(euclid::euclid_classic(48, 18), 6);
  EXPECT_EQ(euclid::euclid_classic(7, 13), 1);
  EXPECT_EQ(euclid::euclid_classic(0, 5), 5);
  EXPECT_EQ(euclid::euclid_classic(5, 0), 5);
  EXPECT_EQ(euclid::euclid_classic(0, 0), 0); // в зависимости от реализации
}

// Тесты для алгоритма Евклида через остаток от деления
TEST(EuclidRemainderTest, BasicTests) {
  EXPECT_EQ(euclid::euclid_remainder(48, 18), 6);
  EXPECT_EQ(euclid::euclid_remainder(7, 13), 1);
  EXPECT_EQ(euclid::euclid_remainder(0, 5), 5);
  EXPECT_EQ(euclid::euclid_remainder(5, 0), 5);
  EXPECT_EQ(euclid::euclid_remainder(0, 0), 0);
}

// Тесты для рекурсивного алгоритма Евклида
TEST(EuclidRecursiveTest, BasicTests) {
  EXPECT_EQ(euclid::euclid_recursive(48, 18), 6);
  EXPECT_EQ(euclid::euclid_recursive(7, 13), 1);
  EXPECT_EQ(euclid::euclid_recursive(0, 5), 5);
  EXPECT_EQ(euclid::euclid_recursive(5, 0), 5);
  EXPECT_EQ(euclid::euclid_recursive(0, 0), 0);
}

// Тесты для алгоритма Евклида с эллипсисом — проверяем НОД нескольких чисел
TEST(EuclidElipsisTest, MultipleArguments) {
  EXPECT_EQ(euclid::euclid_elipsis(2, 48, 18), 6);
  EXPECT_EQ(euclid::euclid_elipsis(3, 48, 18, 30), 6);
  EXPECT_EQ(euclid::euclid_elipsis(4, 7, 13, 19, 31), 1);
  EXPECT_EQ(euclid::euclid_elipsis(1, 42), 42);
  EXPECT_EQ(euclid::euclid_elipsis(0), 0); // если предусмотрено
}

// Тесты для расширенного алгоритма Евклида (коэффициенты Безу)
TEST(EuclidBesuTest, BezoutIdentity) {
  int a = 48, b = 18;
  auto [x, y] = euclid::euclid_Besu(a, b);
  int gcd = euclid::euclid_remainder(a, b);
  EXPECT_EQ(a * x + b * y, gcd);

  a = 101;
  b = 103;
  std::tie(x, y) = euclid::euclid_Besu(a, b);
  gcd = euclid::euclid_remainder(a, b);
  EXPECT_EQ(a * x + b * y, gcd);

  a = 0;
  b = 5;
  std::tie(x, y) = euclid::euclid_Besu(a, b);
  gcd = euclid::euclid_remainder(a, b);
  EXPECT_EQ(a * x + b * y, gcd);

  a = 5;
  b = 0;
  std::tie(x, y) = euclid::euclid_Besu(a, b);
  gcd = euclid::euclid_remainder(a, b);
  EXPECT_EQ(a * x + b * y, gcd);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
