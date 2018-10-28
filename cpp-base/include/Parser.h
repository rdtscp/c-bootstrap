#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Token.h"
#include "Tokeniser.h"

namespace ACC {

class Parser {

public:
  Parser(const Tokeniser &tokeniser);

  /* Parses the Tokens into an AST with root node Program. */
  Program parse();

private:

  Token currToken;
  std::vector<Token> tokenBuffer;
  Tokeniser tokeniser;

  /* Check if we can expect the provided Token to be next. */
  bool accept(Token::TokenClass expected);

  /* Check if we can expect one of the provided Tokens to be next. */
  bool accept(std::vector<Token::TokenClass> expected);

  /* Expect the next Token to be of a certain type. Throws if not. */
  Token expect(Token::TokenClass expected);

  /* Expect the next Token to be one of a list of types. Throws if not. */
  Token expect(std::vector<Token::TokenClass> expected);

  /* Looks ahead 'i' number of Tokens. */
  Token lookAhead(int i);

  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* Parsing */

  std::shared_ptr<Type> expectType();
  VarDecl expectVarDecl();

  void parseIncludes();
  Program parseProgram();
  std::vector<FunDecl> parseFunDecls();
  std::vector<StructTypeDecl> parseStructDecls();
  std::vector<VarDecl> parseVarDecls();

  /* @TODO Parse the rest of the AST. */

  /* Helpers */

  /* Converts an Token to an Type */
  std::shared_ptr<BaseType> tokenToType(const Token::TokenClass &tc);
};

}; // namespace ACC
