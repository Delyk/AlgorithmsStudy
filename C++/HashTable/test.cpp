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
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.FibonacciHash(i), map.FibonacciHash(i));
    unsigned hash = map.FibonacciHash(random(INT_MIN, INT_MAX));
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHashPhi) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.FibonacciHashPhi(i), map.FibonacciHashPhi(i));
    unsigned hash = map.FibonacciHashPhi(random(INT_MIN, INT_MAX));
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FNVHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.FNVHash(i), map.FNVHash(i));
    unsigned hash = map.FNVHash(random(INT_MIN, INT_MAX));
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, MurmurHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.MurmurHash(i), map.MurmurHash(i));
    unsigned hash = map.MurmurHash(random(INT_MIN, INT_MAX));
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, PersonHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.PersonHash64(str), map.PersonHash64(str));
    unsigned hash = map.PersonHash64(str);
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, ModStringHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.ModHash(str), map.ModHash(str));
    unsigned hash = map.ModHash(str);
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FNVStringHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.FNVHash(str), map.FNVHash(str));
    unsigned hash = map.FNVHash(str);
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, PoliminalHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.PoliminalHash(str), map.PoliminalHash(str));
    unsigned hash = map.PoliminalHash(str);
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, MurmurHashString) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.MurmurHash(str), map.MurmurHash(str));
    unsigned hash = map.MurmurHash(str);
    std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
