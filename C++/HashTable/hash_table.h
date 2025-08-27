#pragma once
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>
#include <initializer_list>
#include <random>
#include <string>
#include <vector>

namespace hashtable {
using ull = unsigned long long;
static const unsigned primes[100] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
    383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541};

static unsigned random(unsigned start, unsigned end) {
  std::random_device rd;  // Устройство случайности
  std::mt19937 gen(rd()); // Генератор
  std::uniform_int_distribution<> dist(start, end); // Диапазон распределения
  return dist(gen);
}

template <typename T, typename U> class hash_table {
public:
  ull prime;
  const double phi = 0.6180339887;
  std::vector<uint8_t> table;
  ull ModHash(ull);
  ull FibonacciHash(ull);
  ull FibonacciHashPhi(ull);
  ull PersonHash(std::string);
  struct element {
    T key;
    U data;
    bool occupied;
    element() : key(T{}), data(U{}), occupied(false){};
    void push(T, U);
    void clear();
  };
  std::vector<std::function<unsigned(T)>> hashes;
  unsigned long size; //Размер, т.е. кол-во элементов
  std::vector<element> array;
  unsigned hash(T) const; //Хэш функция
  hash_table(unsigned long = 0);
  hash_table(const hash_table &);
  hash_table(const hash_table &&) noexcept;
  hash_table(std::initializer_list<std::pair<T, U>>);
  hash_table &operator=(const hash_table &);
  hash_table &operator=(const hash_table &&) noexcept;
  U &operator[](T);
  void erase(T);
  bool empty() const;
  void clear();
  unsigned long getSize() const;
  unsigned long getCapacity() const;
};

template <typename T, typename U>
hash_table<T, U>::hash_table(unsigned long size) {
  prime = primes[random(0, 99)];
  array.resize(size);
}

template <typename T, typename U> ull hash_table<T, U>::ModHash(ull key) {
  return (key % prime) % size;
}

template <typename T, typename U> ull hash_table<T, U>::FibonacciHash(ull key) {
  ull f0 = 0;
  ull f1 = 1;
  ull hash = 0;
  while (key) {
    unsigned num = key % 10;
    key /= 10;
    ull f2 = f0 + f1;
    f0 = f1;
    f1 = f2;
    hash = (hash + num * f1) % array.size();
  }
  return hash;
}

template <typename T, typename U>
ull hash_table<T, U>::FibonacciHashPhi(ull key) {
  ull w = ULONG_LONG_MAX;
  ull A = static_cast<ull>(phi * w);
  return ((key * A) >> (sizeof(ull) * 8 - 64)) % array.size();
}

template <typename T, typename U>
ull hash_table<T, U>::PersonHash(std::string str) {
  if (str.empty()) {
    return 0;
  }
  if (table.empty()) {
    for (int i = 0; i < 256; i++) {
      table.push_back(i);
    }
    for (int i = 255; i > 0; i--) {
      int j = random(0, i);
      std::swap(table[i], table[j]);
    }
  }

  std::string hashed_str = str;
  ull hash64 = 0;
  for (unsigned long i = 0; i < sizeof(ull); i++) {
    unsigned char hash = 0;
    for (auto ch : hashed_str) {
      hash = table[(hash ^ static_cast<unsigned char>(ch)) % 256];
    }
    hash64 |= hash << ((7 - i) * 8);
    str[0] = (str[0] + 1) % 256;
  }
  return hash64 % array.size();
}

template <typename T, typename U>
unsigned long hash_table<T, U>::getCapacity() const {
  return this->array.size();
}
} // namespace hashtable
