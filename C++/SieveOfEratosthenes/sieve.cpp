#include "sieve.h"
#include <cmath>
#include <cstddef>
#include <iterator>
#include <mutex>
#include <thread>
#include <vector>
using namespace sieve;

std::vector<unsigned> sieve::SieveOfEratosthenes(unsigned n) {
  // Создаём булевой массив длиной n
  std::vector<bool> nums(n - 1, true);

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
  std::vector<unsigned> primes;
  for (std::size_t i = 2; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

// При инициализации неограниченного решете генерируем массив как обычно
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

// Сначала ищем следующее просто в уже существующем массиве
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

// Если следующее просто число не было найдено, то генерируем продолжение
// массива
void sieve::UnlimitedSieve::next_sieve() {
  // Старт следующего просеивания берём как квадрат последнего простого числа
  std::size_t new_sieve = std::pow(last_prime, 2);
  // Увеличиваем размер массива на квадрат
  primes.resize(std::pow(primes.size(), 2), true);
  // Алгоритм тот же самый
  for (std::size_t i = new_sieve; std::pow(i, 2) <= primes.size(); i++) {
    if (primes[i]) {
      for (std::size_t j = std::pow(i, 2); j <= primes.size(); j += i) {
        primes[j] = false;
      }
    }
  }
}

unsigned sieve::UnlimitedSieve::nextPrime() {
  // Сначала пытаемся найти следующее в уже существующем массиве
  if (!find_next()) {
    // Если простые кончились, то генерируем следующий набор
    next_sieve();
    find_next();
  }

  return last_prime;
}

// Решето Эратосфена с пропуском чётных
std::vector<unsigned> SieveOfEratosthenes_skip(unsigned n) {
  // Создаём массив вдвое меньшего размера
  std::vector<bool> nums((n - 1) / 2, true);

  // Итерация массива как обычно, только начало с 0
  for (std::size_t i = 0; std::pow(i, 2) <= n / 2; i++) {
    if (nums[i]) {
      unsigned p =
          2 * i + 3; // Шаг для нечётных
                     //+3 т.к. мы начинаем индексацию с 3
                     // Внутренний цикл такой же за исключением поправки на шаг
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

// Проверка, что число взаимно простое с базисом
static bool isNotDivide(unsigned i, const std::vector<unsigned> &bazis) {
  for (unsigned j : bazis) {
    if (!(i % j)) {
      return false;
    }
  }
  return true;
}

// Получить размер начальной последовательности
static inline unsigned getStartSize(const std::vector<unsigned> &bazis) {
  unsigned size = 1;
  for (unsigned i : bazis) {
    size *= (i - 1);
  }
  return size;
}

// Кольцевая факторизация
static std::vector<unsigned long long>
wheel_factorization(unsigned long long n) {
  // Указываем базис и ищем его длину
  std::vector<unsigned> bazis{2, 3, 5, 7};
  unsigned primorial = 1;
  for (auto it = bazis.begin(); it != bazis.end(); it++, primorial *= *it)
    ;
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
  std::vector<unsigned long long> wheel = wheel_factorization(n);
  std::vector<bool> nums(n + 1, true);
  nums[0] = nums[3] = false;

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
  std::vector<unsigned> primes;
  for (std::size_t i = 2; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

// Сегментированное решето для больших чисел
std::vector<unsigned long long>
SieveOfEratosthenes_segmented(unsigned long long n) {
  // За размер сегмента берём корень из n
  std::size_t delta = std::sqrt(n);
  std::vector<unsigned long long> primes;
  // Внешний цикл по сегментам
  for (std::size_t i = 0; i < n; i += delta) {
    std::vector<bool> nums(delta + 1, true);
    // Если цикл первый, то заполняем начальные значения в массив простых чисел
    if (primes.empty()) {
      for (std::size_t i = 2; std::pow(i, 2) <= delta; i++) {
        if (nums[i]) {
          for (std::size_t j = std::pow(i, 2); j < delta; j += i) {
            nums[j] = false;
          }
        }
      }
    } else {
      // Иначе проверяем все числа из массива до корня из delta
      for (auto it = primes.begin();
           *it <= static_cast<unsigned long long>(sqrt(i)) &&
           it != primes.end();
           i++) {
        // Устанавливаем все кратные в false
        for (std::size_t j = std::pow(*it, 2); j < delta; j += *it) {
          nums[j] = false;
        }
      }
    }
    // Помещаем числа из сегмента в массив
    for (std::size_t k = 0; k < nums.size(); k++) {
      if (nums[k]) {
        primes.push_back(k + i);
      }
    }
  }
  return primes;
}

// Алгоритм с линейным временем выполнения
std::vector<unsigned> SieveOfEratosthenes_linear(unsigned n) {
  std::vector<unsigned> pr;
  std::vector<unsigned> lp(n, 0); // Минимальные простые делители
  // Для каждого числа от 2 до n
  for (std::size_t i = 2; i <= n; i++) {
    // Если lp пуст
    if (!lp[i]) {
      // Устанавливаем пустоту в i
      lp[i] = i;
      pr.push_back(i); // Помещаем индекс в массив простых чисел
                       // Начинаем итерацию в массиве простых чисел
                       // пока не достигнем добавленного числа или числа до
                       // которого ищем простые
      for (auto p = pr.begin(); *p <= lp[i] && *p * i <= n; p++) {
        lp[*p * i] = *p; // Устанавливаем все кратные в ненулевые значения
      }
    }
  }
  return pr;
}

// Решето с распараллеливанием
// Функция параллельного выполнения - поиск всех кратных
void del_multiple(unsigned i, unsigned n, std::vector<bool> &nums,
                  std::mutex &m) {
  for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
    // Блокируем массив mutexом
    std::lock_guard<std::mutex> lock(m);
    nums[j] = false;
  }
}

std::vector<unsigned> SieveOfEratosthenes_parallel(unsigned n) {
  // Инициализация, создаём потоки по кол-ву ядер
  unsigned threads_count = std::thread::hardware_concurrency();
  if (!threads_count)
    threads_count = 2;
  std::vector<std::thread> threads(threads_count);
  std::mutex m;
  std::vector<bool> nums(n - 1, true);

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
  for (auto &t : threads)
    t.join();
  threads.clear();

  std::vector<unsigned> primes;
  for (std::size_t i = 2; i <= n; i++) {
    if (nums[i])
      primes.push_back(i);
  }
  return primes;
}

// Решето Аткина
std::vector<unsigned long long> SieveOfAtkhin(unsigned long long n) {
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
