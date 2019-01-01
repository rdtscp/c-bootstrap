#include <cassert>
#include <stdexcept>

#include "../include/Parser.h"

/* Public */

using namespace ACC;
using TC = Token::TokenClass;

Parser::Parser(Lexer &lexer)
    : currToken(TC::INVALID, Position(-1, -1, "")), lexer(lexer) {}

std::shared_ptr<Program> Parser::parse() {
  nextToken();
  return parseProgram();
}

/* ---- Token Operations ---- */

bool Parser::accept(TC expected, int offset) {
  if (offset == 0)
    return expected == currToken.tokenClass;
  TC actual = lookAhead(offset).tokenClass;
  return expected == actual;
}

bool Parser::accept(std::vector<TC> expected, int offset) {
  for (const TC token : expected) {
    if (accept(token, offset))
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
  assert(i >= 0);
  if (i == 0)
    return currToken;
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

bool Parser::acceptDecl(int offset) {
  if (acceptStructTypeDecl(offset))
    return true;

  if (acceptVarDecl(offset))
    return true;

  if (acceptFunDecl(offset))
    return true;

  return false;
}

bool Parser::acceptFunDecl(int offset) {
  if (acceptStructType(offset)) {
    offset += 2;
    while (accept(TC::ASTERIX, offset))
      offset++;
    return accept(TC::IDENTIFIER, offset) && accept(TC::LPAR, offset + 1);
  }

  if (acceptType(offset)) {
    offset += 1;
    while (accept(TC::ASTERIX, offset))
      offset++;

    return accept(TC::IDENTIFIER, offset) && accept(TC::LPAR, offset + 1);
  }

  return false;
}

bool Parser::acceptStructTypeDecl(int offset) {
  if (!acceptStructType(offset))
    return false;

  if (!accept(TC::LBRA, offset + 2))
    return false;

  return true;
}

bool Parser::acceptVarDecl(int offset) {
  if (!accept({TC::INT, TC::CHAR, TC::VOID, TC::STRUCT, TC::EXTERN}, offset))
    return false;

  if (accept(TC::EXTERN, offset))
    offset++;

  if (accept(TC::STRUCT, offset)) {
    offset++;
    if (!accept(TC::IDENTIFIER, offset))
      return false;

    offset++;
    while (accept(TC::ASTERIX, offset))
      offset++;

    if (!accept(TC::IDENTIFIER, offset))
      return false;

    offset++;
    if (!accept({TC::SC, TC::LSBR}, offset))
      return false;

  } else if (accept({TC::INT, TC::CHAR, TC::VOID}, offset)) {
    offset++;
    while (accept(TC::ASTERIX, offset))
      offset++;

    if (!accept(TC::IDENTIFIER, offset))
      return false;

    offset++;
    if (!accept({TC::SC, TC::LSBR}, offset))
      return false;
  } else
    return false;

  return true;
}

bool Parser::acceptType(int offset) {
  return accept({TC::INT, TC::CHAR, TC::VOID, TC::STRUCT}, offset);
}

bool Parser::acceptStructType(int offset) {
  if (!accept(TC::STRUCT, offset))
    return false;

  if (!accept(TC::IDENTIFIER, offset + 1))
    return false;

  return true;
}

bool Parser::acceptParam(int offset) { return acceptType(offset); }

bool Parser::acceptStmt(int offset) {
  if (acceptVarDecl(offset))
    return true;
  if (acceptBlock(offset))
    return true;
  if (acceptWhile(offset))
    return true;
  if (acceptDoWhile(offset))
    return true;
  if (acceptIf(offset))
    return true;
  if (acceptReturn(offset))
    return true;
  if (acceptAssign(offset))
    return true;
  if (acceptExpr(offset))
    return true;

  return false;
}

bool Parser::acceptBlock(int offset) { return accept(TC::LBRA, offset); }

bool Parser::acceptWhile(int offset) { return accept(TC::WHILE, offset); }

bool Parser::acceptDoWhile(int offset) { return accept(TC::DO, offset); }

bool Parser::acceptIf(int offset) { return accept(TC::IF, offset); }

bool Parser::acceptReturn(int offset) { return accept(TC::RETURN, offset); }

bool Parser::acceptAssign(int offset) { return acceptExpr(offset); }

bool Parser::acceptExpr(int offset) {
  return accept({TC::LPAR, TC::SIZEOF, TC::ASTERIX, TC::MINUS, TC::IDENTIFIER,
                 TC::INT_LITERAL, TC::CHAR_LITERAL, TC::STRING_LITERAL},
                offset);
}

/* ---- Parsing ---- */

std::shared_ptr<Program> Parser::parseProgram() {
  std::vector<std::shared_ptr<Decl>> decls;
  while (acceptDecl())
    decls.push_back(parseDecl());

  expect(TC::ENDOFFILE);
  std::shared_ptr<Program> p = std::make_shared<Program>(Program(decls));
  p->position = currToken.position;
  return p;
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

  std::shared_ptr<StructTypeDecl> newNode =
      std::make_shared<StructTypeDecl>(StructTypeDecl(structType, fields));
  newNode->Decl::position = currToken.position;
  return newNode;
}

std::shared_ptr<VarDecl> Parser::parseVarDecl() {
  bool isExtern = false;
  if (accept(TC::EXTERN)) {
    expect(TC::EXTERN);
    isExtern = true;
  }
  std::shared_ptr<Type> varType = parseType();
  std::string varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::string arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  expect(TC::SC);
  std::shared_ptr<VarDecl> newNode =
      std::make_shared<VarDecl>(VarDecl(varType, varIdentifier, isExtern));
  newNode->Decl::position = currToken.position;
  return newNode;
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

  std::shared_ptr<FunDecl> newNode = std::make_shared<FunDecl>(
      FunDecl(funBlock, funIdent, funParams, funType));
  newNode->Decl::position = currToken.position;
  return newNode;
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
  std::shared_ptr<VarDecl> newNode =
      std::make_shared<VarDecl>(VarDecl(varType, varIdentifier));
  newNode->Decl::position = currToken.position;
  return newNode;
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
  type->position = currToken.position;
  return type;
}

std::shared_ptr<StructType> Parser::parseStructType() {
  expect(TC::STRUCT);
  std::string structIdentifier = expect(TC::IDENTIFIER).data;
  std::shared_ptr<StructType> newNode =
      std::make_shared<StructType>(StructType(structIdentifier));
  newNode->position = currToken.position;
  return newNode;
}

std::shared_ptr<Block> Parser::parseBlock() {
  expect(TC::LBRA);

  std::vector<std::shared_ptr<Stmt>> blockStmts;

  while (acceptStmt()) {
    blockStmts.push_back(parseStmt());
  }

  expect(TC::RBRA);
  std::shared_ptr<Block> newNode = std::make_shared<Block>(Block(blockStmts));
  newNode->position = currToken.position;
  return newNode;
}

std::shared_ptr<Stmt> Parser::parseStmt() {
  if (acceptVarDecl())
    return parseVarDecl();

  if (acceptBlock())
    return parseBlock();

  if (acceptWhile())
    return parseWhile();

  if (acceptDoWhile())
    return parseDoWhile();

  if (acceptIf())
    return parseIf();

  if (acceptReturn())
    return parseReturn();

  std::shared_ptr<Expr> expr = parseExpr();

  if (accept(TC::ASSIGN)) {
    expect(TC::ASSIGN);
    std::shared_ptr<Expr> rhs = parseExpr();
    expect(TC::SC);
    std::shared_ptr<Assign> newNode =
        std::make_shared<Assign>(Assign(expr, rhs));
    newNode->position = currToken.position;
    return newNode;
  }

  expect(TC::SC);
  expr->position = currToken.position;
  return expr;
}

std::shared_ptr<While> Parser::parseWhile() {
  expect(TC::WHILE);
  expect(TC::LPAR);
  std::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  std::shared_ptr<Stmt> whileBody = parseStmt();

  std::shared_ptr<While> newNode =
      std::make_shared<While>(While(whileBody, whileCondition));
  newNode->position = currToken.position;
  return newNode;
}

std::shared_ptr<DoWhile> Parser::parseDoWhile() {
  expect(TC::DO);
  std::shared_ptr<Stmt> whileBody = parseStmt();
  expect(TC::WHILE);
  expect(TC::LPAR);
  std::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  expect(TC::SC);

  std::shared_ptr<DoWhile> newNode =
      std::make_shared<DoWhile>(DoWhile(whileBody, whileCondition));
  newNode->position = currToken.position;
  return newNode;
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
    std::shared_ptr<If> newNode =
        std::make_shared<If>(If(ifCondition, ifBody, elseBody));
    newNode->position = currToken.position;
    return newNode;
  } else {
    std::shared_ptr<If> newNode = std::make_shared<If>(If(ifCondition, ifBody));
    newNode->position = currToken.position;
    return newNode;
  }
}

std::shared_ptr<Return> Parser::parseReturn() {
  expect(TC::RETURN);
  if (acceptExpr()) {
    std::shared_ptr<Expr> returnExpr = parseExpr();
    expect(TC::SC);
    std::shared_ptr<Return> newNode =
        std::make_shared<Return>(Return(returnExpr));
    newNode->position = currToken.position;
    return newNode;
  } else {
    expect(TC::SC);
    std::shared_ptr<Return> newNode = std::make_shared<Return>(Return());
    newNode->position = currToken.position;
    return newNode;
  }
}

std::shared_ptr<Assign> Parser::parseAssign() {
  std::shared_ptr<Expr> lhs = parseExpr();
  expect(TC::ASSIGN);
  std::shared_ptr<Expr> rhs = parseExpr();
  std::shared_ptr<Assign> newNode = std::make_shared<Assign>(Assign(lhs, rhs));
  newNode->position = currToken.position;
  return newNode;
}

std::shared_ptr<Expr> Parser::parseExpr() {
  if (accept(TC::LPAR) && !acceptType(1)) {
    expect(TC::LPAR);
    std::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    std::shared_ptr<ParenthExpr> newNode =
        std::make_shared<ParenthExpr>(ParenthExpr(innerExpr));
    newNode->position = currToken.position;
    return newNode;
  }

  return parseBoolExpr();
}

std::shared_ptr<Expr> Parser::parseBoolExpr() {
  std::shared_ptr<Expr> lhs = parseEqualExpr();
  if (accept(TC::OR)) {
    expect(TC::OR);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::OR, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::AND)) {
    expect(TC::AND);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::AND, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseEqualExpr() {
  std::shared_ptr<Expr> lhs = parseCompExpr();
  if (accept(TC::NE)) {
    expect(TC::NE);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::NE, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::EQ)) {
    expect(TC::EQ);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::EQ, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseCompExpr() {
  std::shared_ptr<Expr> lhs = parseAddExpr();
  if (accept(TC::LT)) {
    expect(TC::LT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::LT, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::GT)) {
    expect(TC::GT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::GT, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::LE)) {
    expect(TC::LE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::LE, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::GE)) {
    expect(TC::GE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::GE, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseAddExpr() {
  std::shared_ptr<Expr> lhs = parseMulExpr();
  if (accept(TC::PLUS)) {
    expect(TC::PLUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::ADD, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseMulExpr() {
  std::shared_ptr<Expr> lhs = parseUnaryExpr();
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::MUL, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::DIV)) {
    expect(TC::DIV);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::DIV, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::REM)) {
    expect(TC::REM);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::MOD, rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  return lhs;
}

std::shared_ptr<Expr> Parser::parseUnaryExpr() {
  if (accept(TC::SIZEOF)) {
    expect(TC::SIZEOF);
    expect(TC::LPAR);
    std::shared_ptr<Type> type = parseType();
    expect(TC::RPAR);
    std::shared_ptr<SizeOf> newNode = std::make_shared<SizeOf>(SizeOf(type));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseObjExpr();
    std::shared_ptr<ValueAt> newNode = std::make_shared<ValueAt>(ValueAt(rhs));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::LPAR) && acceptType(1)) {
    expect(TC::LPAR);
    std::shared_ptr<Type> castType = parseType();
    expect(TC::RPAR);
    std::shared_ptr<Expr> expToCast = parseObjExpr();
    std::shared_ptr<TypeCast> newNode =
        std::make_shared<TypeCast>(TypeCast(castType, expToCast));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<IntLiteral> lhs(new IntLiteral("0"));
    std::shared_ptr<Expr> rhs = parseObjExpr();
    std::shared_ptr<BinOp> newNode =
        std::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
    newNode->position = currToken.position;
    return newNode;
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
      std::shared_ptr<FunCall> newNode =
          std::make_shared<FunCall>(FunCall(ident, params));
      newNode->position = currToken.position;
      return newNode;
    }
    std::shared_ptr<VarExpr> newNode =
        std::make_shared<VarExpr>(VarExpr(ident));
    newNode->position = currToken.position;
    return newNode;
  }

  std::shared_ptr<Expr> lhs = parseLitExpr();
  if (accept(TC::DOT)) {
    expect(TC::DOT);
    std::string fieldIdent = expect(TC::IDENTIFIER).data;
    std::shared_ptr<FieldAccess> newNode =
        std::make_shared<FieldAccess>(FieldAccess(lhs, fieldIdent));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::shared_ptr<Expr> index = parseLitExpr();
    expect(TC::RSBR);
    std::shared_ptr<ArrayAccess> newNode =
        std::make_shared<ArrayAccess>(ArrayAccess(lhs, index));
    newNode->position = currToken.position;
    return newNode;
  }

  return lhs;
}

std::shared_ptr<Expr> Parser::parseLitExpr() {
  if (accept(TC::INT_LITERAL)) {
    std::shared_ptr<IntLiteral> newNode =
        std::make_shared<IntLiteral>(IntLiteral(expect(TC::INT_LITERAL).data));
    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::CHAR_LITERAL)) {
    std::shared_ptr<CharLiteral> newNode = std::make_shared<CharLiteral>(
        CharLiteral(expect(TC::CHAR_LITERAL).data));

    newNode->position = currToken.position;
    return newNode;
  }
  if (accept(TC::STRING_LITERAL)) {
    std::shared_ptr<StringLiteral> newNode = std::make_shared<StringLiteral>(
        StringLiteral(expect(TC::STRING_LITERAL).data));
    newNode->position = currToken.position;
    return newNode;
  }
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