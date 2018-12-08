#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/ASTPasses.h"
#include "../include/CodeGeneration.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(GenerationTest, MIPS_Fibonacci) {
  Scanner scanner(test_prefix + "fibonacci.c");
  Lexer lexer(scanner);
  Parser parser(lexer);
  Program progAST = parser.parse();

  GenerateMIPS mipsGenerator(progAST, "./fibonacci_mips.asm");
  mipsGenerator.run();
}

// The fixture for testing class Project1. From google test primer.
class Test_Generation : public ::testing::Test {
protected:
  Test_Generation() {
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