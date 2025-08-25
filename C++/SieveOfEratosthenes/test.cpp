#include "sieve.h"
#include "gtest/gtest.h"
#include <chrono>
#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <iomanip>
#include <ios>
#include <map>
#include <ratio>
#include <vector>
using namespace std;

class Sieves : public ::testing::TestWithParam<unsigned long long> {
protected:
  static map<string, double> speed_check;
  static vector<unsigned long long> primes;

public:
  static void load_primes(string filename) {
    ifstream in;
    in.open(filename);
    if (!in) {
      cerr << "Error open file: " << filename;
      exit(1);
    }
    vector<unsigned long long> primes;
    unsigned long long num;
    while (in >> num) {
      Sieves::primes.push_back(num);
    }
    in.close();
  }
  static void results() {
    cout << "Algorithms speed for 1000000:\n";
    for (auto i : speed_check) {
      cout << fixed << setprecision(2) << i.first << ": " << i.second * 1000
           << endl;
    }
  }
  bool check_primes(vector<unsigned long long> &generated) {
    for (size_t i = 0; i < generated.size(); i++) {
      if (generated[i] != primes[i]) {
        cout << "Pos: " << i << " Generated: " << generated[i]
             << " Control: " << primes[i] << endl;
        return false;
      }
    }
    return true;
  }
};
map<string, double> Sieves::speed_check;
vector<unsigned long long> Sieves::primes;

TEST_P(Sieves, SimpleSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfEratosthenes(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Simple sieve"] = elapsed_ms.count();
}

TEST_P(Sieves, UnlimitedSieve) {
  unsigned long long val = GetParam();
  sieve::UnlimitedSieve sieve;
  unsigned long long prime;
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes;
  while ((prime = sieve.nextPrime()) < val) {
    primes.push_back(prime);
  }

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Unlimited sieve"] = elapsed_ms.count();
}

TEST_P(Sieves, SkipSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfEratosthenes_skip(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Sieve with skiping even"] = elapsed_ms.count();
}

TEST_P(Sieves, WheelFactorization) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes =
      sieve::SieveOfEratosthenes_wheel_factorization(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Sieve with wheel factorization"] = elapsed_ms.count();
}

TEST_P(Sieves, SegmentedSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfEratosthenes_segmented(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Segmented sieve"] = elapsed_ms.count();
}

TEST_P(Sieves, LinearSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfEratosthenes_linear(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Linear time sieve"] = elapsed_ms.count();
}

TEST_P(Sieves, ParallelSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfEratosthenes_parallel(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Parallel sieve"] = elapsed_ms.count();
}

TEST_P(Sieves, AtkhinSieve) {
  unsigned long long val = GetParam();
  chrono::duration<double, milli> elapsed_ms;

  auto start = chrono::high_resolution_clock::now();

  vector<unsigned long long> primes = sieve::SieveOfAtkhin(val);

  auto end = chrono::high_resolution_clock::now();
  elapsed_ms = end - start;

  ASSERT_TRUE(check_primes(primes));

  cout << fixed << "Sieve " << val << " for " << elapsed_ms.count() << " ms\n";
  speed_check["Atkhin sieve"] = elapsed_ms.count();
}

INSTANTIATE_TEST_SUITE_P(SievesTest, Sieves,
                         ::testing::Values(0, 1, 2, 3, 4, 5, 30, 1000, 10000,
                                           1000000));

class TimingListener : public ::testing::EmptyTestEventListener {
public:
  void OnTestProgramStart(const testing::UnitTest &unit_test) override {
    Sieves::load_primes("primes_chk");
  }
  void OnTestProgramEnd(const ::testing::UnitTest &unit_test) override {
    Sieves::results();
  }
};

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  // Добавляем наш слушатель
  listeners.Append(new TimingListener);
  return RUN_ALL_TESTS();
}
