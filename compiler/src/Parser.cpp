#include <cassert>
#include <stdexcept>

#include "../include/Parser.h"

using namespace ACC;
using TC = SourceToken::Class;

Parser::Parser(Lexer &lexer)
    : currToken(TC::INVALID, Position(-1, -1, "")), lexer(lexer) {}

atl::shared_ptr<Program> Parser::getAST() {
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

bool Parser::accept(atl::vector<TC> expected, int offset) {
  for (int idx = 0; idx < expected.size(); ++idx) {
    if (accept(expected[idx], offset))
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
      std::string("Parsing: Expected Token ") +
      ACC::tokToStr(expected).c_str() + " at " +
      currToken.position.toString().c_str() +
      " but found: " + ACC::tokToStr(currToken.tokenClass).c_str() + "(" +
      currToken.data.c_str() + ")");
}

SourceToken Parser::expect(atl::vector<TC> expected) {
  for (int idx = 0; idx < expected.size(); ++idx) {
    if (expected[idx] == currToken.tokenClass) {
      SourceToken output = currToken;
      nextToken();
      return output;
    }
  }
  throw std::runtime_error(std::string("Parsing: Invalid Token at ") +
                           currToken.position.toString().c_str());
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
    currToken = tokenBuffer[0];
    tokenBuffer.erase(0);
  } else {
    currToken = lexer.nextToken();
  }
}

/* ---- Look Ahead ---- */

/* -- Decls -- */
bool Parser::acceptClassTypeDecl(int offset) {
  return accept(TC::CLASS, offset);
}
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
bool Parser::acceptNamespace(int offset) {
  return accept(TC::NAMESPACE, offset);
}
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

atl::shared_ptr<Program> Parser::parseProgram() {
  atl::vector<atl::shared_ptr<Decl>> decls;
  while (acceptDecl() || acceptNamespace()) {
    if (acceptDecl())
      decls.push_back(parseDecl());
    else if (acceptNamespace())
      decls.push_back(parseNamespace());
  }

  expect(TC::ENDOFFILE);
  return atl::make_shared<Program>(Program(decls));
}

/* -- Decls -- */
atl::shared_ptr<ClassTypeDecl> Parser::parseClassTypeDecl() {
  expect(TC::CLASS);
  const atl::string classIdentifier = expect(TC::IDENTIFIER).data;
  expect(TC::LBRA);

  expect(TC::RBRA);
  expect(TC::SC);
  return atl::make_shared(ClassTypeDecl(nullptr, {}, {}));
}
atl::shared_ptr<Decl> Parser::parseDecl() {
  if (acceptStructTypeDecl()) {
    atl::shared_ptr<StructTypeDecl> std = parseStructTypeDecl();
    expect(TC::SC);
    return std;
  }

  if (acceptFunDecl()) {
    atl::shared_ptr<FunDecl> fd = parseFunDecl();
    return fd;
  }

  if (acceptVarDecl()) {
    atl::shared_ptr<VarDecl> vd = parseVarDecl();
    expect(TC::SC);
    return vd;
  }

  if (acceptTypeDefDecl()) {
    atl::shared_ptr<TypeDefDecl> tdd = parseTypeDefDecl();
    expect(TC::SC);
    return tdd;
  }

  if (acceptEnumTypeDecl()) {
    atl::shared_ptr<EnumTypeDecl> etd = parseEnumTypeDecl();
    expect(TC::SC);
    return etd;
  }

  throw std::runtime_error("Parser: Expected a Struct/Variable/Function "
                           "Declaration but none was found.");
}
atl::shared_ptr<EnumTypeDecl> Parser::parseEnumTypeDecl() {
  expect(TC::ENUM);
  atl::string ident = "";
  if (accept(TC::IDENTIFIER))
    ident = expect(TC::IDENTIFIER).data;
  expect(TC::LBRA);

  bool moreStates = false;
  std::map<std::string, std::string> states;
  do {
    const atl::string ident = expect(TC::IDENTIFIER).data;
    atl::string value = "";
    if (accept(TC::ASSIGN)) {
      expect(TC::ASSIGN);
      value = expect(TC::INT_LITERAL).data;
    }
    states[std::string(ident.c_str())] = std::string(value.c_str());
    if (accept(TC::COMMA)) {
      expect(TC::COMMA);
      moreStates = true;
    } else
      moreStates = false;
  } while (moreStates);
  expect(TC::RBRA);
  return atl::make_shared<EnumTypeDecl>(EnumTypeDecl(ident, states));
}
atl::shared_ptr<FunDecl> Parser::parseFunDecl() {
  if (accept(TC::EXTERN)) {
    expect(TC::EXTERN);
  }
  atl::shared_ptr<Type> funType = parseType();
  const atl::string funIdent = expect(TC::IDENTIFIER).data;
  expect(TC::LPAR);
  atl::vector<atl::shared_ptr<VarDecl>> funParams;

  bool isDef = true;
  if (acceptParam())
    funParams.push_back(parseParam());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    atl::shared_ptr<VarDecl> currParam = parseParam();
    if (currParam->identifer == "")
      isDef = false;

    funParams.push_back(currParam);
  }

  expect(TC::RPAR);

  if (acceptBlock() && !isDef) {
    throw std::runtime_error(
        "Parser: FunDef signature did not provide names for parameters.");
  } else if (acceptBlock()) {
    atl::shared_ptr<Block> funBlock = parseBlock();
    atl::shared_ptr<FunDef> fd(
        new FunDef(funBlock, funIdent, funParams, funType));
    return fd;
    // return atl::make_shared<FunDef>(
    //     FunDef(funBlock, funIdent, funParams, funType));
  } else {
    expect(TC::SC);
    atl::shared_ptr<FunDecl> fd(new FunDecl(funIdent, funParams, funType));
    return fd;
    // return atl::make_shared<FunDecl>(FunDecl(funIdent, funParams,
    // funType));
  }
}
atl::shared_ptr<StructTypeDecl> Parser::parseStructTypeDecl() {
  atl::shared_ptr<StructType> structType = parseStructType();
  expect(TC::LBRA);
  atl::vector<atl::shared_ptr<VarDecl>> fields;
  do {
    atl::shared_ptr<VarDecl> structField = parseVarDecl();
    expect(TC::SC);
    fields.push_back(structField);
  } while (acceptVarDecl());

  expect(TC::RBRA);

  return atl::make_shared<StructTypeDecl>(StructTypeDecl(structType, fields));
}
atl::shared_ptr<TypeDefDecl> Parser::parseTypeDefDecl() {
  expect(TC::TYPEDEF);
  atl::shared_ptr<Type> aliasedType;
  if (acceptStructTypeDecl()) {
    atl::shared_ptr<StructTypeDecl> std = parseStructTypeDecl();
    aliasedType = std->structType;
  } else if (acceptEnumTypeDecl()) {
    atl::shared_ptr<EnumTypeDecl> etd = parseEnumTypeDecl();
    aliasedType = etd;
  } else {
    aliasedType = parseType();
  }
  const atl::string typeAlias = expect(TC::IDENTIFIER).data;
  return atl::make_shared<TypeDefDecl>(TypeDefDecl(aliasedType, typeAlias));
}
atl::shared_ptr<VarDecl> Parser::parseVarDecl() {
  bool isExtern = false;
  if (accept(TC::EXTERN)) {
    expect(TC::EXTERN);
    isExtern = true;
  }
  atl::shared_ptr<Type> varType = parseType();
  const atl::string varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    atl::string arraySize;
    if (!isExtern || accept(TC::INT_LITERAL))
      arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = atl::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return atl::make_shared<VarDecl>(VarDecl(varType, varIdentifier, isExtern));
}

/* -- Types -- */
atl::shared_ptr<StructType> Parser::parseStructType() {
  expect(TC::STRUCT);
  const atl::string structIdentifier = expect(TC::IDENTIFIER).data;
  return atl::make_shared<StructType>(StructType(structIdentifier));
}
atl::shared_ptr<Type> Parser::parseType() {
  atl::shared_ptr<Type> type;
  if (acceptStructType()) {
    type = parseStructType();
    while (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      type = atl::shared_ptr<PointerType>(new PointerType(type));
    }
  } else {
    atl::vector<TC> modifiers;
    while (accept(TC::UNSIGNED))
      modifiers.push_back(expect(TC::UNSIGNED).tokenClass);

    const SourceToken baseType =
        expect({TC::INT, TC::CHAR, TC::VOID, TC::SHORT});
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
    type = atl::shared_ptr<BaseType>(new BaseType(pType));
    while (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      type = atl::shared_ptr<PointerType>(new PointerType(type));
    }
  }
  return type;
}

/* -- Stmts -- */
atl::shared_ptr<Assign> Parser::parseAssign() {
  atl::shared_ptr<Expr> lhs = parseExpr();
  expect(TC::ASSIGN);
  atl::shared_ptr<Expr> rhs = parseExpr();
  return atl::make_shared<Assign>(Assign(lhs, rhs));
}
atl::shared_ptr<Block> Parser::parseBlock() {
  expect(TC::LBRA);

  atl::vector<atl::shared_ptr<Stmt>> blockStmts;

  while (acceptStmt()) {
    blockStmts.push_back(parseStmt());
  }

  expect(TC::RBRA);
  return atl::make_shared<Block>(Block(blockStmts));
}
atl::shared_ptr<DoWhile> Parser::parseDoWhile() {
  expect(TC::DO);
  atl::shared_ptr<Stmt> whileBody = parseStmt();
  expect(TC::WHILE);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  expect(TC::SC);

  return atl::make_shared<DoWhile>(DoWhile(whileBody, whileCondition));
}
atl::shared_ptr<If> Parser::parseIf() {
  expect(TC::IF);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> ifCondition = parseExpr();
  expect(TC::RPAR);
  atl::shared_ptr<Stmt> ifBody = parseStmt();
  if (accept(TC::ELSE)) {
    expect(TC::ELSE);
    atl::shared_ptr<Stmt> elseBody = parseStmt();
    return atl::make_shared<If>(If(ifCondition, ifBody, elseBody));
  } else {
    return atl::make_shared<If>(If(ifCondition, ifBody));
  }
}
atl::shared_ptr<Namespace> Parser::parseNamespace() {
  expect(TC::NAMESPACE);
  atl::string namespaceIdent;
  if (accept(TC::IDENTIFIER))
    namespaceIdent = expect(TC::IDENTIFIER).data;
  expect(TC::LBRA);
  atl::vector<atl::shared_ptr<Decl>> namespaceDecls;
  while (acceptDecl() || acceptNamespace()) {
    if (acceptDecl())
      namespaceDecls.push_back(parseDecl());
    else if (acceptNamespace())
      namespaceDecls.push_back(parseNamespace());
  }
  expect(TC::RBRA);
  return atl::make_shared<Namespace>(Namespace(namespaceIdent, namespaceDecls));
}
atl::shared_ptr<Return> Parser::parseReturn() {
  expect(TC::RETURN);
  if (acceptExpr()) {
    atl::shared_ptr<Expr> returnExpr = parseExpr();
    expect(TC::SC);
    return atl::make_shared<Return>(Return(returnExpr));
  } else {
    expect(TC::SC);
    return atl::make_shared<Return>(Return());
  }
}
atl::shared_ptr<Stmt> Parser::parseStmt() {
  if (acceptVarDecl()) {
    atl::shared_ptr<VarDecl> vd = parseVarDecl();
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

  atl::shared_ptr<Expr> expr = parseExpr();

  if (accept(TC::ASSIGN)) {
    expect(TC::ASSIGN);
    atl::shared_ptr<Expr> rhs = parseExpr();
    expect(TC::SC);
    return atl::make_shared<Assign>(Assign(expr, rhs));
  }

  expect(TC::SC);
  return expr;
}
atl::shared_ptr<While> Parser::parseWhile() {
  expect(TC::WHILE);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  atl::shared_ptr<Stmt> whileBody = parseStmt();

  return atl::make_shared<While>(While(whileBody, whileCondition));
}

atl::shared_ptr<VarDecl> Parser::parseParam() {
  atl::shared_ptr<Type> varType = parseType();
  atl::string varIdentifier = "";
  if (accept(TC::IDENTIFIER))
    varIdentifier = expect(TC::IDENTIFIER).data;
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    const atl::string arraySize = expect(TC::INT_LITERAL).data;
    expect(TC::RSBR);
    varType = atl::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return atl::make_shared<VarDecl>(VarDecl(varType, varIdentifier));
}

/* -- Exprs -- */
atl::shared_ptr<Expr> Parser::parseExpr() {
  if (accept(TC::LPAR) && !acceptType(1)) {
    expect(TC::LPAR);
    atl::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return atl::make_shared<ParenthExpr>(ParenthExpr(innerExpr));
  }

  return parseBoolExpr();
}
atl::shared_ptr<Expr> Parser::parseBoolExpr() {
  atl::shared_ptr<Expr> lhs = parseEqualExpr();
  if (accept(TC::OR)) {
    expect(TC::OR);
    atl::shared_ptr<Expr> rhs = parseEqualExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::OR, rhs));
  }
  if (accept(TC::AND)) {
    expect(TC::AND);
    atl::shared_ptr<Expr> rhs = parseEqualExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::AND, rhs));
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseEqualExpr() {
  atl::shared_ptr<Expr> lhs = parseCompExpr();
  if (accept(TC::NE)) {
    expect(TC::NE);
    atl::shared_ptr<Expr> rhs = parseCompExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::NE, rhs));
  }
  if (accept(TC::EQ)) {
    expect(TC::EQ);
    atl::shared_ptr<Expr> rhs = parseCompExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::EQ, rhs));
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseCompExpr() {
  atl::shared_ptr<Expr> lhs = parseAddExpr();
  if (accept(TC::LT)) {
    expect(TC::LT);
    atl::shared_ptr<Expr> rhs = parseAddExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::LT, rhs));
  }
  if (accept(TC::GT)) {
    expect(TC::GT);
    atl::shared_ptr<Expr> rhs = parseAddExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::GT, rhs));
  }
  if (accept(TC::LE)) {
    expect(TC::LE);
    atl::shared_ptr<Expr> rhs = parseAddExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::LE, rhs));
  }
  if (accept(TC::GE)) {
    expect(TC::GE);
    atl::shared_ptr<Expr> rhs = parseAddExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::GE, rhs));
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseAddExpr() {
  atl::shared_ptr<Expr> lhs = parseMulExpr();
  if (accept(TC::PLUS)) {
    expect(TC::PLUS);
    atl::shared_ptr<Expr> rhs = parseMulExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::ADD, rhs));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    atl::shared_ptr<Expr> rhs = parseMulExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseMulExpr() {
  atl::shared_ptr<Expr> lhs = parseUnaryExpr();
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    atl::shared_ptr<Expr> rhs = parseUnaryExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::MUL, rhs));
  }
  if (accept(TC::DIV)) {
    expect(TC::DIV);
    atl::shared_ptr<Expr> rhs = parseUnaryExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::DIV, rhs));
  }
  if (accept(TC::REM)) {
    expect(TC::REM);
    atl::shared_ptr<Expr> rhs = parseUnaryExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::MOD, rhs));
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseUnaryExpr() {
  if (accept(TC::SIZEOF)) {
    expect(TC::SIZEOF);
    expect(TC::LPAR);
    atl::shared_ptr<Type> type = parseType();
    expect(TC::RPAR);
    return atl::make_shared<SizeOf>(SizeOf(type));
  }
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    atl::shared_ptr<Expr> rhs = parseObjExpr();
    return atl::make_shared<ValueAt>(ValueAt(rhs));
  }
  if (accept(TC::LPAR) && acceptType(1)) {
    expect(TC::LPAR);
    atl::shared_ptr<Type> castType = parseType();
    expect(TC::RPAR);
    atl::shared_ptr<Expr> expToCast = parseObjExpr();
    return atl::make_shared<TypeCast>(TypeCast(castType, expToCast));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    atl::shared_ptr<IntLiteral> lhs(new IntLiteral("0"));
    atl::shared_ptr<Expr> rhs = parseObjExpr();
    return atl::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
  }

  return parseObjExpr();
}
atl::shared_ptr<Expr> Parser::parseObjExpr() {
  if (accept(TC::IDENTIFIER)) {
    const atl::string ident = expect(TC::IDENTIFIER).data;
    if (accept(TC::LPAR)) {
      expect(TC::LPAR);

      atl::vector<atl::shared_ptr<Expr>> params;
      if (acceptExpr())
        params.push_back(parseLitExpr());
      while (accept(TC::COMMA)) {
        expect(TC::COMMA);
        params.push_back(parseLitExpr());
      }

      expect(TC::RPAR);
      return atl::make_shared<FunCall>(FunCall(ident, params));
    }
    return atl::make_shared<VarExpr>(VarExpr(ident));
  }

  atl::shared_ptr<Expr> lhs = parseLitExpr();
  if (accept(TC::DOT)) {
    expect(TC::DOT);
    const atl::string fieldIdent = expect(TC::IDENTIFIER).data;
    return atl::make_shared<FieldAccess>(FieldAccess(lhs, fieldIdent));
  }
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    atl::shared_ptr<Expr> index = parseLitExpr();
    expect(TC::RSBR);
    return atl::make_shared<ArrayAccess>(ArrayAccess(lhs, index));
  }

  return lhs;
}
atl::shared_ptr<Expr> Parser::parseLitExpr() {
  if (accept(TC::INT_LITERAL)) {
    return atl::make_shared<IntLiteral>(
        IntLiteral(expect(TC::INT_LITERAL).data));
  }
  if (accept(TC::CHAR_LITERAL)) {
    return atl::make_shared<CharLiteral>(
        CharLiteral(expect(TC::CHAR_LITERAL).data));
  }
  if (accept(TC::STRING_LITERAL)) {
    return atl::make_shared<StringLiteral>(
        StringLiteral(expect(TC::STRING_LITERAL).data));
  }
  if (acceptExpr())
    return parseExpr();

  throw std::runtime_error(std::string("Parsing: Expected an Expression at ") +
                           currToken.position.toString().c_str());
}

/* ---- Helpers ---- */

atl::shared_ptr<BaseType> Parser::tokenToType(const TC &tc) {
  switch (tc) {
  case TC::INT:
    return atl::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  case TC::CHAR:
    return atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
  case TC::SHORT:
    return atl::make_shared<BaseType>(BaseType(PrimitiveType::SHORT));
  case TC::VOID:
    return atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  default:
    throw std::runtime_error(std::string("Parsing: Cannot resolve Token ") +
                             tokToStr(tc).c_str() + "  to a type.");
  }
}
