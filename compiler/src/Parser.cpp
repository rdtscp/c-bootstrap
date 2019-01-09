#include <cassert>
#include <stdexcept>

#include "../include/Parser.h"

using namespace ACC;
using TC = SourceToken::Class;

Parser::Parser(Lexer &lexer)
    : currToken(TC::INVALID, Position(-1, -1, "")), lexer(lexer) {}

std::shared_ptr<Program> Parser::getAST() {
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

SourceToken Parser::expect(TC expected) {
  if (expected == currToken.tokenClass) {
    SourceToken output = currToken;
    nextToken();
    return output;
  }
  throw std::runtime_error(
      "Parsing: Expected Token " + ACC::tokToStr(expected) + " at " +
      currToken.position.toString() +
      " but found: " + ACC::tokToStr(currToken.tokenClass));
}

SourceToken Parser::expect(std::vector<TC> expected) {
  for (TC token : expected) {
    if (token == currToken.tokenClass) {
      SourceToken output = currToken;
      nextToken();
      return output;
    }
  }
  throw std::runtime_error("Parsing: Invalid Token at " +
                           currToken.position.toString());
}

SourceToken Parser::lookAhead(int i) {
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

/* -- Decls -- */
bool Parser::acceptDecl(int offset) {
  if (acceptStructTypeDecl(offset))
    return true;

  if (acceptVarDecl(offset))
    return true;

  if (acceptFunDecl(offset))
    return true;

  if (acceptTypeDefDecl())
    return true;

  if (acceptEnumTypeDecl())
    return true;

  return false;
}
bool Parser::acceptEnumTypeDecl(int offset) { return accept(TC::ENUM); }
bool Parser::acceptFunDecl(int offset) {
  if (accept(TC::EXTERN, offset))
    offset++;
  if (acceptStructType(offset)) {
    offset += 2;
    while (accept(TC::ASTERIX, offset))
      offset++;
    return accept(TC::IDENTIFIER, offset) && accept(TC::LPAR, offset + 1);
  }

  if (accept(TC::UNSIGNED, offset))
    offset++;
  if (acceptType(offset)) {
    offset++;
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
bool Parser::acceptTypeDefDecl(int offset) { return accept(TC::TYPEDEF); }
bool Parser::acceptVarDecl(int offset) {
  if (!acceptType(offset) && !accept(TC::EXTERN, offset))
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

  } else if (accept({TC::INT, TC::CHAR, TC::SHORT, TC::VOID, TC::UNSIGNED},
                    offset)) {
    if (accept(TC::UNSIGNED, offset))
      offset++;

    offset++;
    while (accept(TC::ASTERIX, offset))
      offset++;

    if (!accept(TC::IDENTIFIER, offset))
      return false;
  } else
    return false;

  return true;
}

/* -- Types -- */
bool Parser::acceptStructType(int offset) {
  if (!accept(TC::STRUCT, offset))
    return false;

  if (!accept(TC::IDENTIFIER, offset + 1))
    return false;

  return true;
}
bool Parser::acceptType(int offset) {
  return accept(
      {TC::INT, TC::CHAR, TC::VOID, TC::STRUCT, TC::SHORT, TC::UNSIGNED},
      offset);
}

/* -- Stmts -- */
bool Parser::acceptAssign(int offset) { return acceptExpr(offset); }
bool Parser::acceptBlock(int offset) { return accept(TC::LBRA, offset); }
bool Parser::acceptDoWhile(int offset) { return accept(TC::DO, offset); }
bool Parser::acceptIf(int offset) { return accept(TC::IF, offset); }
bool Parser::acceptReturn(int offset) { return accept(TC::RETURN, offset); }
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
bool Parser::acceptWhile(int offset) { return accept(TC::WHILE, offset); }

bool Parser::acceptParam(int offset) { return acceptType(offset); }

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
  return std::make_shared<Program>(Program(decls));
}

/* -- Decls -- */
std::shared_ptr<Decl> Parser::parseDecl() {
  if (acceptStructTypeDecl()) {
    std::shared_ptr<StructTypeDecl> std = parseStructTypeDecl();
    expect(TC::SC);
    return std;
  }

  if (acceptFunDecl()) {
    std::shared_ptr<FunDecl> fd = parseFunDecl();
    return fd;
  }

  if (acceptVarDecl()) {
    std::shared_ptr<VarDecl> vd = parseVarDecl();
    expect(TC::SC);
    return vd;
  }

  if (acceptTypeDefDecl()) {
    std::shared_ptr<TypeDefDecl> tdd = parseTypeDefDecl();
    expect(TC::SC);
    return tdd;
  }

  if (acceptEnumTypeDecl()) {
    std::shared_ptr<EnumTypeDecl> etd = parseEnumTypeDecl();
    expect(TC::SC);
    return etd;
  }

  throw std::runtime_error("Parser: Expected a Struct/Variable/Function "
                           "Declaration but none was found.");
}
std::shared_ptr<EnumTypeDecl> Parser::parseEnumTypeDecl() {
  expect(TC::ENUM);
  std::string ident = "";
  if (accept(TC::IDENTIFIER))
    ident = expect(TC::IDENTIFIER).data;
  expect(TC::LBRA);

  bool moreStates = false;
  std::map<std::string, std::string> states;
  do {
    std::string ident = expect(TC::IDENTIFIER).data;
    std::string value = "";
    if (accept(TC::ASSIGN)) {
      expect(TC::ASSIGN);
      value = expect(TC::INT_LITERAL).data;
    }
    states[ident] = value;
    if (accept(TC::COMMA)) {
      expect(TC::COMMA);
      moreStates = true;
    } else
      moreStates = false;
  } while (moreStates);
  expect(TC::RBRA);
  return std::make_shared<EnumTypeDecl>(EnumTypeDecl(ident, states));
}
std::shared_ptr<FunDecl> Parser::parseFunDecl() {
  if (accept(TC::EXTERN)) {
    expect(TC::EXTERN);
  }
  std::shared_ptr<Type> funType = parseType();
  std::string funIdent = expect(TC::IDENTIFIER).data;
  expect(TC::LPAR);
  std::vector<std::shared_ptr<VarDecl>> funParams;

  bool isDef = true;
  if (acceptParam())
    funParams.push_back(parseParam());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    std::shared_ptr<VarDecl> currParam = parseParam();
    if (currParam->identifer == "")
      isDef = false;

    funParams.push_back(currParam);
  }

  expect(TC::RPAR);

  if (acceptBlock() && !isDef) {
    throw std::runtime_error(
        "Parser: FunDef signature did not provide names for parameters.");
  } else if (acceptBlock()) {
    std::shared_ptr<Block> funBlock = parseBlock();
    std::shared_ptr<FunDef> fd(
        new FunDef(funBlock, funIdent, funParams, funType));
    return fd;
    // return std::make_shared<FunDef>(
    //     FunDef(funBlock, funIdent, funParams, funType));
  } else {
    expect(TC::SC);
    std::shared_ptr<FunDecl> fd(new FunDecl(funIdent, funParams, funType));
    return fd;
    // return std::make_shared<FunDecl>(FunDecl(funIdent, funParams, funType));
  }
}
std::shared_ptr<StructTypeDecl> Parser::parseStructTypeDecl() {
  std::shared_ptr<StructType> structType = parseStructType();
  expect(TC::LBRA);
  std::vector<std::shared_ptr<VarDecl>> fields;
  do {
    std::shared_ptr<VarDecl> structField = parseVarDecl();
    expect(TC::SC);
    fields.push_back(structField);
  } while (acceptVarDecl());

  expect(TC::RBRA);

  return std::make_shared<StructTypeDecl>(StructTypeDecl(structType, fields));
}
std::shared_ptr<TypeDefDecl> Parser::parseTypeDefDecl() {
  expect(TC::TYPEDEF);
  std::shared_ptr<Type> aliasedType;
  if (acceptStructTypeDecl()) {
    std::shared_ptr<StructTypeDecl> std = parseStructTypeDecl();
    aliasedType = std->structType;
  } else if (acceptEnumTypeDecl()) {
    std::shared_ptr<EnumTypeDecl> etd = parseEnumTypeDecl();
    aliasedType = etd;
  } else {
    aliasedType = parseType();
  }
  std::string typeAlias = expect(TC::IDENTIFIER).data;
  return std::make_shared<TypeDefDecl>(TypeDefDecl(aliasedType, typeAlias));
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
    std::string arraySize;
    if (!isExtern || accept(TC::INT_LITERAL))
      arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return std::make_shared<VarDecl>(VarDecl(varType, varIdentifier, isExtern));
}

/* -- Types -- */
std::shared_ptr<StructType> Parser::parseStructType() {
  expect(TC::STRUCT);
  std::string structIdentifier = expect(TC::IDENTIFIER).data;
  return std::make_shared<StructType>(StructType(structIdentifier));
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
    std::vector<TC> modifiers;
    while (accept(TC::UNSIGNED))
      modifiers.push_back(expect(TC::UNSIGNED).tokenClass);

    SourceToken baseType = expect({TC::INT, TC::CHAR, TC::VOID, TC::SHORT});
    PrimitiveType pType;
    switch (baseType.tokenClass) {
    case TC::INT:
      pType = PrimitiveType::INT;
      break;
    case TC::CHAR:
      pType = PrimitiveType::CHAR;
      break;
    case TC::SHORT:
      pType = PrimitiveType::SHORT;
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

/* -- Stmts -- */
std::shared_ptr<Assign> Parser::parseAssign() {
  std::shared_ptr<Expr> lhs = parseExpr();
  expect(TC::ASSIGN);
  std::shared_ptr<Expr> rhs = parseExpr();
  return std::make_shared<Assign>(Assign(lhs, rhs));
}
std::shared_ptr<Block> Parser::parseBlock() {
  expect(TC::LBRA);

  std::vector<std::shared_ptr<Stmt>> blockStmts;

  while (acceptStmt()) {
    blockStmts.push_back(parseStmt());
  }

  expect(TC::RBRA);
  return std::make_shared<Block>(Block(blockStmts));
}
std::shared_ptr<DoWhile> Parser::parseDoWhile() {
  expect(TC::DO);
  std::shared_ptr<Stmt> whileBody = parseStmt();
  expect(TC::WHILE);
  expect(TC::LPAR);
  std::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  expect(TC::SC);

  return std::make_shared<DoWhile>(DoWhile(whileBody, whileCondition));
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
    return std::make_shared<If>(If(ifCondition, ifBody, elseBody));
  } else {
    return std::make_shared<If>(If(ifCondition, ifBody));
  }
}
std::shared_ptr<Return> Parser::parseReturn() {
  expect(TC::RETURN);
  if (acceptExpr()) {
    std::shared_ptr<Expr> returnExpr = parseExpr();
    expect(TC::SC);
    return std::make_shared<Return>(Return(returnExpr));
  } else {
    expect(TC::SC);
    return std::make_shared<Return>(Return());
  }
}
std::shared_ptr<Stmt> Parser::parseStmt() {
  if (acceptVarDecl()) {
    std::shared_ptr<VarDecl> vd = parseVarDecl();
    expect(TC::SC);
    return vd;
  }

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
    return std::make_shared<Assign>(Assign(expr, rhs));
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

  return std::make_shared<While>(While(whileBody, whileCondition));
}

std::shared_ptr<VarDecl> Parser::parseParam() {
  std::shared_ptr<Type> varType = parseType();
  std::string varIdentifier = "";
  if (accept(TC::IDENTIFIER))
    varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::string arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return std::make_shared<VarDecl>(VarDecl(varType, varIdentifier));
}

/* -- Exprs -- */
std::shared_ptr<Expr> Parser::parseExpr() {
  if (accept(TC::LPAR) && !acceptType(1)) {
    expect(TC::LPAR);
    std::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return std::make_shared<ParenthExpr>(ParenthExpr(innerExpr));
  }

  return parseBoolExpr();
}
std::shared_ptr<Expr> Parser::parseBoolExpr() {
  std::shared_ptr<Expr> lhs = parseEqualExpr();
  if (accept(TC::OR)) {
    expect(TC::OR);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::OR, rhs));
  }
  if (accept(TC::AND)) {
    expect(TC::AND);
    std::shared_ptr<Expr> rhs = parseEqualExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::AND, rhs));
  }
  return lhs;
}
std::shared_ptr<Expr> Parser::parseEqualExpr() {
  std::shared_ptr<Expr> lhs = parseCompExpr();
  if (accept(TC::NE)) {
    expect(TC::NE);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::NE, rhs));
  }
  if (accept(TC::EQ)) {
    expect(TC::EQ);
    std::shared_ptr<Expr> rhs = parseCompExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::EQ, rhs));
  }
  return lhs;
}
std::shared_ptr<Expr> Parser::parseCompExpr() {
  std::shared_ptr<Expr> lhs = parseAddExpr();
  if (accept(TC::LT)) {
    expect(TC::LT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::LT, rhs));
  }
  if (accept(TC::GT)) {
    expect(TC::GT);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::GT, rhs));
  }
  if (accept(TC::LE)) {
    expect(TC::LE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::LE, rhs));
  }
  if (accept(TC::GE)) {
    expect(TC::GE);
    std::shared_ptr<Expr> rhs = parseAddExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::GE, rhs));
  }
  return lhs;
}
std::shared_ptr<Expr> Parser::parseAddExpr() {
  std::shared_ptr<Expr> lhs = parseMulExpr();
  if (accept(TC::PLUS)) {
    expect(TC::PLUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::ADD, rhs));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<Expr> rhs = parseMulExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
  }
  return lhs;
}
std::shared_ptr<Expr> Parser::parseMulExpr() {
  std::shared_ptr<Expr> lhs = parseUnaryExpr();
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::MUL, rhs));
  }
  if (accept(TC::DIV)) {
    expect(TC::DIV);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::DIV, rhs));
  }
  if (accept(TC::REM)) {
    expect(TC::REM);
    std::shared_ptr<Expr> rhs = parseUnaryExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::MOD, rhs));
  }
  return lhs;
}
std::shared_ptr<Expr> Parser::parseUnaryExpr() {
  if (accept(TC::SIZEOF)) {
    expect(TC::SIZEOF);
    expect(TC::LPAR);
    std::shared_ptr<Type> type = parseType();
    expect(TC::RPAR);
    return std::make_shared<SizeOf>(SizeOf(type));
  }
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    std::shared_ptr<Expr> rhs = parseObjExpr();
    return std::make_shared<ValueAt>(ValueAt(rhs));
  }
  if (accept(TC::LPAR) && acceptType(1)) {
    expect(TC::LPAR);
    std::shared_ptr<Type> castType = parseType();
    expect(TC::RPAR);
    std::shared_ptr<Expr> expToCast = parseObjExpr();
    return std::make_shared<TypeCast>(TypeCast(castType, expToCast));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    std::shared_ptr<IntLiteral> lhs(new IntLiteral("0"));
    std::shared_ptr<Expr> rhs = parseObjExpr();
    return std::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
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
      return std::make_shared<FunCall>(FunCall(ident, params));
    }
    return std::make_shared<VarExpr>(VarExpr(ident));
  }

  std::shared_ptr<Expr> lhs = parseLitExpr();
  if (accept(TC::DOT)) {
    expect(TC::DOT);
    std::string fieldIdent = expect(TC::IDENTIFIER).data;
    return std::make_shared<FieldAccess>(FieldAccess(lhs, fieldIdent));
  }
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    std::shared_ptr<Expr> index = parseLitExpr();
    expect(TC::RSBR);
    return std::make_shared<ArrayAccess>(ArrayAccess(lhs, index));
  }

  return lhs;
}
std::shared_ptr<Expr> Parser::parseLitExpr() {
  if (accept(TC::INT_LITERAL)) {
    return std::make_shared<IntLiteral>(
        IntLiteral(expect(TC::INT_LITERAL).data));
  }
  if (accept(TC::CHAR_LITERAL)) {
    return std::make_shared<CharLiteral>(
        CharLiteral(expect(TC::CHAR_LITERAL).data));
  }
  if (accept(TC::STRING_LITERAL)) {
    return std::make_shared<StringLiteral>(
        StringLiteral(expect(TC::STRING_LITERAL).data));
  }
  if (acceptExpr())
    return parseExpr();

  throw std::runtime_error("Parsing: Expected an Expression at " +
                           currToken.position.toString());
}

/* ---- Helpers ---- */

std::shared_ptr<BaseType> Parser::tokenToType(const TC &tc) {
  switch (tc) {
  case TC::INT:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  case TC::CHAR:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
  case TC::SHORT:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::SHORT));
  case TC::VOID:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  default:
    throw std::runtime_error("Parsing: Cannot resolve Token " + tokToStr(tc) +
                             "  to a type.");
  }
}
