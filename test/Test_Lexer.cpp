#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "Lexer.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "SourceToken.h"

class Test_Lexer : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_Lexer() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = test_root + "Test_Lexer/" + test_name + "/test.cpp";
  }
};

using namespace ACC;

TEST_F(Test_Lexer, AllTokens) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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

TEST_F(Test_Lexer, FunDecl) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
}

TEST_F(Test_Lexer, NestedComments) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
}

TEST_F(Test_Lexer, VarDecls) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);

  SourceToken::Class currToken = SourceToken::Class::INVALID;
  while (currToken != SourceToken::Class::ENDOFFILE) {
    currToken = lexer.nextToken().tokenClass;
  }
}
