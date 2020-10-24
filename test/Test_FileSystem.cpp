#include "atl/include/string.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "FileSystem.h"

using namespace ACC;

const atl::string test_prefix = test_root;

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
