#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/ASTPasses.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(ASTAnalysisTest, DotGraph) {
  Scanner scanner(test_prefix + "parser/fundecls.c");
  Lexer lexer(scanner);
  Parser parser(lexer);
  Program progAST = parser.parse();

  std::cout.rdbuf( NULL );
  DotGraph dotGraph(progAST, &std::cout);
  dotGraph.print();
}

// The fixture for testing class Project1. From google test primer.
class Test_ASTAnalysis : public ::testing::Test {
protected:
  Test_ASTAnalysis() {
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