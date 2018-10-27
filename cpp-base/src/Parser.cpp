#include "../include/Parser.h"

/* Public */

ACC::Parser::Parser(const ACC::Tokeniser &tokeniser)
    : currToken(ACC::Token::TokenClass::INVALID, -1, -1), tokeniser(tokeniser) {
}

ACC::Program ACC::Parser::parse() { return parseProgram(); }

/* Token Operations */

bool ACC::Parser::accept(ACC::Token::TokenClass expected) {
  return expected == currToken.tokenClass;
}

ACC::Token ACC::Parser::expect(ACC::Token::TokenClass expected) {
  if (expected == currToken.tokenClass) {
    ACC::Token temp = currToken;
    nextToken();
    return temp;
  }
  return currToken;
}

ACC::Token ACC::Parser::lookAhead(int i) {
  assert(i > 0);
  while (tokenBuffer.size() < i) {
    tokenBuffer.push_back(tokeniser.nextToken());
  }

  return tokenBuffer[i - 1];
}

void ACC::Parser::nextToken() {
  // Use Buffer
  if (tokenBuffer.size() != 0) {
    currToken = tokenBuffer.front();
    tokenBuffer.erase(tokenBuffer.begin());
  } else {
    currToken = tokeniser.nextToken();
  }
}

/* AST Construction */

ACC::Program ACC::Parser::parseProgram() {
  parseIncludes();
  std::vector<ACC::StructTypeDecl> stds = parseStructTypeDecls();
  std::vector<ACC::VarDecl> vds = parseVarDecls();
  std::vector<ACC::FunDecl> fds = parseFunDecls();
  expect(ACC::Token::TokenClass::ENDOFFILE);
  return ACC::Program(fds, stds, vds);
}

std::vector<ACC::FunDecl> ACC::Parser::parseFunDecls() {
  /* @TODO */
  std::vector<ACC::FunDecl> output;
  return output;
}

std::vector<ACC::StructTypeDecl> ACC::Parser::parseStructTypeDecls() {
  /* @TODO */
  std::vector<ACC::StructTypeDecl> output;
  return output;
}

std::vector<ACC::VarDecl> ACC::Parser::parseVarDecls() {
  /* @TODO */
  std::vector<ACC::VarDecl> output;
  return output;
}

void ACC::Parser::parseIncludes() {
  if (accept(ACC::Token::TokenClass::INCLUDE)) {
    expect(ACC::Token::TokenClass::INCLUDE);
    expect(ACC::Token::TokenClass::STRING_LITERAL);
    parseIncludes();
  }
}