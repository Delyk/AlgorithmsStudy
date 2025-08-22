#pragma once
#include <vector>

namespace sieve {
//Классический алгоритм
std::vector<unsigned> SieveOfEratosthenes(unsigned n);

//Неограниченное решето Эратосфена
class UnlimitedSieve {
  std::vector<bool> primes;
  unsigned index;
  unsigned last_prime;
  void next_sieve();
  bool find_next();

public:
  UnlimitedSieve(unsigned = 2);
  unsigned nextPrime();
};

//Алгоритм с пропусками кратных
std::vector<unsigned> SieveOfEratosthenes_skip(unsigned n);

//Алгоритм с кольцевой факторизацией
std::vector<unsigned> SieveOfEratosthenes_wheel_factorization(unsigned n);

//Сегментированное решето
std::vector<unsigned long long>
SieveOfEratosthenes_segmented(unsigned long long n);

//Алгоритм с линейным временем выполнения
std::vector<unsigned> SieveOfEratosthenes_linear(unsigned n);

//Решето с распараллеливанием
std::vector<unsigned long long>
SieveOfEratosthenes_parallel(unsigned long long n);

//Решето Аткина
std::vector<unsigned long long> SieveOfAtkhin(unsigned long long n);
} // namespace sieve
