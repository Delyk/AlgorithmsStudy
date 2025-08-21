#include "sieve.h"
#include <cmath>
#include <cstddef>
#include <vector>
using namespace sieve;

std::vector<unsigned> sieve::SieveOfEratosthenes(unsigned n) {
  //Создаём булевой массив длиной n
  std::vector<bool> nums(n - 1, true);

  //Начиная с двух делаем обход до тех пор, пока квадрат
  //индекса не станет больше n
  for (std::size_t i = 2; std::pow(i, 2) <= n; i++) {
    //Если число простое, то убираем все кратные числа
    if (nums[i]) {
      //Начинаем с квадрата т.к. до этого все числа уже будут убраны
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        nums[j] = false;
      }
    }
  }
  //Переносим все простые числа в вектор
  std::vector<unsigned> primes;
  for (std::size_t i = 2; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

//При инициализации неограниченного решете генерируем массив как обычно
sieve::UnlimitedSieve::UnlimitedSieve(unsigned n) {
  primes.resize(n - 1, true);
  for (std::size_t i = 2; std::pow(i, 2) <= n; i++) {
    if (primes[i]) {
      last_prime = i;
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        primes[j] = false;
      }
    }
  }
}

//Сначала ищем следующее просто в уже существующем массиве
bool sieve::UnlimitedSieve::find_next() {
  bool has_prime = false;
  for (std::size_t i = last_prime; i < primes.size(); i++) {
    has_prime = primes[i];
    if (has_prime) {
      last_prime = i;
      break;
    }
  }
  return has_prime;
}

//Если следующее просто число не было найдено, то генерируем продолжение массива
void sieve::UnlimitedSieve::next_sieve() {
  //Старт следующего просеивания берём как квадрат последнего простого числа
  std::size_t new_sieve = std::pow(last_prime, 2);
  //Увеличиваем размер массива на квадрат
  primes.resize(std::pow(primes.size(), 2), true);
  //Алгоритм тот же самый
  for (std::size_t i = new_sieve; std::pow(i, 2) <= primes.size(); i++) {
    if (primes[i]) {
      for (std::size_t j = std::pow(i, 2); j <= primes.size(); j += i) {
        primes[j] = false;
      }
    }
  }
}

unsigned sieve::UnlimitedSieve::nextPrime() {
  //Сначала пытаемся найти следующее в уже существующем массиве
  if (!find_next()) {
    //Если простые кончились, то генерируем следующий набор
    next_sieve();
    find_next();
  }

  return last_prime;
}

//Решето Эратосфена с пропуском чётных
std::vector<unsigned> SieveOfEratosthenes_skip(unsigned n) {
  //Создаём массив вдвое меньшего размера
  std::vector<bool> nums((n - 1) / 2, true);

  //Итерация массива как обычно, только начало с 0
  for (std::size_t i = 0; std::pow(i, 2) <= n / 2; i++) {
    if (nums[i]) {
      unsigned p =
          2 * i + 3; //Шаг для нечётных
                     //+3 т.к. мы начинаем индексацию с 3
                     //Внутренний цикл такой же за исключением поправки на шаг
      for (std::size_t j = (std::pow(p, 2) - 3) / 2; j < nums.size(); j += p) {
        nums[i] = false;
      }
    }
  }

  std::vector<unsigned> primes{2};
  for (std::size_t i = 0; i < nums.size(); i++) {
    if (nums[i]) {
      primes.push_back(2 * i + 3);
    }
  }
  return primes;
}

/*Кольцевая факторизация - метод создания последовательности натуральных чисел,
взаимно простых (НОД = 1) с несколькими первыми простыми числами
*/

//Проверка, что число взаимно простое с базисом
static bool isNotDivide(unsigned i, const std::vector<unsigned> &bazis) {
  for (unsigned j : bazis) {
    if (!(i % j)) {
      return false;
    }
  }
  return true;
}

//Получить размер начальной последовательности
static inline unsigned getStartSize(const std::vector<unsigned> &bazis) {
  unsigned size = 1;
  for (unsigned i : bazis) {
    size *= (i - 1);
  }
  return size;
}

//Кольцевая факторизация
static std::vector<unsigned> wheel_factorization(unsigned n) {
  //Указываем базис и ищем его длину
  std::vector<unsigned> bazis{2, 3, 5};
  unsigned primorial = 1;
  for (auto it = bazis.begin(); it != bazis.end(); it++, primorial *= *it)
    ;
  //Заполняем начальную последовательность значениями
  std::vector<unsigned> wheel;
  for (std::size_t i = 1; i <= primorial; i++) {
    if (isNotDivide(i, bazis)) {
      wheel.push_back(i);
    }
  }
  //Оставшиюся последовательность вычисляем прибавляя к начальной
  //последовательности длину базиса в цикле, со умножением длины на текущий цикл
  unsigned size = getStartSize(bazis);
  for (int i = 1; i <= primorial; i++) {
    for (std::size_t j = 0; j < size; j++) {
      unsigned val = wheel[j] + i * primorial;
      if (val > n) {
        return wheel;
      }
      wheel.push_back(val);
    }
  }

  return wheel;
}

std::vector<unsigned> SieveOfEratosthenes_wheel_factorization(unsigned n) {
  std::vector<unsigned> wheel = wheel_factorization(n);
  std::vector<bool> nums(n + 1, true);
  nums[0] = nums[3] = false;

  //Итерируемся и проверяем только числа из колеса факторизации
  for (auto i : wheel) {
    if (i < 2)
      continue;
    if (std::pow(i, 2) > n)
      break;
    if (nums[i]) {
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        nums[i] = false;
      }
    }
  }

  //Переносим все простые числа в вектор
  std::vector<unsigned> primes;
  for (std::size_t i = 2; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

//Сегментированное решето для больших чисел
std::vector<unsigned long long>
SieveOfEratosthenes_segmented(unsigned long long n);
