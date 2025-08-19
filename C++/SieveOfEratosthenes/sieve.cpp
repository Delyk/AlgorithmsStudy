#include "sieve.h"
#include <cmath>
#include <cstddef>
#include <vector>
using namespace sieve;

std::vector<unsigned long long> SieveOfEratosthenes(unsigned long long n) {
  std::vector<bool> nums(n - 1, true);

  for (std::size_t i = 2; std::pow(i, 2) <= n; i++) {
    if (nums[i]) {
      for (std::size_t j = std::pow(i, 2); j <= n; j += i) {
        nums[j] = false;
      }
    }
  }
  std::vector<unsigned long long> primes;
  for (std::size_t i = 0; i < n; i++) {
    if (nums[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}
