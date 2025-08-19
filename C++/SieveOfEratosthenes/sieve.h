#pragma once
#include <vector>

namespace sieve {
std::vector<unsigned long long> SieveOfEratosthenes(unsigned long long n);

std::vector<unsigned long long>
SieveOfEratosthenes_optimized(unsigned long long n);

std::vector<unsigned long long>
SieveOfEratosthenes_parallel(unsigned long long n);
} // namespace sieve
