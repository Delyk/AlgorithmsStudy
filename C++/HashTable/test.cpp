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
    ull hashi = map.ModHash(i) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.ModHash(random(INT_MIN, INT_MAX)) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    ull hashi = map.FibonacciHash(i) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash =
        map.FibonacciHash(random(INT_MIN, INT_MAX)) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FibonacciHashPhi) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    ull hashi = map.FibonacciHashPhi(i) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash =
        map.FibonacciHashPhi(random(INT_MIN, INT_MAX)) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FNVHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    ull hashi = map.FNVHash(i) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.FNVHash(random(INT_MIN, INT_MAX)) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, MurmurHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    ull hashi = map.MurmurHash(i) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.MurmurHash(random(INT_MIN, INT_MAX)) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, UniversalHash) {
  hash_table<int, int> map(1000);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(map.hash(i), map.hash(i));
    unsigned hash = map.hash(random(INT_MIN, INT_MAX));
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, PersonHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    ull hashi = map.PersonHash64(str) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.PersonHash64(str) % map.array.size();
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, ModStringHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    ull hashi = map.ModHash(str) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.ModHash(str) % map.array.size();
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, FNVStringHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    ull hashi = map.FNVHash(str) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.FNVHash(str) % map.array.size();
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, PoliminalHash) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    ull hashi = map.PoliminalHash(str) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.PoliminalHash(str) % map.array.size();
    // std::cout << str << " " << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, MurmurHashString) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    ull hashi = map.MurmurHash(str) % map.array.size();
    EXPECT_EQ(hashi, hashi);
    unsigned hash = map.MurmurHash(str) % map.array.size();
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

TEST(HashPrivateTest, UniversalHashString) {
  hash_table<std::string, int> map(1000);
  for (int i = 0; i < 100; i++) {
    std::string str = random_string(i);
    EXPECT_EQ(map.hash(str), map.hash(str));
    unsigned hash = map.hash(str);
    // std::cout << hash << std::endl;
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, map.getCapacity());
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
