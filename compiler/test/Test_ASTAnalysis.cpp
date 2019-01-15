#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"
#include "../include/passes/DotGraph.h"
#include "../include/passes/NameAnalysis.h"
#include "../include/passes/TypeAnalysis.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(ASTAnalysisTest, DotGraph) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "parser/fundecls.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST);
  dotGraph.print();
}

TEST(ASTAnalysisTest, NameAnalysis) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "parser/fundecls.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateFunction) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "sem/dupefun.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateVariable) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "sem/dupevar.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, AmbiguousIdentifier) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "sem/ambig.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, NoMainFunc) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "sem/nomain.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, Fibonacci) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "fibonacci.cpp");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);
}

TEST(ASTAnalysisTest, TicTacToe) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "tictactoe.c");
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

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