#pragma once
#include <vector>

namespace sieve {
// Классический алгоритм
std::vector<unsigned long long> SieveOfEratosthenes(unsigned long long n);

// Неограниченное решето Эратосфена
class UnlimitedSieve {
  std::vector<bool> primes;
  unsigned long long index;
  unsigned long long last_prime;
  void next_sieve();
  bool find_next();

public:
  UnlimitedSieve(unsigned long long = 2);
  unsigned long long nextPrime();
};

// Алгоритм с пропусками кратных
std::vector<unsigned long long> SieveOfEratosthenes_skip(unsigned long long n);

// Алгоритм с кольцевой факторизацией
std::vector<unsigned long long>
SieveOfEratosthenes_wheel_factorization(unsigned long long n);

// Сегментированное решето
std::vector<unsigned long long>
SieveOfEratosthenes_segmented(unsigned long long n);

// Алгоритм с линейным временем выполнения
std::vector<unsigned long long>
SieveOfEratosthenes_linear(unsigned long long n);

// Решето с распараллеливанием
std::vector<unsigned long long>
SieveOfEratosthenes_parallel(unsigned long long n);

// Решето Аткина
std::vector<unsigned long long> SieveOfAtkhin(unsigned long long n);
} // namespace sieve
