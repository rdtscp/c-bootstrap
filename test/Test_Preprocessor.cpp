#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "Preprocessor.h"
#include "SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(PreprocessorTest, TestConstruction) {
  const atl::string filepath = test_prefix + "scanner/scanner.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Preprocessor preprocessor(src);
  const SourceHandler pp_src = preprocessor.getSource();
  ASSERT_EQ(src.type, pp_src.type);
  ASSERT_EQ(src.value, pp_src.value);
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