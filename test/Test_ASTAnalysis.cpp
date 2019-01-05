#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Parser.h"
#include "../include/Preprocessor.h"
#include "../include/SourceCode.h"
#include "../include/passes/DotGraph.h"
#include "../include/passes/NameAnalysis.h"
#include "../include/passes/TypeAnalysis.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(ASTAnalysisTest, DotGraph) {
  Preprocessor preprocessor(test_prefix + "/parser/fundecls.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST);
  dotGraph.print();
}

TEST(ASTAnalysisTest, NameAnalysis) {
  Preprocessor preprocessor(test_prefix + "/parser/fundecls.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateFunction) {
  Preprocessor preprocessor(test_prefix + "sem/dupefun.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateVariable) {
  Preprocessor preprocessor(test_prefix + "sem/dupevar.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, AmbiguousIdentifier) {
  Preprocessor preprocessor(test_prefix + "sem/ambig.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, NoMainFunc) {
  Preprocessor preprocessor(test_prefix + "sem/nomain.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, Fibonacci) {
  Preprocessor preprocessor(test_prefix + "fibonacci.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);
}

TEST(ASTAnalysisTest, TicTacToe) {
  Preprocessor preprocessor(test_prefix + "tictactoe.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);
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