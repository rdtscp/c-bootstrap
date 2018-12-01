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

TEST(PreprocessorTest, IncludeWorks) {
  Scanner scanner(test_prefix + "preprocessor/prog.c");
  Lexer lexer(scanner);
  Parser parser(lexer);
  Program progAST = parser.parse();

  NameAnalysis nameAnalysis(progAST, &std::cout);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, NestedIncludes) {
  Scanner scanner(test_prefix + "preprocessor/prog3.c");
  Lexer lexer(scanner);
  Parser parser(lexer);
  Program progAST = parser.parse();

  NameAnalysis nameAnalysis(progAST, &std::cout);
  nameAnalysis.run();

  ASSERT_EQ(progAST.decls.size(), 3);
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, IfNDef) {
  Scanner scanner(test_prefix + "preprocessor/prog2.c");
  Lexer lexer(scanner);
  Parser parser(lexer);
  Program progAST = parser.parse();

  NameAnalysis nameAnalysis(progAST, &std::cout);
  nameAnalysis.run();

  ASSERT_EQ(progAST.decls.size(), 2);
  ASSERT_EQ(0, nameAnalysis.errorCount);
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