#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Lexer.h"
#include "../include/Scanner.h"
#include "../include/Token.h"

using namespace ACC;

// std::string test_prefix =
    // "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(LexerTest, AllTokens) {
  Scanner scanner("alltokens.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

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
    currToken = lexer.nextToken().tokenClass;
    actual.push_back(currToken);
  }

  ASSERT_EQ(expected.size(), actual.size());

  for (int i = 0; i < actual.size(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(LexerTest, InvalidIdentifier) {
  Scanner scanner("errors.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

  ASSERT_EQ(Token::TokenClass::INT_LITERAL, lexer.nextToken().tokenClass);
  ASSERT_EQ(Token::TokenClass::IDENTIFIER, lexer.nextToken().tokenClass);
}

TEST(LexerTest, InvalidIncludes) {
  Scanner scanner("inclood.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

  try {
    Token::TokenClass currToken = Token::TokenClass::INVALID;
    while (currToken != Token::TokenClass::ENDOFFILE) {
      currToken = lexer.nextToken().tokenClass;
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
  Scanner scanner("include_error.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

  try {
    Token::TokenClass currToken = Token::TokenClass::INVALID;
    while (currToken != Token::TokenClass::ENDOFFILE) {
      currToken = lexer.nextToken().tokenClass;
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
  Scanner scanner("nested_comments.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

  Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, VarDecls) {
  Scanner scanner("vardecl.c", test_prefix + "parser/");
  Lexer lexer(scanner);

  Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, SingleLetterVar) {
  Scanner scanner("singlelettervar.c", test_prefix + "lexer/");
  Lexer lexer(scanner);

  std::vector<Token> expectedTokens = {
      Token(Token::TokenClass::INT, 1, 1),
      Token(Token::TokenClass::IDENTIFIER, 1, 4),
      Token(Token::TokenClass::SC, 1, 5),
      Token(Token::TokenClass::ENDOFFILE, 1, 6)};

  std::vector<Token> actualTokens;
  Token currToken(Token::TokenClass::INVALID, 0, 0);
  while (currToken.tokenClass != Token::TokenClass::ENDOFFILE) {
    currToken = lexer.nextToken();
    actualTokens.push_back(currToken);
  }

  ASSERT_EQ(actualTokens.size(), expectedTokens.size());
  for (int i = 0; i < actualTokens.size(); i++) {
    ASSERT_TRUE(actualTokens[i] == expectedTokens[i]);
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, FunDecl) {
  Scanner scanner("fundecl.c", test_prefix + "parser/");
  Lexer lexer(scanner);

  Token::TokenClass currToken = Token::TokenClass::INVALID;
  while (currToken != Token::TokenClass::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
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