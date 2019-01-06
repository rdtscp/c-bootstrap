#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Parser.h"
#include "../include/passes/DotGraph.h"
#include "../include/passes/NameAnalysis.h"
#include "../include/passes/Optimiser.h"
#include "../include/passes/TypeAnalysis.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
std::string test_prefix = "../../../compiler/test/tests/";

TEST(OptimiserTest, FunDeclsC) {
  const std::string abspath = (test_prefix + "/parser/fundecls.c");
  ACC::Scanner scanner(abspath);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<ACC::Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);
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