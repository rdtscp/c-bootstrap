#include "atl/include/string.h"

#include <iostream>

#include "gtest/gtest.h"

#include "Preprocessor.h"
#include "SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(PreprocessorTest, TestConstruction) {
  ACC::Preprocessor preprocessor({});
  const SourceHandler pp_src =
      preprocessor.getSource(SourceHandler(SourceHandler::Type::RAW, "foo"));
  ASSERT_EQ(pp_src.type, SourceHandler::Type::RAW);
  ASSERT_EQ(std::string(pp_src.value.c_str()), "# 1 \"RAW\"\nfoo");
}

TEST(PreprocessorTest, TestFormatIncludeDirective) {
  ACC::Preprocessor preprocessor({});
  const atl::string formattedInclude =
      ACC::Preprocessor::formatIncludeDirective("test/tests/scanner/header.h");
  ASSERT_EQ(std::string(formattedInclude.c_str()),
            "# 1 \"test/tests/scanner/header.h\"");
}

TEST(PreprocessorTest, TestFileExists) {
  ASSERT_TRUE(Preprocessor::fileExists(test_prefix + "preprocessor/test1.cpp"));
  ASSERT_TRUE(Preprocessor::fileExists(test_prefix + "preprocessor/header1.h"));
}

TEST(PreprocessorTest, TestInclude) {
  ACC::Preprocessor preprocessor({});
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "preprocessor/test1.cpp");
  const SourceHandler pp_src = preprocessor.getSource(src);

  atl::string actual_val = pp_src.value.c_str();
  atl::string expect_val =
      "# 1 \"" + test_prefix + +"preprocessor/test1.cpp\"\n# 1 \"" +
      test_prefix +
      +"preprocessor/header1.h\"\nint header1() { return 1; }\n# 2 \"" +
      test_prefix + +"preprocessor/test1.cpp\"\nint test1() { return 1; }";

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestIncludeChildDir) {
  ACC::Preprocessor preprocessor({});
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "preprocessor/test2.cpp");
  const SourceHandler pp_src = preprocessor.getSource(src);

  atl::string actual_val = pp_src.value.c_str();
  atl::string expect_val =
      "# 1 \"" + test_prefix + +"preprocessor/test2.cpp\"\n# 1 \"" +
      test_prefix +
      +"preprocessor/other_dir/header2.h\"\nint header2() { return 1; }\n# 2 "
       "\"" +
      test_prefix + +"preprocessor/test2.cpp\"\nint test2() { return 1; }";

  ASSERT_EQ(actual_val, expect_val);
}

TEST(PreprocessorTest, TestIncludeParentDir) {
  ACC::Preprocessor preprocessor({});
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "preprocessor/other_dir/test3.cpp");
  const SourceHandler pp_src = preprocessor.getSource(src);

  atl::string actual_val = pp_src.value.c_str();
  atl::string expect_val =
      "# 1 \"" + test_prefix + +"preprocessor/other_dir/test3.cpp\"\n# 1 \"" +
      test_prefix +
      +"preprocessor/header3.h\"\nint header3() { return 1; }\n# 2 \"" +
      test_prefix +
      +"preprocessor/other_dir/test3.cpp\"\nint test3() { return 1; }";

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