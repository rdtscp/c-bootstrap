#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "FileSystem.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(FileSystemTest, TestParentDir) {
  ASSERT_EQ(FileSystem::parentDir("/usr/local/include/header.h"),
            "/usr/local/include/");

  ASSERT_EQ(FileSystem::parentDir("/usr/local/include/"), "/usr/local/");

  ASSERT_EQ(FileSystem::parentDir("/usr/local/include"), "/usr/local/");

  ASSERT_EQ(FileSystem::parentDir(FileSystem::parentDir("/usr/local/include")),
            "/usr/");
}

TEST(FileSystemTest, TestResolveRelativePath) {
  const atl::string currFilepath = "/path/to/some/directory/file.cpp";
  const atl::string relativePath = "../../header.h";
  const atl::string expected = "/path/to/header.h";
  const atl::string actual =
      FileSystem::resolveRelativePath(currFilepath, relativePath);
  ASSERT_EQ(actual, expected);
}

// The fixture for testing class Project1. From google test primer.
class Test_Scanner : public ::testing::Test {
protected:
  Test_Scanner() {
    // You can do set-up work for each test here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
};