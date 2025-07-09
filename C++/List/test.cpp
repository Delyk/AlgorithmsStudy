#include "list.h"
#include <gtest/gtest.h>
using namespace list;

TEST(SimpleTest, Basic) {
  single_list<int> list = {1, 2, 3};
  for (auto i : list) {
    std::cout << i << std::endl;
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
