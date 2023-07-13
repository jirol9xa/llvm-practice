#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parser.hpp>

TEST(UnitTests, isPIC) {
  Parser psr("../test/pic/test_binary", "../test/pic/Graph.txt",
             "../test/pic/maps.txt");

  EXPECT_EQ(psr.isPIC(), true);
}

TEST(UnitTests, numOfLines) {
  Parser psr("../test/pic/test_binary", "../test/pic/Graph.txt",
             "../test/pic/maps.txt");

  ASSERT_EQ(psr.getNumOfLines(), 2);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}