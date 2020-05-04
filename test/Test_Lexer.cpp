#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "gtest/gtest.h"
#include "TestPath.h"

#include "Lexer.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "SourceToken.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_Lexer/";

TEST(Test_Lexer, AllTokens) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "AllTokens/test.txt");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  atl::vector<SourceToken::Class> expected = {
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

  atl::vector<SourceToken::Class> actual;

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
    actual.push_back(currToken);
  }

  ASSERT_EQ(expected.size(), actual.size());

  for (unsigned int i = 0; i < actual.size(); ++i) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(Test_Lexer, FunDecl) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "FunDecl/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
}

TEST(Test_Lexer, NestedComments) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NestedComments/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
}

TEST(Test_Lexer, VarDecls) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "VarDecls/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
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