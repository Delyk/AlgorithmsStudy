#pragma once
#include <initializer_list>
#include <vector>

namespace hashtable {

const unsigned primes[100] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
    383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541};

template <typename T, typename U> class hash_table {
  struct element {
    T key;
    U data;
    bool occupied;
    element();
    void push(T, U);
    void clear();
  };
  float load_factor;  //Коэффициент загрузки
  unsigned k;         //Интервал обхода
  unsigned long size; //Размер, т.е. кол-во элементов
  std::vector<element> array;
  unsigned hash(T) const; //Хэш функция
  unsigned hash_k(unsigned);
  unsigned probing(unsigned) const; //Функция обхода (двойное хэширование)
  void rehashing(); //Рехиширование массива при увеличении размера

public:
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
};

template <typename T, typename U>
hash_table<T, U>::hash_table(unsigned long size) : size(0), load_factor(0.0f) {
  array.resize(size);
  hash_k(size);
}
} // namespace hashtable
