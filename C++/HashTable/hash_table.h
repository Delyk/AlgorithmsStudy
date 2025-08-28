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

//Функция для генерации случайного числа
static unsigned random(unsigned start, unsigned end) {
  std::random_device rd;  // Устройство случайности
  std::mt19937 gen(rd()); // Генератор
  std::uniform_int_distribution<> dist(start, end); // Диапазон распределения
  return dist(gen);
}

//Интерфейс для хэш таблиц
class hash_table_interface {
protected:
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
  struct elementBase {
    bool occupied;
    elementBase() : occupied(false){};
    virtual void clear() = 0;
  };
  ull prime;          //Простое число для алгоритмов
  unsigned long size; //Размер, т.е. кол-во элементов

public:
  hash_table_interface(unsigned long size) : size(size) {}
  virtual bool empty() const = 0; //Проверка на пустоту
  virtual void clear() = 0;       //Очистка
  virtual unsigned long
  getSize() const = 0; //Получить кол-во сохранённых элементов
  virtual unsigned long
  getCapacity() const = 0; //Получить размер массива для хранения
};

//Обычная универсальная хэш таблица
template <typename T, typename U>
class hash_table : public hash_table_interface {
public:
  //Элемент хранящий ключ и данные
  struct element : public hash_table_interface::elementBase {
    T key;
    U data;
    element() : hash_table_interface::elementBase(), key(T{}), data(U{}) {}
    void clear() override {}
  };
  //Массив для хранения
  std::vector<element> array;
  //Набор хэш функций
  ull ModHash(ull);
  ull FibonacciHash(ull);
  ull FibonacciHashPhi(ull);
  ull FNVHash(ull);
  ull MurmurHash(ull);
  //Массив хэш функций
  std::vector<std::function<unsigned(T)>> hashes;
  //Хэширование
  unsigned hash(T) const; //Хэш функция
  //Конструкторы
  hash_table(unsigned long = 0);
  hash_table(const hash_table &);
  hash_table(const hash_table &&) noexcept;
  hash_table(std::initializer_list<std::pair<T, U>>);
  //Операторы присваивания
  hash_table &operator=(const hash_table &);
  hash_table &operator=(const hash_table &&) noexcept;
  //Оператор получения значения по ключу
  U &operator[](T);
  //Удаление элемента
  void erase(T);
  bool empty() const override;
  void clear() override;
  unsigned long getSize() const override;
  unsigned long getCapacity() const override;
};

//Специализированная хэш таблица для строк
template <typename U>
class hash_table<std::string, U> : public hash_table_interface {
public:
  struct element : public hash_table_interface::elementBase {
    std::string key;
    U data;
    element()
        : hash_table_interface::elementBase(), key(std::string{}), data(U{}) {}
    void clear() override{};
  };
  //Таблица перестановок для хэша Пирсона
  std::vector<uint8_t> table;
  std::vector<element> array;
  //Хэши для строк
  ull PersonHash64(std::string);
  uint8_t PersonHash8(std::string);
  ull ModHash(std::string);
  ull FNVHash(std::string);
  ull PoliminalHash(std::string);
  ull MurmurHash(std::string);
  unsigned hash(std::string) const; //Хэш функция
  hash_table(unsigned long = 0);
  hash_table(const hash_table &);
  hash_table(const hash_table &&) noexcept;
  hash_table(std::initializer_list<std::pair<std::string, U>>);
  hash_table &operator=(const hash_table &);
  hash_table &operator=(const hash_table &&) noexcept;
  U &operator[](std::string);
  void erase(std::string);
  bool empty() const override;
  void clear() override;
  unsigned long getSize() const override;
  unsigned long getCapacity() const override;
};

/*
 * Методы общей хэш-таблицы
 */
//Конструкторы
template <typename T, typename U>
hash_table<T, U>::hash_table(unsigned long size) : hash_table_interface(size) {
  prime = primes[random(0, 99)];
  array.resize(size);
}

//Получить размер массива для хранения
template <typename T, typename U>
unsigned long hash_table<T, U>::getCapacity() const {
  return this->array.size();
}

// Проверка на пустоту
template <typename T, typename U> bool hash_table<T, U>::empty() const {
  return true;
}

// Очистка таблицы
template <typename T, typename U> void hash_table<T, U>::clear() {}

//Получить кол-во уже сохранённых элементов
template <typename T, typename U>
unsigned long hash_table<T, U>::getSize() const {
  return size;
}

//Простой хэш - остаток от деления на простое число
template <typename T, typename U> ull hash_table<T, U>::ModHash(ull key) {
  return (key % prime) % size;
}

//Хэш Фибоначчи - прибавление к хэшу последнего разряда числа умноженного на
//текущее число Фибоначчи
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

//Хэш Фибоначчи через золотое сечение - умножение числа Фи на машинное слово,
//умножение ключа на получившиеся число, сдвиг битов в начало (если машинное
//слово больше 64)
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

  return hash % array.size();
}

// MurmurHash - суть в "хорошем" перемишивании битов числа для широко
// распределённого хэша
template <typename T, typename U> ull hash_table<T, U>::MurmurHash(ull key) {
  const ull M = 0xc6a4a7935bd1e995ULL;
  const int r = 47;
  const ull seed = 0xc70f6907UL;
  //Инициализация хэша константой и сида
  ull hash = seed ^ (8 * M);

  //Перемешивание ключа
  key *= M; //Увеличение распространения битов
  key ^= key >> r; //Перемещивание битов
  key *= M;

  //Добавляем перемешанный ключ к хэшу
  hash ^= key;
  hash *= M;

  //Ещё одно перемешивание битов, но уже с хэшем
  hash ^= hash >> r;
  hash *= M;
  hash ^= hash >> r;

  return hash % array.size();
}

/*
 * Методы хэш таблицы специализированной на строках
 */
//Конструктор с генерацией таблицы перестановок
template <typename U>
hash_table<std::string, U>::hash_table(unsigned long size)
    : hash_table_interface(size) {
  prime = primes[random(0, 99)];
  array.resize(size);
  for (int i = 0; i < 256; i++) {
    table.push_back(i);
  }
  //Перемешивание - идём от конца к началу и меняем в не перемещанной части
  //числа с перемешанной
  for (int i = 255; i > 0; i--) {
    int j = random(0, i);
    std::swap(table[i], table[j]);
  }
}

//Хэш Пирсона - по символьно берём строку и выбираем для хэша число из таблицы
//перестановок по индексу XOR между текущим значением хэша и текущим символом
template <typename U>
uint8_t hash_table<std::string, U>::PersonHash8(std::string str) {
  uint8_t hash = str.length() % 256;
  for (auto ch : str) {
    hash = table[(hash ^ static_cast<unsigned char>(ch)) % 256];
  }
  return hash;
}

template <typename U>
ull hash_table<std::string, U>::PersonHash64(std::string str) {
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
  return hash64 % array.size();
}

//Строковый хэш на основе хэширования отдельных символов
template <typename U> ull hash_table<std::string, U>::ModHash(std::string str) {
  ull hash = 0;
  for (auto ch : str) {
    hash += (ch % prime);
  }
  return (hash % prime) % array.size();
}

// FNV хэш для строк, проход по всем символам и поочерёдное их хэширование
template <typename U> ull hash_table<std::string, U>::FNVHash(std::string str) {
  ull hash = 2166136261;

  for (auto ch : str) {
    hash ^= ch;
    hash *= FNV_prime;
  }

  return hash % array.size();
}

//Полиминальный хэш - хэш вида hash(S0..n-1) = S0+ps1+p^2S2+...+p^n-1Sn-1, где p
//больше значения наибольшего символа
//-
template <typename U>
ull hash_table<std::string, U>::PoliminalHash(std::string str) {
  unsigned p = 31;
  ull p_pow = 1;
  ull hash = 0;

  for (auto ch : str) {
    hash += (ch * p_pow) % FNV_prime;
    p_pow = (p_pow * p) % FNV_prime;
  }
  return hash % array.size();
}

template <typename U>
ull hash_table<std::string, U>::MurmurHash(std::string str) {
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

  return hash % array.size();
}

//Проверка на пустоту
template <typename U> bool hash_table<std::string, U>::empty() const {
  return true;
}

//Очистка таблицы
template <typename U> void hash_table<std::string, U>::clear() {}

//Получить текущего кол-во сохранённых элементов
template <typename U>
unsigned long hash_table<std::string, U>::getSize() const {
  return size;
}

//Получить размер массива для хранения
template <typename U>
unsigned long hash_table<std::string, U>::getCapacity() const {
  return this->array.size();
}
} // namespace hashtable
