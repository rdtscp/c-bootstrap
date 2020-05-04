#include "gtest/gtest.h"
#include "TestPath.h"

#include "atl/include/string.h"

#include "Preprocessor.h"
#include "SourceToken.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_Preprocessor/";

TEST(Test_Preprocessor, AdjacentInclude) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "AdjacentInclude/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "AdjacentInclude/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "AdjacentInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + test_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, Construction) {
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

TEST(Test_Preprocessor, FileExists) {
  ASSERT_TRUE(Preprocessor::fileExists(test_prefix + "FileExists/test.cpp"));
  ASSERT_TRUE(Preprocessor::fileExists(test_prefix + "FileExists/header.h"));
}

TEST(Test_Preprocessor, FormatIncludeDirective) {
  const atl::string actual =
      ACC::Preprocessor::formatIncludeDirective("path/to/the/header.h");
  const atl::string expect = "# 0 \"path/to/the/header.h\"";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, IncludeChildDir) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "IncludeChildDir/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "IncludeChildDir/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "IncludeChildDir/subdir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "IncludeChildDir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, IncludeNormal) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "IncludeNormal/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "IncludeNormal/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "IncludeNormal/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "IncludeNormal/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, IncludeParentDir) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "IncludeParentDir/subdir/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  // const SourceHandler src(SourceHandler::Type::FILEPATH,
  //                         test_prefix + "preprocessor/test2.cpp");
  // ACC::Preprocessor preprocessor(src, {});
  // const SourceHandler pp_src = preprocessor.getSource();
  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "IncludeParentDir/subdir/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "IncludeParentDir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "IncludeParentDir/subdir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, NestedInclude) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NestedInclude/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "NestedInclude/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "NestedInclude/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "NestedInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "NestedInclude/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "NestedInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST(Test_Preprocessor, PragmaOnce) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "PragmaOnce/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  const SourceHandler pp_src = preprocessor.getSource();

  atl::string actual = pp_src.value;
  atl::string expect = "";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/test.cpp\"\n";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/header_derived.h\"\n";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "\n";
  expect += "int header_base() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "PragmaOnce/header_derived.h\"\n";
  expect += "\n";
  expect += "int header_derived() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "PragmaOnce/header.h\"\n";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/header_derived2.h\"\n";
  expect += "# 0 \"" + test_prefix + "PragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "PragmaOnce/header_derived2.h\"\n";
  expect += "\n";
  expect += "int header_derived2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + test_prefix + "PragmaOnce/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + test_prefix + "PragmaOnce/test.cpp\"\n";
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