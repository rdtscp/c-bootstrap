#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Scanner.h"
#include "../include/Token.h"
#include "../include/Tokeniser.h"

using namespace ACC;

std::string test_prefix = "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/cpp-base/test/tests/";
// std::string test_prefix = "../../test/tests/";

TEST(LexerTest, AllTokens) {
  Scanner scanner(test_prefix + "lexer/alltokens.c");
  Tokeniser tokeniser(scanner);

  std::vector<Token::TokenClass> expected = {Token::TokenClass::IDENTIFIER,
                                             Token::TokenClass::ASSIGN,
                                             Token::TokenClass::LBRA,
                                             Token::TokenClass::RBRA,
                                             Token::TokenClass::LPAR,
                                             Token::TokenClass::RPAR,
                                             Token::TokenClass::LSBR,
                                             Token::TokenClass::RSBR,
                                             Token::TokenClass::SC,
                                             Token::TokenClass::COMMA,
                                             Token::TokenClass::INT,
                                             Token::TokenClass::VOID,
                                             Token::TokenClass::CHAR,
                                             Token::TokenClass::IF,
                                             Token::TokenClass::ELSE,
                                             Token::TokenClass::WHILE,
                                             Token::TokenClass::RETURN,
                                             Token::TokenClass::STRUCT,
                                             Token::TokenClass::SIZEOF,
                                             Token::TokenClass::INCLUDE,
                                             Token::TokenClass::IDENTIFIER,
                                             Token::TokenClass::STRING_LITERAL,
                                             Token::TokenClass::STRING_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::INT_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::CHAR_LITERAL,
                                             Token::TokenClass::AND,
                                             Token::TokenClass::OR,
                                             Token::TokenClass::EQ,
                                             Token::TokenClass::NE,
                                             Token::TokenClass::LT,
                                             Token::TokenClass::GT,
                                             Token::TokenClass::LE,
                                             Token::TokenClass::GE,
                                             Token::TokenClass::PLUS,
                                             Token::TokenClass::MINUS,
                                             Token::TokenClass::ASTERIX,
                                             Token::TokenClass::DIV,
                                             Token::TokenClass::REM,
                                             Token::TokenClass::DOT,
                                             Token::TokenClass::ENDOFFILE};

  std::vector<Token::TokenClass> actual;

  Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = tokeniser.nextToken().tokenClass;
    actual.push_back(currToken);
  }

  ASSERT_EQ(expected.size(), actual.size());

  for (int i = 0; i < actual.size(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(LexerTest, InvalidIdentifier) {
  Scanner scanner(test_prefix + "lexer/errors.c");  
  Tokeniser tokeniser(scanner);

  ASSERT_EQ(Token::TokenClass::INT_LITERAL, tokeniser.nextToken().tokenClass);
  ASSERT_EQ(Token::TokenClass::IDENTIFIER, tokeniser.nextToken().tokenClass);
}

TEST(LexerTest, InvalidIncludes) {
  Scanner scanner(test_prefix + "lexer/inclood.c");
  Tokeniser tokeniser(scanner);

  try {
    Token::TokenClass currToken = Token::TokenClass::INVALID;
    while (currToken != Token::TokenClass::ENDOFFILE) {
      currToken = tokeniser.nextToken().tokenClass;
    }
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (std::exception const &err) {
    std::cout << "Expected a std::runtime_error, but got:" << err.what()
              << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, no exception thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(LexerTest, InvalidIncludes2) {
  Scanner scanner(test_prefix + "lexer/include_error.c");
  Tokeniser tokeniser(scanner);

  try {
    Token::TokenClass currToken = Token::TokenClass::INVALID;
    while (currToken != Token::TokenClass::ENDOFFILE) {
      currToken = tokeniser.nextToken().tokenClass;
    }
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (std::exception const &err) {
    std::cout << "Expected a std::runtime_error, but got:" << err.what()
              << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, no exception thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(LexerTest, NestedComments) {
  Scanner scanner(test_prefix + "lexer/nested_comments.c");
  Tokeniser tokeniser(scanner);

  Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = tokeniser.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, VarDecls) {
  Scanner scanner(test_prefix + "parser/vardecl.c");
  Tokeniser tokeniser(scanner);

	Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = tokeniser.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

// The fixture for testing class Project1. From google test primer.
class Test_Lexer : public ::testing::Test {
protected:
  Test_Lexer() {
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