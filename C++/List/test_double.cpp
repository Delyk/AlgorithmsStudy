#include "list.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(SimpleTest, test_one) { ASSERT_EQ(); }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
