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
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace hashtable {
using ull = unsigned long long;

// Функция для генерации случайного числа
static unsigned random(unsigned start, unsigned end) {
  std::random_device rd;  // Устройство случайности
  std::mt19937 gen(rd()); // Генератор
  std::uniform_int_distribution<> dist(start, end); // Диапазон распределения
  return dist(gen);
}

// Обычная универсальная хэш таблица
template <typename T, typename U> class hash_table {
#ifdef ALL_TEST
public:
#else
private:
#endif
  const unsigned primes[100] = {
      2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
      53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
      127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
      199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
      283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
      383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
      467, 479, 487, 491, 499, 503, 509, 521, 523, 541};
  const double phi = 0.6180339887;
  const ull FNV_prime = 16777619;
  ull prime;          // Простое число для алгоритмов
  unsigned long size; // Размер, т.е. кол-во элементов
  double load_factor = 0.0; //Коэффициент заполнения
  // Элемент хранящий ключ и данные
  struct element {
    T key;
    U data;
    bool occupied;
    element() : key(T{}), data(U{}), occupied(false) {}
    element(T key, U val) : key(key), data(val), occupied(true) {}
    void clear() {
      key = T{};
      data = U{};
      occupied = false;
    }
    ~element() { this->clear(); };
  };
  // Массив для хранения
  std::vector<element> array;
  // Набор хэш функций
  ull ModHash(ull);
  ull FibonacciHash(ull);
  ull FibonacciHashPhi(ull);
  ull FNVHash(ull);
  ull MurmurHash(ull);
  // Массив хэш функций
  std::vector<std::function<ull(ull)>> hashes{
      // bind связывает функцию с экземпляром класса
      // также bind позволяет "зафиксировать" параметр функции, как будто он по
      // умолчанию
      std::bind(static_cast<ull (hash_table::*)(ull)>(&hash_table::ModHash),
                this, std::placeholders::_1),
      std::bind(
          static_cast<ull (hash_table::*)(ull)>(&hash_table::FibonacciHash),
          this, std::placeholders::_1),
      std::bind(
          static_cast<ull (hash_table::*)(ull)>(&hash_table::FibonacciHashPhi),
          this, std::placeholders::_1),
      std::bind(static_cast<ull (hash_table::*)(ull)>(&hash_table::FNVHash),
                this, std::placeholders::_1),
      std::bind(static_cast<ull (hash_table::*)(ull)>(&hash_table::MurmurHash),
                this, std::placeholders::_1)};
  std::vector<uint8_t> table;
  void createTable();
  // Хэши для строк
  ull PersonHash64(std::string);
  uint8_t PersonHash8(std::string);
  ull ModHash(std::string);
  ull FNVHash(std::string);
  ull PoliminalHash(std::string);
  ull MurmurHash(std::string);
  std::vector<std::function<ull(std::string)>> hashes_str{
      std::bind(&hash_table::PersonHash64, this, std::placeholders::_1),
      std::bind(
          static_cast<ull (hash_table::*)(std::string)>(&hash_table::ModHash),
          this, std::placeholders::_1),
      std::bind(
          static_cast<ull (hash_table::*)(std::string)>(&hash_table::FNVHash),
          this, std::placeholders::_1),
      std::bind(&hash_table::PoliminalHash, this, std::placeholders::_1),
      std::bind(static_cast<ull (hash_table::*)(std::string)>(
                    &hash_table::MurmurHash),
                this, std::placeholders::_1)};
  // Хэширование
  ull hash(const T &); // Хэш функция
  ull probing(const T &);
  void actualSize();
  void rehashing();

public:
  // Конструкторы
  hash_table(unsigned long = 0);
  hash_table(const hash_table &);
  hash_table(hash_table &&) noexcept;
  hash_table(std::initializer_list<element>);
  // Операторы присваивания
  hash_table &operator=(const hash_table &);
  hash_table &operator=(hash_table &&) noexcept;
  // Оператор получения значения по ключу
  U &operator[](T);
  // Удаление элемента
  bool erase(const T &);
  bool empty() const;
  void clear();
  unsigned long getSize() const;
  unsigned long getCapacity() const;
};

/* ########################
 * Методы общей хэш-таблицы
 * ########################
 */
// Конструкторы
template <typename T, typename U>
hash_table<T, U>::hash_table(unsigned long size) : size(0) {
  if (table.empty()) {
    createTable();
  }
  prime = primes[random(0, 99)];
  array.resize(size);
}

template <typename T, typename U>
hash_table<T, U>::hash_table(const hash_table<T, U> &right) {
  prime = right.prime;
  table = right.table;
  for (auto i : right.array) {
    if (i.occupied)
      (*this)[i.key] = i.data;
  }
}

template <typename T, typename U>
hash_table<T, U>::hash_table(hash_table<T, U> &&right) noexcept {
  prime = right.prime;
  table = right.table;
  for (auto i : right.array) {
    if (i.occupied)
      (*this)[i.key] = i.data;
  }
  right.clear();
}

template <typename T, typename U>
hash_table<T, U>::hash_table(std::initializer_list<element> list) {
  if (table.empty()) {
    createTable();
  }
  prime = primes[random(0, 99)];
  array.resize(list.size());
  for (auto i : list) {
    (*this)[i.key] = i.data;
  }
}

// Операторы присваивания
template <typename T, typename U>
hash_table<T, U> &hash_table<T, U>::operator=(const hash_table<T, U> &right) {
  this->clear();
  prime = right.prime;
  table = right.table;
  for (auto i : right.array) {
    if (i.occupied)
      (*this)[i.key] = i.data;
  }
  return *this;
}

template <typename T, typename U>
hash_table<T, U> &
hash_table<T, U>::operator=(hash_table<T, U> &&right) noexcept {
  this->clear();
  prime = right.prime;
  table = right.table;
  for (auto i : right.array) {
    if (i.occupied)
      (*this)[i.key] = i.data;
  }
  right.clear();
  return *this;
}

//Оператор индексирования
template <typename T, typename U> U &hash_table<T, U>::operator[](T key) {
  actualSize();

  ull hash_idx = hash(key);
  if (array[hash_idx].key != key && array[hash_idx].occupied) {
    hash_idx = probing(key);
  }
  if (!array[hash_idx].occupied) {
    array[hash_idx].key = key;
    array[hash_idx].occupied = true;
    size++;
  }
  return array[hash_idx].data;
}

template <typename T, typename U> ull hash_table<T, U>::probing(const T &key) {
  ull hash_idx = 0;
  ull start_idx = hash_idx;
  ull m = array.size() > 1 ? array.size() - 1 : 1;
  ull step = 1 + MurmurHash(key) % m;
  while (true) {
    if (array[hash_idx].key == key) {
      return hash_idx;
    }
    hash_idx = (hash_idx + step) % array.size();
    if (hash_idx == start_idx) {
      break;
    }
  }
  hash_idx = 0;
  start_idx = hash_idx;
  while (true) {
    if (!array[hash_idx].occupied) {
      return hash_idx;
    }
    hash_idx = (hash_idx + step) % array.size();
    if (hash_idx == start_idx) {
      throw std::runtime_error("Hash is full");
    }
  }
}

template <typename T, typename U> void hash_table<T, U>::actualSize() {
  size = 0;
  for (auto i : array) {
    if (i.occupied) {
      size++;
    }
  }
  if (array.size()) {
    load_factor = size / static_cast<double>(array.size());
    if (load_factor >= 0.7) {
      rehashing();
    }
  } else {
    rehashing();
  }
}

template <typename T, typename U> ull hash_table<T, U>::hash(const T &key) {
  // constexpr - значение вычисляется на этапе компиляции
  if constexpr (std::is_same<T, std::string>()) {
    if (table.empty()) {
      createTable();
    }
    ull func_index = FNVHash(key) % hashes_str.size();
    auto hash_func = hashes_str[func_index];
    return hash_func(key) % array.size();
  } else {

    ull key_to_num = static_cast<ull>(key);
    ull func_index = FNVHash(key_to_num) % hashes.size();
    auto hash_func = hashes[func_index];
    return hash_func(key_to_num) % array.size();
  }
}

template <typename T, typename U> void hash_table<T, U>::rehashing() {
  std::vector<element> copy = array;
  array.clear();
  array.resize(copy.size() * 2 + 1);
  for (auto i : copy) {
    if (i.occupied) {
      (*this)[i.key] = i.data;
    }
  }
}

//Удалить элемент
template <typename T, typename U> bool hash_table<T, U>::erase(const T &key) {
  ull index = hash(key);
  if (array[index].key != key) {
    index = probing(key);
  }
  element &el = array.at(index);
  if (el.occupied) {
    el.clear();
    size--;
    return true;
  }
  return false;
}

// Получить размер массива для хранения
template <typename T, typename U>
unsigned long hash_table<T, U>::getCapacity() const {
  return this->array.size();
}

// Проверка на пустоту
template <typename T, typename U> bool hash_table<T, U>::empty() const {
  return size > 0 ? false : true;
}

// Очистка таблицы
template <typename T, typename U> void hash_table<T, U>::clear() {
  array.clear();
  size = 0;
}

// Получить кол-во уже сохранённых элементов
template <typename T, typename U>
unsigned long hash_table<T, U>::getSize() const {
  return size;
}

// Простой хэш - остаток от деления на простое число
template <typename T, typename U> ull hash_table<T, U>::ModHash(ull key) {
  return key % prime;
}

// Хэш Фибоначчи - прибавление к хэшу последнего разряда числа умноженного на
// текущее число Фибоначчи
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
    hash = (hash + num * f1);
  }
  return hash;
}

// Хэш Фибоначчи через золотое сечение - умножение числа Фи на машинное слово,
// умножение ключа на получившиеся число, сдвиг битов в начало (если машинное
// слово больше 64)
template <typename T, typename U>
ull hash_table<T, U>::FibonacciHashPhi(ull key) {
  ull w = ULONG_LONG_MAX;
  ull A = static_cast<ull>(phi * w);
  return ((key * A) >> (sizeof(ull) * 8 - 64)) % array.size();
}

// FNV хэш - XOR к данными и умножение на простое число
template <typename T, typename U> ull hash_table<T, U>::FNVHash(ull key) {
  ull hash = 2166136261;

  hash ^= key;
  hash *= FNV_prime;

  return hash;
}

// MurmurHash - суть в "хорошем" перемишивании битов числа для широко
// распределённого хэша
template <typename T, typename U> ull hash_table<T, U>::MurmurHash(ull key) {
  const ull M = 0xc6a4a7935bd1e995ULL;
  const int r = 47;
  const ull seed = 0xc70f6907UL;
  // Инициализация хэша константой и сида
  ull hash = seed ^ (8 * M);

  // Перемешивание ключа
  key *= M; // Увеличение распространения битов
  key ^= key >> r; // Перемещивание битов
  key *= M;

  // Добавляем перемешанный ключ к хэшу
  hash ^= key;
  hash *= M;

  // Ещё одно перемешивание битов, но уже с хэшем
  hash ^= hash >> r;
  hash *= M;
  hash ^= hash >> r;

  return hash;
}

/* ################################################
 * Методы хэш таблицы специализированной на строках
 * ################################################
 */

template <typename T, typename U> void hash_table<T, U>::createTable() {
  for (int i = 0; i < 256; i++) {
    table.push_back(i);
  }
  // Перемешивание - идём от конца к началу и меняем в не перемещанной части
  // числа с перемешанной
  for (int i = 255; i > 0; i--) {
    int j = random(0, i);
    std::swap(table[i], table[j]);
  }
}

// Хэш Пирсона - по символьно берём строку и выбираем для хэша число из таблицы
// перестановок по индексу XOR между текущим значением хэша и текущим символом
template <typename T, typename U>
uint8_t hash_table<T, U>::PersonHash8(std::string str) {
  uint8_t hash = str.length() % 256;
  for (auto ch : str) {
    hash = table[(hash ^ static_cast<unsigned char>(ch)) % 256];
  }
  return hash;
}

template <typename T, typename U>
ull hash_table<T, U>::PersonHash64(std::string str) {
  if (str.empty()) {
    return 0;
  }

  std::string hashed_str = str;
  ull hash64 = 0;
  for (unsigned long i = 0; i < sizeof(ull); i++) {
    unsigned char hash = PersonHash8(str);
    hash64 |= hash << ((7 - i) * 8);
    str[0] = (str[0] + 1) % 256;
  }
  return hash64;
}

// Строковый хэш на основе хэширования отдельных символов
template <typename T, typename U>
ull hash_table<T, U>::ModHash(std::string str) {
  ull hash = 0;
  for (auto ch : str) {
    hash += (ch % prime);
  }
  return hash % prime;
}

// FNV хэш для строк, проход по всем символам и поочерёдное их хэширование
template <typename T, typename U>
ull hash_table<T, U>::FNVHash(std::string str) {
  ull hash = 2166136261;

  for (auto ch : str) {
    hash ^= ch;
    hash *= FNV_prime;
  }

  return hash;
}

// Полиминальный хэш - хэш вида hash(S0..n-1) = S0+ps1+p^2S2+...+p^n-1Sn-1, где
// p больше значения наибольшего символа
//-
template <typename T, typename U>
ull hash_table<T, U>::PoliminalHash(std::string str) {
  unsigned p = 31;
  ull p_pow = 1;
  ull hash = 0;

  for (auto ch : str) {
    hash += (ch * p_pow) % FNV_prime;
    p_pow = (p_pow * p) % FNV_prime;
  }
  return hash;
}

template <typename T, typename U>
ull hash_table<T, U>::MurmurHash(std::string str) {
  const ull M = 0xc6a4a7935bd1e995ULL;
  const int r = 47;
  const ull seed = 0xc70f6907UL;
  ull hash = seed ^ (8 * M);
  ull k = 0;
  const int byte = 8;
  std::size_t len = str.length();
  while (len >= 8) {
    k = str[0];
    for (std::size_t i = 1; i < byte; i++) {
      k |= static_cast<ull>(str[i]) << (byte * i);
    }

    k *= M;
    k ^= k >> r;
    k *= M;

    hash *= M;
    hash ^= k;
    str = str.substr(8);
    len -= 8;
  }

  if (len > 0) {
    ull remainder = 0;
    for (size_t i = 0; i < len; i++) {
      remainder |= static_cast<ull>(str[i]) << (byte * i);
    }
    hash ^= remainder;
    hash *= M;
  }

  hash ^= hash >> 47;
  hash *= M;
  hash ^= hash >> 47;

  return hash;
}
} // namespace hashtable
