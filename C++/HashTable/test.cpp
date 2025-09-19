#include "hash_table.h"
#include <climits>
#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using namespace hashtable;

#ifdef ALL_TEST
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
    int key = random(INT_MIN, INT_MAX);
    unsigned hash = map.hash(key);
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

#endif

TEST(ConstructorStandartHashTest, BasicConstructorZero) {
  hash_table<int, int> map;
  EXPECT_EQ(map.getSize(), 0);
  EXPECT_EQ(map.getCapacity(), 0);
  EXPECT_TRUE(map.empty());
}

TEST(ConstructorStandartHashTest, BasicConstructor) {
  hash_table<int, int> map(100);
  EXPECT_EQ(map.getSize(), 0);
  EXPECT_EQ(map.getCapacity(), 100);
}

TEST(IndexTest, Basic) {
  hash_table<int, int> map;
  std::vector<std::pair<int, int>> control{{1, 2}, {2, 3}, {3, 4}};
  map[1] = 2;
  map[2] = 3;
  map[3] = 4;
  map[4] = 5;
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_EQ(map.getCapacity(), 7);
  for (auto i : control) {
    EXPECT_EQ(map[i.first], i.second);
  }
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_EQ(map.getCapacity(), 7);
}

TEST(IndexTest, StringIndex) {
  hash_table<std::string, int> map;
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}};
  map["two"] = 2;
  map["three"] = 3;
  map["four"] = 4;
  map["five"] = 5;
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_EQ(map.getCapacity(), 7);
  for (auto i : control) {
    EXPECT_EQ(map[i.first], i.second);
  }
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_EQ(map.getCapacity(), 7);
}

TEST(ConstructorStandartHashTest, ListConstrucor) {
  hash_table<int, int> map({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_GE(map.getCapacity(), map.getSize());
  EXPECT_EQ(map[1], 2);
  EXPECT_EQ(map[2], 3);
  EXPECT_EQ(map[3], 4);
  EXPECT_EQ(map[4], 5);
  EXPECT_EQ(map[5], 0);
  EXPECT_EQ(map.getSize(), 5);
  EXPECT_GT(map.getCapacity(), map.getSize());
}

TEST(ConstructorStandartHashTest, CopyConstructor) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  hash_table<int, int> map2(map1);
  EXPECT_EQ(map1.getSize(), map2.getSize());
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (int i = 1; i <= 4; i++) {
    EXPECT_EQ(map1[i], map2[i]);
  }
}

TEST(ConstructorStandartHashTest, MoveConstructor) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  std::vector<std::pair<int, int>> control{{1, 2}, {2, 3}, {3, 4}, {4, 5}};
  hash_table<int, int> map2(std::move(map1));
  EXPECT_EQ(map1.getSize(), 0);
  EXPECT_EQ(map1.getCapacity(), 0);
  EXPECT_EQ(map2.getSize(), 4);
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  EXPECT_TRUE(map1.empty());
  for (int i = 1; i <= 4; i++) {
    EXPECT_EQ(map2[i], control[i - 1].second);
  }
}

TEST(AssignmentStandartHashTest, CopyAssignment) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  hash_table<int, int> map2{{1, 2}};
  map2 = map1;
  EXPECT_EQ(map1.getSize(), map2.getSize());
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (int i = 1; i <= 4; i++) {
    EXPECT_EQ(map1[i], map2[i]);
  }
}

TEST(AssignmentStandartHashTest, MoveAssignment) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  std::vector<std::pair<int, int>> control{{1, 2}, {2, 3}, {3, 4}, {4, 5}};
  hash_table<int, int> map2{{5, 10}};
  map2 = std::move(map1);
  EXPECT_EQ(map1.getSize(), 0);
  EXPECT_EQ(map1.getCapacity(), 0);
  EXPECT_EQ(map2.getSize(), 4);
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  EXPECT_TRUE(map1.empty());
  for (int i = 1; i <= 4; i++) {
    EXPECT_EQ(map2[i], control[i - 1].second);
  }
}

TEST(ConstructorStringHashTest, BasicConstructorZero) {
  hash_table<std::string, int> map;
  EXPECT_EQ(map.getSize(), 0);
  EXPECT_EQ(map.getCapacity(), 0);
  EXPECT_TRUE(map.empty());
}

TEST(ConstructorStringHashTest, BasicConstructor) {
  hash_table<std::string, int> map(3);
  EXPECT_EQ(map.getSize(), 0);
  EXPECT_EQ(map.getCapacity(), 3);
}

TEST(ConstructorStringHashTest, ListConstrucor) {
  hash_table<std::string, int> map{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_GE(map.getCapacity(), map.getSize());
  for (auto i : control) {
    EXPECT_EQ(map[i.first], i.second);
  }
  EXPECT_EQ(map.getSize(), 4);
  EXPECT_GT(map.getCapacity(), map.getSize());
}

TEST(ConstructorStringHashTest, CopyConstructor) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  hash_table<std::string, int> map2(map1);
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  EXPECT_EQ(map1.getSize(), map2.getSize());
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], map2[i.first]);
  }
}

TEST(ConstructorStringHashTest, MoveConstructor) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  hash_table<std::string, int> map2 = std::move(map1);
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  EXPECT_EQ(map1.getSize(), 0);
  EXPECT_EQ(map1.getCapacity(), 0);
  EXPECT_TRUE(map1.empty());
  EXPECT_EQ(map2.getSize(), 4);
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (auto i : control) {
    EXPECT_EQ(map2[i.first], i.second);
  }
}

TEST(AssignmentStringHashTest, CopyAssignment) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  hash_table<std::string, int> map2{{"aboba", 2}};
  map2 = map1;
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  EXPECT_EQ(map1.getSize(), map2.getSize());
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], map2[i.first]);
  }
}

TEST(AssignmentStringHashTest, MoveAssignment) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  hash_table<std::string, int> map2{{"aboba", 2}};
  map2 = std::move(map1);
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  EXPECT_EQ(map1.getSize(), 0);
  EXPECT_EQ(map1.getCapacity(), 0);
  EXPECT_TRUE(map1.empty());
  EXPECT_EQ(map2.getSize(), 4);
  EXPECT_GE(map2.getCapacity(), map2.getSize());
  for (auto i : control) {
    EXPECT_EQ(map2[i.first], i.second);
  }
}

TEST(DeleteStandartTest, OnlyDelete) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  std::vector<std::pair<int, int>> control{{1, 2}, {2, 3}, {3, 0}, {4, 0}};
  EXPECT_TRUE(map1.erase(3));
  EXPECT_TRUE(map1.erase(4));
  EXPECT_FALSE(map1.erase(4));
  EXPECT_EQ(map1.getSize(), 2);
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], i.second);
  }
}

TEST(DeleteStandartTest, WriteAfterDelete) {
  hash_table<int, int> map1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  std::vector<std::pair<int, int>> control{{1, 2}, {2, 3}, {3, 10}, {4, 10}};
  EXPECT_TRUE(map1.erase(3));
  EXPECT_TRUE(map1.erase(4));
  EXPECT_FALSE(map1.erase(4));
  EXPECT_EQ(map1.getSize(), 2);
  map1[3] = 10;
  map1[4] = 10;
  EXPECT_EQ(map1.getSize(), 4);
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], i.second);
  }
}

TEST(DeleteStringTest, OnlyDelete) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 0}, {"five", 0}};
  EXPECT_TRUE(map1.erase("four"));
  EXPECT_TRUE(map1.erase("five"));
  EXPECT_FALSE(map1.erase("four"));
  EXPECT_EQ(map1.getSize(), 2);
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], i.second);
  }
}

TEST(DeleteStringTest, WriteAfterDelete) {
  hash_table<std::string, int> map1{
      {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};
  std::vector<std::pair<std::string, int>> control{
      {"two", 2}, {"three", 3}, {"four", 10}, {"five", 10}};
  EXPECT_TRUE(map1.erase("four"));
  EXPECT_TRUE(map1.erase("five"));
  EXPECT_FALSE(map1.erase("four"));
  EXPECT_EQ(map1.getSize(), 2);
  map1["four"] = 10;
  map1["five"] = 10;
  EXPECT_EQ(map1.getSize(), 4);
  for (auto i : control) {
    EXPECT_EQ(map1[i.first], i.second);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
