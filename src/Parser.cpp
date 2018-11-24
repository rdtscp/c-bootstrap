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
  for (const TC token : expected) {
    if (accept(token))
      return true;
  }
  return false;
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

  Token tokenPastType = lookAhead(numTokens);
  bool followedByIDENT = (tokenPastType.tokenClass == TC::IDENTIFIER);
  Token tokenPastIdent = lookAhead(numTokens + 1);
  bool followedBySC = (tokenPastIdent.tokenClass == TC::SC);
  bool followedByLSBR = (tokenPastIdent.tokenClass == TC::LSBR);
  return (canAcceptType && followedByIDENT && (followedBySC || followedByLSBR));
}

std::pair<bool, int> Parser::acceptType(int startAhead) {
  int numTokens = startAhead + 1;
  if (accept({TC::INT, TC::CHAR, TC::VOID})) {
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
  if (acceptExpr())
    return true;

  return false;
}

bool Parser::acceptBlock() { return accept(TC::LBRA); }

bool Parser::acceptWhile() { return accept(TC::WHILE); }

bool Parser::acceptIf() { return accept(TC::IF); }

bool Parser::acceptReturn() { return accept(TC::RETURN); }

bool Parser::acceptAssign() { return acceptExpr(); }

bool Parser::acceptExpr() {
  return accept({TC::LPAR, TC::SIZEOF, TC::ASTERIX, TC::MINUS, TC::IDENTIFIER,
                 TC::INT_LITERAL, TC::CHAR_LITERAL, TC::STRING_LITERAL});
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
  if (acceptVarDecl())
    return parseVarDecl();

  if (acceptBlock())
    return parseBlock();

  if (acceptWhile())
    return parseWhile();

  if (acceptIf())
    return parseIf();

  if (acceptReturn())
    return parseReturn();

  std::shared_ptr<Expr> expr = parseExpr();

  if (accept(TC::ASSIGN)) {
    expect(TC::ASSIGN);
    std::shared_ptr<Expr> rhs = parseExpr();
    expect(TC::SC);
    return std::shared_ptr<Assign>(new Assign(expr, rhs));
  }

  expect(TC::SC);
  return expr;
}

std::shared_ptr<While> Parser::parseWhile() {
  expect(TC::WHILE);
  expect(TC::LPAR);
  std::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  std::shared_ptr<Stmt> whileBody = parseStmt();

  return std::shared_ptr<While>(new While(whileBody, whileCondition));
}

std::shared_ptr<If> Parser::parseIf() {
  expect(TC::IF);
  expect(TC::LPAR);
  std::shared_ptr<Expr> ifCondition = parseExpr();
  expect(TC::RPAR);
  std::shared_ptr<Stmt> ifBody = parseStmt();
  if (accept(TC::ELSE)) {
    expect(TC::ELSE);
    std::shared_ptr<Stmt> elseBody = parseStmt();
    return std::shared_ptr<If>(new If(ifCondition, ifBody, elseBody));
  } else {
    return std::shared_ptr<If>(new If(ifCondition, ifBody));
  }
}

std::shared_ptr<Return> Parser::parseReturn() {
  expect(TC::RETURN);
  if (acceptExpr()) {
    std::shared_ptr<Expr> returnExpr = parseExpr();
    expect(TC::SC);
    return std::shared_ptr<Return>(new Return(returnExpr));
  } else {
    expect(TC::SC);
    return std::shared_ptr<Return>(new Return());
  }
}

std::shared_ptr<Assign> Parser::parseAssign() {
  std::shared_ptr<Expr> lhs = parseExpr();
  expect(TC::ASSIGN);
  std::shared_ptr<Expr> rhs = parseExpr();
  return std::shared_ptr<Assign>(new Assign(lhs, rhs));
}

std::shared_ptr<Expr> Parser::parseExpr() {
  if (accept(TC::LPAR) && (acceptType(1).first == false)) {
    expect(TC::LPAR);
    std::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return std::shared_ptr<ParenthExpr>(new ParenthExpr(innerExpr));
  }

  return parseBoolExpr();
}

std::shared_ptr<Expr> Parser::parseBoolExpr() {
  std::shared_ptr<Expr> lhs = parseEqualExpr();
  if (accept(TC::OR)) {
    expect(TC::OR);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::OR, rhs));
  }
  if (accept(TC::AND)) {
    expect(TC::AND);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::AND, rhs));
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseEqualExpr() {
  std::shared_ptr<Expr> lhs = parseCompExpr();
  if (accept(TC::NE)) {
    expect(TC::NE);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::NE, rhs));
  }
  if (accept(TC::EQ)) {
    expect(TC::EQ);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::EQ, rhs));
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseCompExpr() {
  std::shared_ptr<Expr> lhs = parseAddExpr();
  if (accept(TC::LT)) {
    expect(TC::LT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::LT, rhs));
  }
  if (accept(TC::GT)) {
    expect(TC::GT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::GT, rhs));
  }
  if (accept(TC::LE)) {
    expect(TC::LE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::LE, rhs));
  }
  if (accept(TC::GE)) {
    expect(TC::GE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::GE, rhs));
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseAddExpr() {
  std::shared_ptr<Expr> lhs = parseMulExpr();
  if (accept(TC::PLUS)) {
    expect(TC::PLUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::ADD, rhs));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::SUB, rhs));
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseMulExpr() {
  std::shared_ptr<Expr> lhs = parseUnaryExpr();
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::MUL, rhs));
  }
  if (accept(TC::DIV)) {
    expect(TC::DIV);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::DIV, rhs));
  }
  if (accept(TC::REM)) {
    expect(TC::REM);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::MOD, rhs));
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseUnaryExpr() {
  if (accept(TC::SIZEOF)) {
    expect(TC::SIZEOF);
    expect(TC::LPAR);
    std::shared_ptr<Type> type = parseType();
    expect(TC::RPAR);
    return std::shared_ptr<SizeOf>(new SizeOf(type));
  }
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseObjExpr();
    return std::shared_ptr<ValueAt>(new ValueAt(rhs));
  }
  if (accept(TC::LPAR) && (acceptType(1).first == true)) {
    expect(TC::LPAR);
    std::shared_ptr<Type> castType = parseType();
    expect(TC::RPAR);
    std::shared_ptr<Expr> expToCast = parseObjExpr();
    return std::shared_ptr<TypeCast>(new TypeCast(castType, expToCast));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<IntLiteral> lhs(new IntLiteral("0"));
    std::shared_ptr<Expr> rhs = parseObjExpr();
    return std::shared_ptr<BinOp>(new BinOp(lhs, Op::SUB, rhs));
  }

  return parseObjExpr();
}

std::shared_ptr<Expr> Parser::parseObjExpr() {
  if (accept(TC::IDENTIFIER)) {
    std::string ident = expect(TC::IDENTIFIER).data;
    if (accept(TC::LPAR)) {
      expect(TC::LPAR);

      std::vector<std::shared_ptr<Expr>> params;
      if (acceptExpr())
        params.push_back(parseLitExpr());
      while (accept(TC::COMMA)) {
        expect(TC::COMMA);
        params.push_back(parseLitExpr());
      }

      expect(TC::RPAR);
      return std::shared_ptr<FunCall>(new FunCall(ident, params));
    }
    return std::shared_ptr<VarExpr>(new VarExpr(ident));
  }

  std::shared_ptr<Expr> lhs = parseLitExpr();
  if (accept(TC::DOT)) {
    expect(TC::DOT);
    std::string fieldIdent = expect(TC::IDENTIFIER).data;
    return std::shared_ptr<FieldAccess>(new FieldAccess(lhs, fieldIdent));
  }
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::shared_ptr<Expr> index = parseLitExpr();
    expect(TC::RSBR);
    return std::shared_ptr<Expr>(new ArrayAccess(lhs, index));
  }

  return lhs;
}

std::shared_ptr<Expr> Parser::parseLitExpr() {
  if (accept(TC::INT_LITERAL))
    return std::shared_ptr<IntLiteral>(
        new IntLiteral(expect(TC::INT_LITERAL).data));
  if (accept(TC::CHAR_LITERAL))
    return std::shared_ptr<CharLiteral>(
        new CharLiteral(expect(TC::CHAR_LITERAL).data));
  if (accept(TC::STRING_LITERAL))
    return std::shared_ptr<StringLiteral>(
        new StringLiteral(expect(TC::STRING_LITERAL).data));

  if (acceptExpr())
    return parseExpr();

  throw std::runtime_error("Parsing: Expected an Expression at " +
                           currToken.position.toString());
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