#include "TestPath.h"
#include "gtest/gtest.h"

#include "atl/include/string.h"

#include "Preprocessor.h"
#include "SourceToken.h"

const atl::string tests_prefix = test_root + "Test_Preprocessor/";

class Test_Preprocessor : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_Preprocessor() {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = tests_prefix + test_name + "/test.cpp";
  }

  virtual void TearDown() { t_source_file = ""; }
};

using namespace ACC;

TEST_F(Test_Preprocessor, AdjacentInclude) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "AdjacentInclude/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "AdjacentInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + tests_prefix + "AdjacentInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, Construction) {
  const atl::shared_ptr<SourceMemHandler> src(
      new SourceMemHandler("int main(int argc, char **argv) { return 1; }\n"));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"RAW\"\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, FileExists) {
  ASSERT_TRUE(Preprocessor::fileExists(tests_prefix + "FileExists/test.cpp"));
  ASSERT_TRUE(Preprocessor::fileExists(tests_prefix + "FileExists/header.h"));
}

TEST_F(Test_Preprocessor, FormatIncludeDirective) {
  const atl::string actual =
      ACC::Preprocessor::formatIncludeDirective("path/to/the/header.h");
  const atl::string expect = "# 0 \"path/to/the/header.h\"";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, IncludeChildDir) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "IncludeChildDir/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "IncludeChildDir/subdir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "IncludeChildDir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, IncludeNormal) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "IncludeNormal/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "IncludeNormal/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "IncludeNormal/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, IncludeParentDir) {
  const atl::string filepath =
      tests_prefix + "IncludeParentDir/subdir/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  // const SourceHandler src(SourceHandler::Type::FILEPATH,
  //                         test_prefix + "preprocessor/test2.cpp");
  // ACC::Preprocessor preprocessor(src, {});
  // const SourceHandler pp_src = preprocessor.getSource();
  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "IncludeParentDir/subdir/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "IncludeParentDir/header.h\"\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "IncludeParentDir/subdir/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, NestedInclude) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "NestedInclude/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "NestedInclude/header.h\"\n";
  expect += "# 0 \"" + tests_prefix + "NestedInclude/header2.h\"\n";
  expect += "int header2() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "NestedInclude/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "NestedInclude/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}

TEST_F(Test_Preprocessor, PragmaOnce) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  const atl::shared_ptr<SourceMemHandler> pp_src = preprocessor.getSource();

  atl::string actual = pp_src->read();
  atl::string expect = "";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/test.cpp\"\n";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/header.h\"\n";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/header_derived.h\"\n";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "\n";
  expect += "int header_base() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "PragmaOnce/header_derived.h\"\n";
  expect += "\n";
  expect += "int header_derived() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "PragmaOnce/header.h\"\n";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/header_derived2.h\"\n";
  expect += "# 0 \"" + tests_prefix + "PragmaOnce/header_base.h\"\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "PragmaOnce/header_derived2.h\"\n";
  expect += "\n";
  expect += "int header_derived2() { return 1; }\n";
  expect += "\n";
  expect += "# 2 \"" + tests_prefix + "PragmaOnce/header.h\"\n";
  expect += "\n";
  expect += "int header() { return 1; }\n";
  expect += "\n";
  expect += "# 1 \"" + tests_prefix + "PragmaOnce/test.cpp\"\n";
  expect += "\n";
  expect += "int main(int argc, char **argv) { return 1; }\n";
  expect += "\n";

  std::string actual_val = actual.c_str();
  std::string expect_val = expect.c_str();

  ASSERT_EQ(actual_val, expect_val);
}
