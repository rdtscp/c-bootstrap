#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "passes/DotGraph.h"
#include "passes/NameAnalysis.h"
#include "passes/TypeAnalysis.h"

#include "Lexer.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "targets/GenerateX86.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/Test_CodeGeneration/";
atl::string test_prefix = "../../test/tests/Test_CodeGeneration/";

TEST(Test_CodeGeneration, X86_SimpleFuncs) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "X86_SimpleFuncs/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);

  GenerateX86 x86Generator(progAST, "./fibonacci_x86.s");
  x86Generator.run();
}

// The fixture for testing class Project1. From google test primer.
class Test_CodeGeneration : public ::testing::Test {
protected:
  Test_CodeGeneration() {
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