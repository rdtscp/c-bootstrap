#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Scanner.h"
#include "../include/Token.h"
#include "../include/Lexer.h"

using namespace ACC;

std::string test_prefix = "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/cpp-base/test/tests/";
// std::string test_prefix = "../../test/tests/";

TEST(ScannerTest, TestAPI) {
  Scanner scanner(test_prefix + "scanner/scanner.c");

  std::string output;
  ASSERT_EQ(scanner.column, 1);
  ASSERT_EQ(scanner.line, 1);

  output += scanner.next();
  ASSERT_EQ(output, "1");
  ASSERT_EQ(scanner.peek(), '2');

  output += scanner.next();
  ASSERT_EQ(output, "12");
  ASSERT_EQ(scanner.peek(), '3');

  output += scanner.next();
  ASSERT_EQ(output, "123");
  ASSERT_EQ(scanner.peek(), '4');

  output += scanner.next();
  ASSERT_EQ(output, "1234");
  ASSERT_EQ(scanner.peek(), '5');

  output += scanner.next();
  ASSERT_EQ(output, "12345");
  ASSERT_EQ(scanner.peek(), '\n');

  output += scanner.next();
  ASSERT_EQ(output, "12345\n");
  ASSERT_EQ(scanner.peek(), '6');

  scanner.next();
  scanner.next();

  ASSERT_EQ(scanner.next(), '8');
  ASSERT_EQ(scanner.next(), '9');
  
  ASSERT_EQ(scanner.next(), 'a');
  ASSERT_EQ(scanner.next(), '\n');
  ASSERT_EQ(scanner.next(), '\n');
  ASSERT_EQ(scanner.next(), ' ');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
}

// The fixture for testing class Project1. From google test primer.
class Test_Scanner : public ::testing::Test {
protected:
  Test_Scanner() {
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