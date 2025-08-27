#include "hash_table.h"
#include <climits>
#include <gtest/gtest.h>
#include <string>
using namespace std;
using namespace hashtable;

static int random(int start, int end) {
  std::random_device rd;  // Устройство случайности
  std::mt19937 gen(rd()); // Генератор
  std::uniform_int_distribution<> dist(start, end); // Диапазон распределения
  return dist(gen);
}

static std::string random_string(unsigned size) {
  std::string str;
  for (unsigned int i = 0; i < size; i++) {
    str += random(-127, 127);
  }
  return str;
}

TEST(HashPrivateTest, SimpleHashMod) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.ModHash(i), map.ModHash(i));
    unsigned hash = map.ModHash(random(INT_MIN, INT_MAX));
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.FibonacciHash(i), map.FibonacciHash(i));
    unsigned hash = map.FibonacciHash(random(INT_MIN, INT_MAX));
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHashPhi) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.FibonacciHashPhi(i), map.FibonacciHashPhi(i));
    unsigned hash = map.FibonacciHashPhi(random(INT_MIN, INT_MAX));
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, PersonHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.PersonHash(str), map.PersonHash(str));
    unsigned hash = map.PersonHash(str);
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
