#include "atl/include/string.h"
#include <vector>

#include "gtest/gtest.h"

#include "../include/Lexer.h"
#include "../include/Scanner.h"
#include "../include/SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(LexerTest, AllTokens) {
  const atl::string filepath = test_prefix + "lexer/alltokens.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);

  std::vector<SourceToken::Class> expected = {
      SourceToken::Class::IDENTIFIER,
      SourceToken::Class::ASSIGN,
      SourceToken::Class::LBRA,
      SourceToken::Class::RBRA,
      SourceToken::Class::LPAR,
      SourceToken::Class::RPAR,
      SourceToken::Class::LSBR,
      SourceToken::Class::RSBR,
      SourceToken::Class::SC,
      SourceToken::Class::COMMA,
      SourceToken::Class::INT,
      SourceToken::Class::VOID,
      SourceToken::Class::CHAR,
      SourceToken::Class::IF,
      SourceToken::Class::ELSE,
      SourceToken::Class::WHILE,
      SourceToken::Class::RETURN,
      SourceToken::Class::STRUCT,
      SourceToken::Class::SIZEOF,
      SourceToken::Class::IDENTIFIER,
      SourceToken::Class::STRING_LITERAL,
      SourceToken::Class::STRING_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::INT_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::CHAR_LITERAL,
      SourceToken::Class::AND,
      SourceToken::Class::OR,
      SourceToken::Class::EQ,
      SourceToken::Class::NE,
      SourceToken::Class::LT,
      SourceToken::Class::GT,
      SourceToken::Class::LE,
      SourceToken::Class::GE,
      SourceToken::Class::PLUS,
      SourceToken::Class::MINUS,
      SourceToken::Class::ASTERIX,
      SourceToken::Class::DIV,
      SourceToken::Class::REM,
      SourceToken::Class::DOT,
      SourceToken::Class::ENDOFFILE};

  std::vector<SourceToken::Class> actual;

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
    actual.push_back(currToken);
  }

  ASSERT_EQ(expected.size(), actual.size());

  for (int i = 0; i < actual.size(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(LexerTest, NestedComments) {
  const atl::string filepath = test_prefix + "lexer/nested_comments.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, VarDecls) {
  const atl::string filepath = test_prefix + "parser/vardecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }

  ASSERT_TRUE(true);
}

TEST(LexerTest, FunDecl) {
  const atl::string filepath = test_prefix + "parser/fundecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
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