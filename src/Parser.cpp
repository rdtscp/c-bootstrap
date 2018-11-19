#include <cassert>
#include <stdexcept>

#include "../include/Parser.h"

/* Public */

using namespace ACC;
using TC = Token::TokenClass;

Parser::Parser(const Lexer &lexer)
    : currToken(TC::INVALID, -1, -1), lexer(lexer) {}

Program Parser::parse() {
  nextToken();
  return parseProgram();
}

/* ---- Token Operations ---- */

bool Parser::accept(TC expected) { return expected == currToken.tokenClass; }

bool Parser::accept(std::vector<TC> expected) {
  bool output = false;
  for (const TC token : expected) {
    bool curr = accept(token);
    output |= curr;
  }
  return output;
}

Token Parser::expect(TC expected) {
  if (expected == currToken.tokenClass) {
    Token temp = currToken;
    nextToken();
    return temp;
  }
  throw std::runtime_error(
      "Parsing: Expected Token " + ACC::tokToStr(expected) + " at " +
      currToken.position.toString() +
      " but found: " + ACC::tokToStr(currToken.tokenClass));
}

Token Parser::expect(std::vector<TC> expected) {
  for (TC token : expected) {
    if (token == currToken.tokenClass) {
      Token output = currToken;
      nextToken();
      return output;
    }
  }
  throw std::runtime_error("Parsing: Invalid Token at " +
                           currToken.position.toString());
}

Token Parser::lookAhead(int i) {
  assert(i > 0);
  while (tokenBuffer.size() < i) {
    tokenBuffer.push_back(lexer.nextToken());
  }

  return tokenBuffer[i - 1];
}

void Parser::nextToken() {
  // Use Buffer
  if (tokenBuffer.size() != 0) {
    currToken = tokenBuffer.front();
    tokenBuffer.erase(tokenBuffer.begin());
  } else {
    currToken = lexer.nextToken();
  }
}

/* ---- Look Ahead ---- */

bool Parser::acceptDecl() {
  return acceptStructTypeDecl() || acceptVarDecl() || acceptFunDecl();
}

bool Parser::acceptFunDecl() {
  std::pair<bool, int> acceptTypeRes = acceptType();
  bool canAcceptType = acceptTypeRes.first;
  int numTokens = acceptTypeRes.second;
  TC expctIdentLoc = lookAhead(numTokens).tokenClass;
  TC expctLPARLoc = lookAhead(numTokens + 1).tokenClass;
  return (canAcceptType && (expctIdentLoc == TC::IDENTIFIER) &&
          (expctLPARLoc) == TC::LPAR);
}

bool Parser::acceptStructTypeDecl() {
  Token twoAhead = lookAhead(2);
  return acceptStructType() && (twoAhead.tokenClass == TC::LBRA);
}

bool Parser::acceptVarDecl() {
  std::pair<bool, int> acceptTypeRes = acceptType();
  bool canAcceptType = acceptTypeRes.first;
  int numTokens = acceptTypeRes.second;

  TC tokenPastType = lookAhead(numTokens).tokenClass;
  bool followedByIDENT = (tokenPastType == TC::IDENTIFIER);
  TC tokenPastIdent = lookAhead(numTokens + 1).tokenClass;
  bool followedBySC = (tokenPastIdent == TC::SC);
  bool followedByLSBR = (tokenPastIdent == TC::LSBR);
  return (canAcceptType && followedByIDENT && (followedBySC || followedByLSBR));
}

std::pair<bool, int> Parser::acceptType() {
  int numTokens = 1;
  if (accept({TC::INT, TC::CHAR, TC::VOID})) {
    numTokens = 1;
    TC lookAheadToken = lookAhead(numTokens).tokenClass;
    while (lookAheadToken == TC::ASTERIX) {
      numTokens++;
      lookAheadToken = lookAhead(numTokens).tokenClass;
    }
  } else if (acceptStructType()) {
    numTokens = 2;
    TC lookAheadToken = lookAhead(numTokens).tokenClass;
    while (lookAheadToken == TC::ASTERIX) {
      numTokens++;
      lookAheadToken = lookAhead(numTokens).tokenClass;
    }
  } else {
    return std::pair<bool, int>(false, numTokens);
  }
  return std::pair<bool, int>(true, numTokens);
}

bool Parser::acceptStructType() {
  return accept(TC::STRUCT) && (lookAhead(1).tokenClass == TC::IDENTIFIER);
}

bool Parser::acceptParam() {
  std::pair<bool, int> acceptTypeRes = acceptType();
  bool canAcceptType = acceptTypeRes.first;
  int numTokens = acceptTypeRes.second;

  TC tokenPastType = lookAhead(numTokens).tokenClass;
  bool followedByIDENT = (tokenPastType == TC::IDENTIFIER);
  return (canAcceptType && followedByIDENT);
}

bool Parser::acceptStmt() {
  if (acceptVarDecl())
    return true;
  if (acceptBlock())
    return true;
  if (acceptWhile())
    return true;
  if (acceptIf())
    return true;
  if (acceptReturn())
    return true;
  if (acceptAssign())
    return true;
  if (acceptExpr().first)
    return true;

  return false;
}

bool Parser::acceptBlock() { return accept(TC::LBRA); }

bool Parser::acceptWhile() { return accept(TC::WHILE); }

bool Parser::acceptIf() { return accept(TC::IF); }

bool Parser::acceptReturn() { return accept(TC::RETURN); }

bool Parser::acceptAssign() {
  std::pair<bool, int> acceptExprRes = acceptExpr();
  bool canAcceptExpr = acceptExprRes.first;
  int numTokens = acceptExprRes.second;

  TC tokenPastType = lookAhead(numTokens).tokenClass;
  bool followedByASSIGN = (tokenPastType == TC::ASSIGN);
  return (canAcceptExpr && followedByASSIGN);
}

std::pair<bool, int> Parser::acceptExpr() {
  // TODO
  return std::pair<bool, int>(false, 1);
}

/* ---- Parsing ---- */

Program Parser::parseProgram() {
  while (accept(TC::INCLUDE))
    parseInclude();

  std::vector<std::shared_ptr<Decl>> decls;
  while (acceptDecl())
    decls.push_back(parseDecl());

  expect(TC::ENDOFFILE);
  return Program(decls);
}

void Parser::parseInclude() {
  expect(TC::INCLUDE);
  expect(TC::STRING_LITERAL);
}

std::shared_ptr<Decl> Parser::parseDecl() {
  if (acceptStructTypeDecl())
    return parseStructTypeDecl();

  if (acceptVarDecl())
    return parseVarDecl();

  if (acceptFunDecl())
    return parseFunDecl();

  throw std::runtime_error("Parser: Expected a Struct/Variable/Function "
                           "Declaration but none was found.");
}

std::shared_ptr<StructTypeDecl> Parser::parseStructTypeDecl() {
  std::shared_ptr<StructType> structType = parseStructType();
  expect(TC::LBRA);
  std::vector<std::shared_ptr<VarDecl>> fields;
  do {
    fields.push_back(parseVarDecl());
  } while (acceptVarDecl());

  expect(TC::RBRA);
  expect(TC::SC);
  return std::shared_ptr<StructTypeDecl>(
      new StructTypeDecl(structType, fields));
}

std::shared_ptr<VarDecl> Parser::parseVarDecl() {
  std::shared_ptr<Type> varType = parseType();
  std::string varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::string arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  expect(TC::SC);
  return std::shared_ptr<VarDecl>(new VarDecl(varType, varIdentifier));
}

std::shared_ptr<FunDecl> Parser::parseFunDecl() {
  std::shared_ptr<Type> funType = parseType();
  std::string funIdent = expect(TC::IDENTIFIER).data;
  expect(TC::LPAR);
  std::vector<std::shared_ptr<VarDecl>> funParams;

  if (acceptParam())
    funParams.push_back(parseParam());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    funParams.push_back(parseParam());
  }

  expect(TC::RPAR);

  std::shared_ptr<Block> funBlock = parseBlock();

  return std::shared_ptr<FunDecl>(
      new FunDecl(funBlock, funIdent, funParams, funType));
}

std::shared_ptr<VarDecl> Parser::parseParam() {
  std::shared_ptr<Type> varType = parseType();
  std::string varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::string arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return std::shared_ptr<VarDecl>(new VarDecl(varType, varIdentifier));
}

std::shared_ptr<Type> Parser::parseType() {
  std::shared_ptr<Type> type;
  if (acceptStructType()) {
    type = parseStructType();
    while (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      type = std::shared_ptr<PointerType>(new PointerType(type));
    }
  } else {
    Token baseType = expect({TC::INT, TC::CHAR, TC::VOID});
    PrimitiveType pType;
    switch (baseType.tokenClass) {
    case TC::INT:
      pType = PrimitiveType::INT;
      break;
    case TC::CHAR:
      pType = PrimitiveType::CHAR;
      break;
    case TC::VOID:
      pType = PrimitiveType::VOID;
      break;
    default:
      pType = PrimitiveType::VOID;
      break;
    }
    type = std::shared_ptr<BaseType>(new BaseType(pType));
    while (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      type = std::shared_ptr<PointerType>(new PointerType(type));
    }
  }
  return type;
}

std::shared_ptr<StructType> Parser::parseStructType() {
  expect(TC::STRUCT);
  std::string structIdentifier = expect(TC::IDENTIFIER).data;
  return std::shared_ptr<StructType>(new StructType(structIdentifier));
}

std::shared_ptr<Block> Parser::parseBlock() {
  expect(TC::LBRA);

  std::vector<std::shared_ptr<Stmt>> blockStmts;

  while (acceptStmt()) {
    blockStmts.push_back(parseStmt());
  }

  expect(TC::RBRA);
  return std::shared_ptr<Block>(new Block(blockStmts));
}

std::shared_ptr<Stmt> Parser::parseStmt() {
  return std::shared_ptr<Stmt>(new Stmt());
}

/* Helpers */

std::shared_ptr<BaseType> Parser::tokenToType(const TC &tc) {
  switch (tc) {
  case TC::INT:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  case TC::CHAR:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
  case TC::VOID:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  default:
    throw std::runtime_error("Parsing: Cannot resolve Token " + tokToStr(tc) +
                             "  to a type.");
  }
}