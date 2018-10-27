#pragma once

#include <string>
#include <vector>


#include "AST.h"
#include "Tokeniser.h"

namespace ACC {

class Parser {

public:
  Parser(const ACC::Tokeniser &tokeniser);

  /* Parses the Tokens into an AST with root node ACC::Program. */
  ACC::Program parse();

private:
  ACC::Token currToken;
  std::vector<ACC::Token> tokenBuffer;
  ACC::Tokeniser tokeniser;

  /* Check if we can expect the provided Token to be next. */
  bool accept(ACC::Token::TokenClass expected);

  /* Expect the next Token to be of a certain type. Throws if not. */
  ACC::Token expect(ACC::Token::TokenClass expected);

  /* Looks ahead 'i' number of Tokens. */
  ACC::Token lookAhead(int i);
  
  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* Parsing */
  
  void parseIncludes();
  ACC::Program parseProgram();
  std::vector<ACC::FunDecl> parseFunDecls();
  std::vector<ACC::StructTypeDecl> parseStructTypeDecls();
  std::vector<ACC::VarDecl> parseVarDecls();

  /* @TODO Parse the rest of the AST. */

};

}; // namespace ACC
