#include "sort.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <chrono>
#include <functional>
#include <gtest/gtest.h>
#include <iomanip>
#include <ios>
#include <random>
#include <ratio>
#include <vector>
#define TEMPLATE_PARAMS                                                        \
  typename BidirectIt,                                                         \
      typename Compare =                                                       \
          std::less<typename std::iterator_traits<BidirectIt>::value_type>
#define TEMPLATE_PARAMS_RANDOM                                                 \
  typename RandomIt,                                                           \
      typename Compare =                                                       \
          std::less<typename std::iterator_traits<RandomIt>::value_type>
#define SORT_PARAMS BidirectIt first, BidirectIt last, Compare comp = Compare()
#define SORT_PARAMS_RANDOM                                                     \
  RandomIt first, RandomIt last, Compare comp = Compare()
#define TEST_COUNT (6 * 6)
using namespace sort;

// Обёртки алгоритмов сортировки
struct SelectionSortWrapper {
  static std::chrono::duration<double, std::milli> total_ms;
  template <TEMPLATE_PARAMS> void operator()(SORT_PARAMS) const {
    SelectionSort(first, last, comp);
  }
  static void totalTime() {
    std::cout << "SelectionSort (avg): " << std::fixed << std::setprecision(1)
              << total_ms.count() / TEST_COUNT << " ms" << std::endl;
  }
};

struct QuickSortWrapper {
  static std::chrono::duration<double, std::milli> total_ms;
  template <TEMPLATE_PARAMS_RANDOM> void operator()(SORT_PARAMS_RANDOM) const {
    QuickSort(first, last, comp);
  }
  static void totalTime() {
    std::cout << "QuickSort (avg): " << std::fixed << std::setprecision(1)
              << total_ms.count() / TEST_COUNT << " ms" << std::endl;
  }
};
std::chrono::duration<double, std::milli> SelectionSortWrapper::total_ms{0};
std::chrono::duration<double, std::milli> QuickSortWrapper::total_ms{0};

// Список алгоритмов сортировки
using SortAlgorithms = ::testing::Types<SelectionSortWrapper, QuickSortWrapper>;

// Функция проверки сортировки
template <typename Sorter, typename Container, typename Compare = std::less<>>
void TestSort(const Sorter &sorter, const Container &input,
              Compare comp = Compare()) {
  size_t count = input.size();
  Container control = input;
  Container data = control;

  auto start = std::chrono::high_resolution_clock::now();

  sorter(data.begin(), data.end(), comp);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed_ms = end - start;

  EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), comp));
  std::sort(control.begin(), control.end(), comp);
  EXPECT_EQ(control, data);

  std::cout << std::fixed << "Sorted: " << count
            << " Time: " << elapsed_ms.count() << " ms\n";
  sorter.total_ms += elapsed_ms;
  testing::Test::RecordProperty("elements_sorted", static_cast<int>(count));
  testing::Test::RecordProperty("duration_ms", elapsed_ms.count());
}

// Шаблонный класс для каждого алгоритма
template <typename Sorter> class SortTest : public ::testing::Test {
public:
  Sorter sorter;

  static int random(int start, int end) {
    std::random_device rd;  // Устройство случайности
    std::mt19937 gen(rd()); // Генератор
    std::uniform_int_distribution<> dist(start, end); // Диапазон распределения
    return dist(gen);
  }

  static double random(double start, double end) {
    std::random_device rd;  // Устройство случайности
    std::mt19937 gen(rd()); // Генератор
    std::uniform_real_distribution<> dist(start, end); // Диапазон распределения
    return dist(gen);
  }

  static char random() {
    std::random_device rd;  // Устройство случайности
    std::mt19937 gen(rd()); // Генератор
    std::uniform_int_distribution<> dist(-128, 128); // Диапазон распределения
    return dist(gen);
  }

  static std::vector<std::vector<int>> datasetsInt() {
    std::vector<int> bigAssArray1, bigAssArray2, randomArr;
    for (int i = 1000, j = 0; i >= 0 && j < 1000; i--, j++) {
      bigAssArray1.push_back(i);
      bigAssArray2.push_back(j);
    }
    for (int i = 0; i < 100; i++) {
      randomArr.push_back(random(-100, 100));
    }
    return {{},
            {1},                // один элемент
            {5, 4, 3, 2, 1},    // обратный порядок
            {3, 5, 3, 1, 2, 5}, // с дубликатами
            randomArr,
            bigAssArray1,
            bigAssArray2};
  }

  static std::vector<std::vector<double>> datasetsDouble() {
    std::vector<double> bigAssArray1, bigAssArray2, randomArr;
    for (int i = 1000, j = 0; i >= 0 && j < 1000; i--, j++) {
      bigAssArray1.push_back(random(-1000.0, 1000.0));
      bigAssArray2.push_back(random(-1000.0, 1000.0));
    }
    for (int i = 0; i < 100; i++) {
      randomArr.push_back(random(-100.0, 100.0));
    }
    return {{},
            {1.2},
            {5.6, 4.0, 3.14, 2.999999, 1.5, 0.3},
            {5.5, 5.5, 0.9, 5.5, 5.5},
            randomArr,
            bigAssArray1,
            bigAssArray2};
  }

  static std::vector<std::string> datasetsString() {
    std::string bigAssArray1, bigAssArray2, randomArr;
    for (int i = 1000, j = 0; i >= 0 && j < 1000; i--, j++) {
      bigAssArray1.push_back(random());
      bigAssArray2.push_back(random());
    }
    for (int i = 0; i < 100; i++) {
      randomArr.push_back(random());
    }
    return {std::string{}, std::string("a"), std::string("fedcba"),
            randomArr,     bigAssArray1,     bigAssArray2};
  }
};
//Слушатель для вывода информации после всех тестов
class TimingListener : public ::testing::EmptyTestEventListener {
public:
  void OnTestProgramEnd(const ::testing::UnitTest &unit_test) override {
    SelectionSortWrapper::totalTime();
    QuickSortWrapper::totalTime();
  }
};

TYPED_TEST_SUITE(SortTest, SortAlgorithms);

TYPED_TEST(SortTest, SortAscendingInt) {
  for (const auto &data : SortTest<TypeParam>::datasetsInt()) {
    TestSort(this->sorter, data, std::less<int>{});
  }
}

TYPED_TEST(SortTest, SortDescendingInt) {
  for (const auto &data : SortTest<TypeParam>::datasetsInt()) {
    TestSort(this->sorter, data, std::greater<int>{});
  }
}

TYPED_TEST(SortTest, SortAscendingDouble) {
  for (const auto &data : SortTest<TypeParam>::datasetsDouble()) {
    TestSort(this->sorter, data, std::less<double>{});
  }
}

TYPED_TEST(SortTest, SortDescendingDouble) {
  for (const auto &data : SortTest<TypeParam>::datasetsDouble()) {
    TestSort(this->sorter, data, std::greater<double>{});
  }
}

TYPED_TEST(SortTest, SortAscendingString) {
  for (const auto &data : SortTest<TypeParam>::datasetsString()) {
    TestSort(this->sorter, data, std::less<char>{});
  }
}

TYPED_TEST(SortTest, SortDescendingString) {
  for (const auto &data : SortTest<TypeParam>::datasetsString()) {
    TestSort(this->sorter, data, std::greater<char>{});
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  // Удаляем стандартный слушатель вывода (по желанию)
  // delete listeners.Release(listeners.default_result_printer());

  // Добавляем наш слушатель
  listeners.Append(new TimingListener);

  return RUN_ALL_TESTS();
}
