#include "atl/include/string.h"

#include "gtest/gtest.h"
#include "TestPath.h"

#include "Parser.h"
#include "Preprocessor.h"
#include "passes/DotGraph.h"
#include "passes/Optimiser.h"
#include "passes/SemanticAnalysis.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_Optimiser/";

TEST(Test_Optimiser, FunDecls) {
  const atl::string filepath = test_prefix + "FunDecls/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<ACC::Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  Optimiser optimiser(progAST);
  do {
    optimiser.run();
    optimiser.printOptimisations();
  } while (optimiser.optimisationsCount > 0);
}

// The fixture for testing class Project1. From google test primer.
class Test_Optimiser : public ::testing::Test {
protected:
  Test_Optimiser() {
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