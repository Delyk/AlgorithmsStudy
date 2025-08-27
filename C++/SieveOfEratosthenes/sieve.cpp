#include "sieve.h"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>
using namespace sieve;

std::vector<unsigned long long>
sieve::SieveOfEratosthenes(unsigned long long n) {
  // Создаём булевой массив длиной n
  std::vector<bool> nums(n + 1, true);

  // Начиная с двух делаем обход до тех пор, пока квадрат
  // индекса не станет больше n
  for (std::size_t i = 2; std::pow(i, 2) <= n; i++) {
    // Если число простое, то убираем все кратные числа
    if (nums[i]) {
      // Начинаем с квадрата т.к. до этого все числа уже будут убраны
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        nums[j] = false;
      }
    }
  }
  // Переносим все простые числа в вектор
  std::vector<unsigned long long> primes;
  for (std::size_t i = 2; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

// При инициализации неограниченного решете генерируем массив как обычно
sieve::UnlimitedSieve::UnlimitedSieve(unsigned long long n) {
  primes.resize(n + 1, true);
  primes[0] = primes[1] = false;
  for (std::size_t i = 2; i <= n; i++) {
    if (primes[i]) {
      last_prime = i - 1;
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        primes[j] = false;
      }
    }
  }
}

// Сначала ищем следующее просто в уже существующем массиве
bool sieve::UnlimitedSieve::find_next() {
  for (std::size_t i = last_prime + 1; i < primes.size(); i++) {
    if (primes[i]) {
      last_prime = i;
      return true;
    }
  }
  return false;
}

// Если следующее просто число не было найдено, то генерируем продолжение
// массива
void sieve::UnlimitedSieve::next_sieve() {
  // Старт следующего просеивания берём как квадрат последнего простого числа
  std::size_t old_size = primes.size();
  std::size_t new_sieve = std::pow(old_size, 2);
  // Увеличиваем размер массива на квадрат
  primes.resize(new_sieve, true);
  // Алгоритм тот же самый
  std::size_t lim = static_cast<std::size_t>(std::sqrt(new_sieve));
  for (std::size_t i = 2; i <= lim; i++) {
    if (primes[i]) {
      for (std::size_t j = i * i; j <= primes.size(); j += i) {
        primes[j] = false;
      }
    }
  }
}

unsigned long long sieve::UnlimitedSieve::nextPrime() {
  // Сначала пытаемся найти следующее в уже существующем массиве
  if (!find_next()) {
    // Если простые кончились, то генерируем следующий набор
    next_sieve();
    if (!find_next()) {
      throw std::runtime_error("Not found next prime");
    }
  }

  return last_prime;
}

// Решето Эратосфена с пропуском чётных
std::vector<unsigned long long>
sieve::SieveOfEratosthenes_skip(unsigned long long n) {
  // Создаём массив вдвое меньшего размера
  std::vector<bool> nums;
  if (n > 2)
    nums.resize(n / 2, true);
  else if (n == 2) {
    return {2};
  } else
    return {};

  // Итерация массива как обычно, только начало с 0
  for (std::size_t i = 0; std::pow(i, 2) <= n / 2.0f; i++) {
    if (nums[i]) {
      unsigned long long p =
          2 * i + 3; // Шаг для нечётных
                     //+3 т.к. мы начинаем индексацию с 3
                     // Внутренний цикл такой же за исключением поправки на шаг
      for (std::size_t j = (std::pow(p, 2) - 3) / 2; j < nums.size(); j += p) {
        nums[j] = false;
      }
    }
  }

  std::vector<unsigned long long> primes{2};
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

// Проверка, что число взаимно простое с базисом
static bool isNotDivide(unsigned long long i,
                        const std::vector<unsigned long long> &bazis) {
  for (unsigned long long j : bazis) {
    if (!(i % j)) {
      return false;
    }
  }
  return true;
}

// Получить размер начальной последовательности
static inline unsigned long long
getStartSize(const std::vector<unsigned long long> &bazis) {
  unsigned long long size = 1;
  for (unsigned long long i : bazis) {
    size *= (i - 1);
  }
  return size;
}

// Кольцевая факторизация
static std::vector<unsigned long long>
wheel_factorization(unsigned long long n) {
  // Указываем базис и ищем его длину
  std::vector<unsigned long long> bazis{2, 3, 5};
  unsigned long long primorial = 1;
  for (auto it = bazis.begin(); it != bazis.end(); it++)
    primorial *= *it;
  // Заполняем начальную последовательность значениями
  std::vector<unsigned long long> wheel;
  for (std::size_t i = 1; i <= primorial; i++) {
    if (isNotDivide(i, bazis)) {
      wheel.push_back(i);
    }
  }
  // Оставшиюся последовательность вычисляем в цикле прибавляя к начальной
  // последовательности длину базиса, со умножением длины на текущий
  // цикл
  unsigned long long size = getStartSize(bazis);
  for (unsigned long long i = 1; i <= primorial; i++) {
    for (std::size_t j = 0; j < size; j++) {
      unsigned long long val = wheel[j] + i * primorial;
      if (val > n) {
        return wheel;
      }
      wheel.push_back(val);
    }
  }

  return wheel;
}

std::vector<unsigned long long>
sieve::SieveOfEratosthenes_wheel_factorization(unsigned long long n) {
  if (n < 2) {
    return {};
  }
  std::vector<unsigned long long> wheel = wheel_factorization(n);
  std::vector<bool> nums(n + 1, true);
  nums[0] = nums[1] = false;

  //Допросеивание до первого простого числа из колеса
  for (std::size_t i = 2; i < wheel[1] || i < nums.size(); i++) {
    if (nums[i]) {
      for (std::size_t j = i * i; j < wheel[1] || j < nums.size(); j += i) {
        nums[j] = false;
      }
    }
  }

  // Итерируемся и проверяем только числа из колеса факторизации
  for (auto i : wheel) {
    if (i < 2)
      continue;
    if (std::pow(i, 2) > n)
      break;
    if (nums[i]) {
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        nums[j] = false;
      }
    }
  }

  // Переносим все простые числа в вектор
  std::vector<unsigned long long> primes;
  for (std::size_t i = 2; i <= n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

// Сегментированное решето для больших чисел
std::vector<unsigned long long>
sieve::SieveOfEratosthenes_segmented(unsigned long long n) {
  if (n < 2) {
    return {};
  }
  std::vector<unsigned long long> primes;
  //Размер сегмента как корень из n
  unsigned long long lim = (std::sqrt(n)) + 1;
  std::vector<bool> nums(lim + 1, true);
  nums[0] = nums[1] = false;

  //Первоначальное просеивание первого сегмента
  for (std::size_t i = 2; i * i <= lim; i++) {
    if (nums[i]) {
      for (std::size_t j = i * i; j <= lim; j += i) {
        nums[j] = false;
      }
    }
  }

  for (std::size_t i = 2; i <= lim; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  nums.clear();
  nums.resize(lim, true);

  //Установка границ сегмента
  //Нижняя граница по корню из n
  unsigned long long low = lim;
  //Верхняя граница в два раза больше или равная n
  unsigned long long high = 2 * lim - 1;
  if (high > n)
    high = n;

  //Пока нижняя граница меньше n
  while (low <= n) {
    //Сброс значений на true
    std::fill(nums.begin(), nums.end(), true);
    //Для всех простых из уже найденных
    for (auto i : primes) {
      //Начинаем с первого кратного i в новом сегменте
      //(low+i-1) - окргуляем до нового сегмента, чтобы найти  первое кратное в
      //нём
      // / i - Получаем коэффициент (сколько раз простое содержится в числе)
      // * i - Умножаем и получаем точное кратное число
      unsigned long long start = (low + i - 1) / i * i;
      //Если стартового значение меньше нижней границы, то Устанавливаем его в
      //квадрат
      //Либо просто в нижнюю границу
      if (start < low)
        start = i * i;
      if (start < low)
        start = low;

      //Убираем все кратные простым числам
      for (std::size_t j = start; j <= high; j += i) {
        nums[j - low] = false;
      }
    }

    //Помещаем простые в массив
    for (std::size_t i = low; i <= high; i++) {
      if (nums[i - low]) {
        primes.push_back(i);
      }
    }

    //Переходим к следующему сегменту
    low += lim;
    high += lim;
    if (high > n)
      high = n;
  }

  return primes;
}

// Алгоритм с линейным временем выполнения
std::vector<unsigned long long>
sieve::SieveOfEratosthenes_linear(unsigned long long n) {
  std::vector<unsigned long long> pr;
  std::vector<unsigned long long> lp(n + 1, 0); // Минимальные простые делители
  // Для каждого числа от 2 до n
  for (std::size_t i = 2; i <= n; i++) {
    // Если lp пуст
    if (!lp[i]) {
      // Устанавливаем пустоту в i
      lp[i] = i;
      pr.push_back(i); // Помещаем индекс в массив простых чисел
    }
    // Начинаем итерацию в массиве простых чисел
    // пока не достигнем добавленного числа или числа до
    // которого ищем простые
    for (auto p = pr.begin(); *p <= lp[i] && *p * i <= n; p++) {
      lp[*p * i] = *p; // Устанавливаем все кратные в ненулевые значения
    }
  }
  return pr;
}

// Решето с распараллеливанием
// Функция параллельного выполнения - поиск всех кратных
static void del_multiple(unsigned long long i, unsigned long long n,
                         std::vector<bool> &nums, std::mutex &m) {
  for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
    // Блокируем массив mutexом
    std::lock_guard<std::mutex> lock(m);
    nums[j] = false;
  }
}

std::vector<unsigned long long>
sieve::SieveOfEratosthenes_parallel(unsigned long long n) {
  if (n < 2)
    return {};
  // Инициализация, создаём потоки по кол-ву ядер
  unsigned threads_count = std::thread::hardware_concurrency();
  if (!threads_count)
    threads_count = 2;
  std::vector<std::thread> threads;
  std::mutex m;
  std::vector<bool> nums(n + 1, true);
  nums[0] = nums[1] = false;

  for (std::size_t i = 2; std::pow(i, 2) <= n; i++) {
    if (nums[i]) {
      // Если потоков больше чем кол-во ядер, то ждём их завершения
      if (threads.size() >= threads_count) {
        for (auto &t : threads)
          t.join();
        threads.clear();
      }
      // Если число простое, то поиск кратных передаём в поток
      // Активируем поток через помещение в вектор конструктор потока
      threads.emplace_back(del_multiple, i, n, std::ref(nums), std::ref(m));
    }
  }
  // Ждём завершения потоков
  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }
  threads.clear();

  std::vector<unsigned long long> primes;
  for (std::size_t i = 2; i <= n; i++) {
    if (nums[i])
      primes.push_back(i);
  }
  return primes;
}

// Решето Аткина
std::vector<unsigned long long> sieve::SieveOfAtkhin(unsigned long long n) {
  using ull = unsigned long long;
  std::vector<unsigned long long> primes{2, 3, 5};
  if (n <= 5) {
    return primes;
  }
  std::vector<bool> nums(n + 1, false);
  unsigned long long lim = std::sqrt(n);
  if (lim >= 3)
    nums[2] = nums[3] = true;

  // Основной цикл проходит по всем возможным x и y от 1 до sqrt(n)
  for (ull x = 1; x <= lim; x++) {
    ull x2 = x * x; // x^2 для ускорения расчетов
    for (ull y = 1; y <= lim; y++) {
      ull y2 = y * y;       // y^2
      ull eq = 4 * x2 + y2; // 4x^2 + y^2
      // Первая квадратичная форма: n = 4x^2 + y^2
      // Числа с остатками 1 или 5 по модулю 12 могут быть простыми
      if (eq <= n && (eq % 12 == 1 || eq % 12 == 5)) {
        nums[eq] = !nums[eq]; // Если кол-во решений нечётно, то тут будет true
      }

      eq = 3 * x2 + y2; // 3*x^2 + y^2
      // Вторая квадратичная форма: n = 3x^2 + y^2
      // Числа с остатком 7 по модулю 12 могут быть простыми
      if (eq <= n && (eq % 12 == 7)) {
        nums[eq] = !nums[eq]; // Если кол-во решений нечётно, то тут будет true
      }
      // Третья квадратичная форма: n = 3x^2 - y^2, где x > y
      // Числа с остатком 11 по модулю 12 могут быть простыми
      if (x > y) {
        eq = 3 * x2 - y2; // 3*x^2 - y^2
        if (eq <= n && (eq % 12 == 11)) {
          nums[eq] =
              !nums[eq]; // Если кол-во решений нечётно, то тут будет true
        }
      }
    }
  }

  // После маркировки чисел кандидатов исключаем числа,
  // которые кратны квадратам найденных простых (начиная с 5)
  // Это необходимо, так как простые не могут иметь квадратные делители
  for (ull i = 5; i <= lim; i++) {
    if (nums[i]) {
      ull i2 = i * i;
      for (ull j = i2; j < n; j += i2) {
        nums[j] = false;
      }
    }
  }

  for (std::size_t i = 6; i <= n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }
  return primes;
}
