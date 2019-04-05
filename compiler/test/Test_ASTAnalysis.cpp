#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "Lexer.h"
#include "Parser.h"
#include "Scanner.h"
#include "passes/DotGraph.h"
#include "passes/NameAnalysis.h"
#include "passes/TypeAnalysis.h"

using namespace ACC;

atl::string test_prefix =
    "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
// atl::string test_prefix = "../../test/tests/";

TEST(ASTAnalysisTest, DotGraph) {
  const atl::string filepath = test_prefix + "parser/fundecls.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST);
  dotGraph.print();
}

TEST(ASTAnalysisTest, NameAnalysis) {
  const atl::string filepath = test_prefix + "parser/fundecls.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateFunction) {
  const atl::string filepath = test_prefix + "sem/dupefun.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  // ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DuplicateVariable) {
  const atl::string filepath = test_prefix + "sem/dupevar.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  // ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, AmbiguousIdentifier) {
  const atl::string filepath = test_prefix + "sem/ambig.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  ;
  nameAnalysis.run();
  // ASSERT_NE(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, NoMainFunc) {
  const atl::string filepath = test_prefix + "sem/nomain.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(ASTAnalysisTest, Fibonacci) {
  const atl::string filepath = test_prefix + "fibonacci.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  // ASSERT_EQ(0, typeAnalysis.errorCount);
}

TEST(ASTAnalysisTest, TicTacToe) {
  const atl::string filepath = test_prefix + "tictactoe.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  // ASSERT_EQ(0, typeAnalysis.errorCount);
}

TEST(ASTAnalysisTest, DotGraphClass) {
  const atl::string filepath = test_prefix + "parser/class.cpp";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  // ASSERT_EQ(0, nameAnalysis.errorCount);

  // TypeAnalysis typeAnalysis(progAST);
  // typeAnalysis.run();
  // ASSERT_EQ(0, typeAnalysis.errorCount);

  // DotGraph dotGraph(progAST);
  // dotGraph.print();
}

TEST(ASTTest, ClassTypeDecl_resolveThisVarExpr) {
  // Create Program node which has a class with one method.
  // Run name analysis and then assert that the member method has a `this`
  // variable.
  ASSERT_TRUE(false);
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