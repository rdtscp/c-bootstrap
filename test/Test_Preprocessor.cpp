#include "gtest/gtest.h"

#include "atl/include/string.h"

#include "Preprocessor.h"
#include "SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/"
// "test/tests/PreprocessorTest/";
atl::string test_prefix = "../../test/tests/PreprocessorTest/";

TEST(PreprocessorTest, TestConstruction) {
  const SourceHandler src(SourceHandler::Type::RAW,
                          "int main(int argc, char **argv) { return 1; }\n");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();
  ASSERT_EQ(pp_src.type, SourceHandler::Type::RAW);

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"RAW\"\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestFormatIncludeDirective) {
  const atl::string actual =
      ACC::Preprocessor::formatIncludeDirective("path/to/the/header.h");
  const atl::string expect = "# 0 \"path/to/the/header.h\"";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestFileExists) {
  ASSERT_TRUE(
      Preprocessor::fileExists(test_prefix + "TestFileExists/test.cpp"));
  ASSERT_TRUE(
      Preprocessor::fileExists(test_prefix + "TestFileExists/header.h"));
  ASSERT_FALSE(
      Preprocessor::fileExists(test_prefix + "TestFileExists/test2.cpp"));
  ASSERT_FALSE(
      Preprocessor::fileExists(test_prefix + "TestFileExists/header2.h"));
}

TEST(PreprocessorTest, TestInclude) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestInclude/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestInclude/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestNestedInclude) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestNestedInclude/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestNestedInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestNestedInclude/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "TestNestedInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestNestedInclude/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestNestedInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestAdjacentInclude) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestAdjacentInclude/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestAdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestAdjacentInclude/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestAdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestAdjacentInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + test_prefix + "TestAdjacentInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestIncludeChildDir) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestIncludeChildDir/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestIncludeChildDir/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestIncludeChildDir/subdir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestIncludeChildDir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestIncludeParentDir) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestIncludeParentDir/subdir/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  // const SourceHandler src(SourceHandler::Type::FILEPATH,
  //                         test_prefix + "preprocessor/test2.cpp");
  // ACC::Preprocessor preprocessor(src, {});
  // const SourceHandler pp_src = preprocessor.getSource();
  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestIncludeParentDir/subdir/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestIncludeParentDir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestIncludeParentDir/subdir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestPragmaOnce) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TestPragmaOnce/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/header_derived.h\"\n";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "\n";
  expect += "int header_base() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestPragmaOnce/header_derived.h\"\n";
  expect += "\n";
  expect += "int header_derived() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestPragmaOnce/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/header_derived2.h\"\n";
  expect += "# 0 \"" + test_prefix + "TestPragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestPragmaOnce/header_derived2.h\"\n";
  expect += "\n";
  expect += "int header_derived2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + test_prefix + "TestPragmaOnce/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "TestPragmaOnce/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

// The fixture for testing class Project1. From google test primer.
class Test_Preprocessor : public ::testing::Test {
protected:
  Test_Preprocessor() {
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