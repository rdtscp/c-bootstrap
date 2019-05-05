#include "atl/include/set.h"

#include "Error.h"
#include "Parser.h"

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
  throw ACC::Error("Parsing: Expected Token " + ACC::tokToStr(expected) +
                       " but found " + ACC::tokToStr(currToken.tokenClass),
                   currToken.position);
}

SourceToken Parser::expect(atl::vector<TC> expected) {
  for (int idx = 0; idx < expected.size(); ++idx) {
    if (expected[idx] == currToken.tokenClass) {
      SourceToken output = currToken;
      nextToken();
      return output;
    }
  }
  throw ACC::Error("Parsing: Unexpected Token " +
                       ACC::tokToStr(currToken.tokenClass),
                   currToken.position);
}

SourceToken Parser::lookAhead(int i) {
  if (i < 0)
    throw Error("Parser: Cannot lookAhead negative indices.");

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
bool Parser::acceptAccessModifier(int offset) {
  return accept(TC::PUBLIC, offset) | accept(TC::PRIVATE, offset) |
         accept(TC::PROTECTED, offset);
}
bool Parser::acceptClassTypeDecl(int offset) {
  return accept(TC::CLASS, offset);
}
bool Parser::acceptConstructor(int offset) {
  if (!accept(TC::IDENTIFIER, offset))
    return false;

  if (!accept(TC::LPAR, offset + 1))
    return false;

  return true;
}
bool Parser::acceptDecl(int offset) {
  if (acceptClassTypeDecl(offset))
    return true;

  if (acceptStructTypeDecl(offset))
    return true;

  if (acceptVarDecl(offset))
    return true;

  if (acceptFunDecl(offset))
    return true;

  if (acceptTypeDefDecl())
    return true;

  if (acceptEnumClassTypeDecl())
    return true;

  return false;
}
bool Parser::acceptDestructor(int offset) {
  if (!accept(TC::DESTRUCTOR, offset))
    return false;

  if (!accept(TC::IDENTIFIER, offset + 1))
    return false;

  return true;
}
bool Parser::acceptEnumClassTypeDecl(int offset) {
  return (accept(TC::ENUM, offset) && accept(TC::CLASS, offset + 1));
}
bool Parser::acceptFunDecl(int offset) {
  if (accept(TC::STATIC))
    ++offset;

  if (acceptStructType(offset)) {
    offset += 2;
    while (accept(TC::ASTERIX, offset))
      ++offset;
    return (accept(TC::IDENTIFIER, offset) || acceptOpOverload(offset)) &&
           accept(TC::LPAR, offset + 1);
  }

  if (acceptType(offset)) {
    if (accept(TC::CONST))
      ++offset;
    ++offset;
    if (accept(TC::ASTERIX, offset) && !accept(TC::REF, offset))
      while (accept(TC::ASTERIX, offset))
        ++offset;

    else if (accept(TC::REF, offset)) {
      ++offset;
    } else if (accept(TC::AND, offset)) {
      ++offset;
      ++offset;
    }

    return (accept(TC::IDENTIFIER, offset) || acceptOpOverload(offset)) &&
           accept(TC::LPAR, offset + 1);
  }

  return false;
}
bool Parser::acceptOpOverload(int offset) {
  return accept({TC::OPASSIGN, TC::OPAT, TC::OPADD, TC::OPADDTO, TC::OPEQ,
                 TC::OPGE, TC::OPGT, TC::OPLE, TC::OPLT, TC::OPNE},
                offset);
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
  if (accept(TC::STATIC))
    ++offset;

  if (!acceptType(offset))
    return false;

  if (accept(TC::CONST))
    ++offset;

  if (accept(TC::STRUCT, offset)) {
    ++offset;
    if (!accept(TC::IDENTIFIER, offset))
      return false;

    ++offset;
    while (accept(TC::ASTERIX, offset))
      ++offset;

    if (!accept(TC::IDENTIFIER, offset))
      return false;

  } else if (accept(
                 {TC::INT, TC::CHAR, TC::SHORT, TC::VOID, TC::UINT, TC::BOOL},
                 offset)) {
    ++offset;
    if (accept(TC::ASTERIX, offset) && !accept(TC::REF, offset) &&
        !accept(TC::AND, offset))
      while (accept(TC::ASTERIX, offset))
        ++offset;

    else if (accept(TC::REF, offset)) {
      ++offset;
    } else if (accept(TC::AND, offset)) {
      ++offset;
      ++offset;
    }

    if (!accept(TC::IDENTIFIER, offset))
      return false;
  } else if (accept(TC::IDENTIFIER, offset)) {
    ++offset;
    while (accept(TC::NAMESPACEACCESS, offset) &&
           accept(TC::IDENTIFIER, offset + 1))
      offset = offset + 2;
    if (accept(TC::ASTERIX, offset) && !accept(TC::REF, offset) &&
        !accept(TC::AND, offset))
      while (accept(TC::ASTERIX, offset))
        ++offset;

    else if (accept(TC::REF, offset)) {
      ++offset;
    } else if (accept(TC::AND, offset)) {
      ++offset;
      ++offset;
    }

    if (!accept(TC::IDENTIFIER, offset))
      return false;
  } else {
    return false;
  }

  return true;
}

/* -- Types -- */
bool Parser::acceptClassType(int offset) {
  return accept(TC::IDENTIFIER, offset);
}
bool Parser::acceptStructType(int offset) {
  if (!accept(TC::STRUCT, offset))
    return false;

  if (!accept(TC::IDENTIFIER, offset + 1))
    return false;

  return true;
}
bool Parser::acceptType(int offset) {
  if (accept(TC::CONST, offset))
    ++offset;

  if (accept({TC::INT, TC::CHAR, TC::VOID, TC::UINT, TC::BOOL}, offset))
    return true;
  if (accept(TC::STRUCT, offset) && accept(TC::IDENTIFIER, offset + 1))
    return true;

  if (accept(TC::IDENTIFIER, offset))
    return true;

  return false;
}

/* -- Stmts -- */
bool Parser::acceptAssign(int offset) { return acceptExpr(offset); }
bool Parser::acceptBlock(int offset) { return accept(TC::LBRA, offset); }
bool Parser::acceptDeletion(int offset) {
  return accept({TC::DELETE, TC::DELETEARR}, offset);
}
bool Parser::acceptDoWhile(int offset) { return accept(TC::DO, offset); }
bool Parser::acceptFor(int offset) { return accept(TC::FOR, offset); }
bool Parser::acceptIf(int offset) { return accept(TC::IF, offset); }
bool Parser::acceptNamespace(int offset) {
  return accept(TC::NAMESPACE, offset);
}
bool Parser::acceptReturn(int offset) { return accept(TC::RETURN, offset); }
bool Parser::acceptStmt(int offset) {
  if (acceptDeletion(offset))
    return true;
  if (acceptVarDecl(offset))
    return true;
  if (acceptBlock(offset))
    return true;
  if (acceptWhile(offset))
    return true;
  if (acceptDoWhile(offset))
    return true;
  if (acceptFor())
    return true;
  if (acceptIf(offset))
    return true;
  if (acceptReturn(offset))
    return true;
  if (acceptAssign(offset))
    return true;
  if (acceptExpr(offset))
    return true;
  if (acceptThrow(offset))
    return true;

  return false;
}
bool Parser::acceptThrow(int offset) { return accept(TC::THROW, offset); }
bool Parser::acceptWhile(int offset) { return accept(TC::WHILE, offset); }

bool Parser::acceptExpr(int offset) {
  return accept({TC::LPAR, TC::SIZEOF, TC::ASTERIX, TC::MINUS, TC::IDENTIFIER,
                 TC::INT_LITERAL, TC::CHAR_LITERAL, TC::STRING_LITERAL, TC::NEW,
                 TC::PREFIXINC, TC::TRUE_VAL, TC::FALSE_VAL, TC::NOT, TC::REF,
                 TC::THIS},
                offset);
}
bool Parser::acceptObjExpr(int offset) {
  return accept({TC::IDENTIFIER, TC::THIS}, offset);
}
bool Parser::acceptObjExprOp(int offset) {
  return accept({TC::DOT, TC::PTRDOT, TC::LSBR}, offset);
}
bool Parser::acceptLitExpr(int offset) {
  return accept({TC::INT_LITERAL, TC::CHAR_LITERAL, TC::STRING_LITERAL,
                 TC::TRUE_VAL, TC::FALSE_VAL},
                offset);
}

bool Parser::acceptParam(int offset) { return acceptType(offset); }

bool Parser::acceptOperatorOverload(int offset) {
  return accept({TC::OPASSIGN, TC::OPAT, TC::OPADD, TC::OPADDTO, TC::OPEQ,
                 TC::OPGE, TC::OPGT, TC::OPLE, TC::OPLT, TC::OPNE},
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
  return atl::shared_ptr<Program>(new Program(decls));
}

atl::shared_ptr<Identifier> Parser::parseIdentifier() {
  atl::shared_ptr<Identifier> identifier;
  if (acceptOperatorOverload()) {
    identifier =
        atl::make_shared<Identifier>(Identifier(parseOperatorOverload()));
  } else {
    identifier =
        atl::make_shared<Identifier>(Identifier(expect(TC::IDENTIFIER).data));
    while (accept(TC::NAMESPACEACCESS)) {
      expect(TC::NAMESPACEACCESS);
      if (acceptOperatorOverload()) {
        identifier =
            atl::make_shared<Identifier>(Identifier(parseOperatorOverload()));
        break;
      }
      identifier = atl::make_shared<Identifier>(
          Identifier(expect(TC::IDENTIFIER).data, identifier));
    }
  }
  return identifier;
}

/* -- Decls -- */
atl::shared_ptr<ClassTypeDecl> Parser::parseClassTypeDecl() {
  expect(TC::CLASS);
  const atl::shared_ptr<Identifier> classIdentifier = parseIdentifier();
  expect(TC::LBRA);

  atl::shared_ptr<ClassType> classType(new ClassType(classIdentifier));

  atl::vector<atl::shared_ptr<Decl>> classDecls;
  TC currVisibility = TC::PRIVATE;

  while (acceptDecl() || acceptAccessModifier() || acceptConstructor() ||
         acceptDestructor()) {
    atl::shared_ptr<Decl> declaration;
    if (acceptAccessModifier()) {
      currVisibility =
          expect({TC::PUBLIC, TC::PRIVATE, TC::PROTECTED}).tokenClass;
      continue;
    } else if (acceptDecl()) {
      declaration = parseDecl();
    } else if (acceptDestructor()) {
      declaration = parseDestructor();
    } else {
      declaration = parseConstructor();
    }

    switch (currVisibility) {
    case TC::PUBLIC:
      declaration->visibility = Decl::Visibility::PUBLIC;
      break;
    case TC::PRIVATE:
      declaration->visibility = Decl::Visibility::PRIVATE;
      break;
    case TC::PROTECTED:
      declaration->visibility = Decl::Visibility::PROTECTED;
      break;
    default:
      declaration->visibility = Decl::Visibility::NONE;
      break;
    }

    classDecls.push_back(declaration);
  }

  expect(TC::RBRA);
  return atl::make_shared(ClassTypeDecl(classType, classDecls));
}
atl::shared_ptr<ConstructorDecl> Parser::parseConstructor() {
  const atl::shared_ptr<Identifier> constructorIdentifier = parseIdentifier();
  const atl::shared_ptr<ClassType> classType(
      new ClassType(constructorIdentifier));
  expect(TC::LPAR);
  atl::vector<atl::shared_ptr<VarDecl>> constructorParams;

  bool unnamedParams = false;
  if (acceptParam())
    constructorParams.push_back(parseParam());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    atl::shared_ptr<VarDecl> currParam = parseParam();
    if (!currParam->identifier || currParam->identifier->toString() == "")
      unnamedParams = true;

    constructorParams.push_back(currParam);
  }

  expect(TC::RPAR);

  if (acceptBlock() && unnamedParams) {
    throw ACC::Error(
        "Parser: Constructor signature did not provide names for parameters.",
        currToken.position);
  } else if (acceptBlock()) {
    atl::shared_ptr<Block> constructorBlock = parseBlock();
    atl::shared_ptr<ConstructorDef> cd(
        new ConstructorDef(classType, constructorParams, {}, constructorBlock));
    return cd;
  } else if (accept(TC::COLON)) {
    expect(TC::COLON);
    atl::vector<atl::shared_ptr<Assign>> initialiserList;
    {
      const atl::shared_ptr<VarExpr> initialiserMember(
          new VarExpr(parseIdentifier()));
      expect(TC::LPAR);
      const atl::shared_ptr<Expr> initialiseValue = parseExpr();
      expect(TC::RPAR);
      const atl::shared_ptr<Assign> intialiserStmt(
          new Assign(initialiserMember, initialiseValue));
      initialiserList.push_back(intialiserStmt);
    }
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      const atl::shared_ptr<Identifier> identifier = parseIdentifier();
      const atl::shared_ptr<VarExpr> initialiserMember(new VarExpr(identifier));
      expect(TC::LPAR);
      const atl::shared_ptr<Expr> initialiseValue = parseExpr();
      expect(TC::RPAR);
      const atl::shared_ptr<Assign> intialiserStmt(
          new Assign(initialiserMember, initialiseValue));
      initialiserList.push_back(intialiserStmt);
    }
    atl::shared_ptr<Block> constructorBlock = parseBlock();
    atl::shared_ptr<ConstructorDef> cd(new ConstructorDef(
        classType, constructorParams, initialiserList, constructorBlock));
    return cd;
  } else {
    expect(TC::SC);
    atl::shared_ptr<ConstructorDecl> cd(
        new ConstructorDecl(classType, constructorParams));
    return cd;
  }
}
atl::shared_ptr<Decl> Parser::parseDecl() {
  if (acceptClassTypeDecl()) {
    atl::shared_ptr<ClassTypeDecl> ctd = parseClassTypeDecl();
    expect(TC::SC);
    return ctd;
  }
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

  if (acceptEnumClassTypeDecl()) {
    atl::shared_ptr<EnumClassTypeDecl> ectd = parseEnumClassTypeDecl();
    expect(TC::SC);
    return ectd;
  }

  throw ACC::Error("Parser: Expected a Struct/Variable/Function Declaration "
                   "but none was found.",
                   currToken.position);
}
atl::shared_ptr<Deletion> Parser::parseDeletion() {
  TC deletionToken = expect({TC::DELETE, TC::DELETEARR}).tokenClass;
  Deletion::DelType deletionType = Deletion::DelType::OBJECT;
  if (deletionToken == TC::DELETEARR)
    deletionType = Deletion::DelType::ARRAY;

  const atl::shared_ptr<VarExpr> deletionVar = parseObjExpr();
  return atl::make_shared<Deletion>(Deletion(deletionType, deletionVar));
}
atl::shared_ptr<DestructorDecl> Parser::parseDestructor() {
  expect(TC::DESTRUCTOR);
  const atl::shared_ptr<Identifier> classIdentifier = parseIdentifier();
  const atl::shared_ptr<ClassType> classType(new ClassType(classIdentifier));
  expect(TC::LPAR);
  expect(TC::RPAR);
  if (acceptBlock()) {
    const atl::shared_ptr<Block> destructorBlock = parseBlock();
    const atl::shared_ptr<DestructorDef> dd(
        new DestructorDef(classType, destructorBlock));
    return dd;
  } else {
    expect(TC::SC);
    const atl::shared_ptr<DestructorDecl> dd(new DestructorDecl(classType));
    return dd;
  }
}

atl::shared_ptr<EnumClassTypeDecl> Parser::parseEnumClassTypeDecl() {
  expect(TC::ENUM);
  expect(TC::CLASS);
  const atl::shared_ptr<Identifier> enumIdentifier = parseIdentifier();
  expect(TC::LBRA);

  bool moreStates = false;
  atl::unordered_map<atl::string, atl::string> states;
  do {
    const atl::string stateIdentifier = expect(TC::IDENTIFIER).data;
    atl::string stateValue = "";
    if (accept(TC::ASSIGN)) {
      expect(TC::ASSIGN);
      if (accept(TC::MINUS)) {
        expect(TC::MINUS);
        stateValue = "-";
      }
      stateValue += expect(TC::INT_LITERAL).data;
    }
    states[stateIdentifier] = stateValue;
    if (accept(TC::COMMA)) {
      expect(TC::COMMA);
      moreStates = true;
    } else
      moreStates = false;
  } while (moreStates);
  expect(TC::RBRA);
  return atl::make_shared<EnumClassTypeDecl>(
      EnumClassTypeDecl(enumIdentifier, states));
}
atl::shared_ptr<FunDecl> Parser::parseFunDecl() {
  atl::set<FunDecl::FunModifiers> funModifiers;
  if (accept(TC::STATIC)) {
    expect(TC::STATIC);
    funModifiers.insert(FunDecl::FunModifiers::STATIC);
  }

  atl::shared_ptr<Type> funType = parseType();
  atl::shared_ptr<Identifier> funIdentifier;
  if (acceptOpOverload()) {
    const atl::string identifier = parseOperatorOverload();
    funIdentifier = atl::make_shared<Identifier>(Identifier(identifier));
  } else {
    funIdentifier = parseIdentifier();
  }
  expect(TC::LPAR);
  atl::vector<atl::shared_ptr<VarDecl>> funParams;

  bool isDef = true;
  if (acceptParam())
    funParams.push_back(parseParam());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    atl::shared_ptr<VarDecl> currParam = parseParam();
    if (!currParam->identifier || currParam->identifier->toString() == "")
      isDef = false;

    funParams.push_back(currParam);
  }

  expect(TC::RPAR);

  if (accept(TC::CONST)) {
    expect(TC::CONST);
    funModifiers.insert(FunDecl::FunModifiers::CONST);
  }
  // TODO: Modifiers for FunDecls

  if (acceptBlock() && !isDef) {
    throw ACC::Error(
        "Parser: FunDef signature did not provide names for parameters.",
        currToken.position);
  } else if (acceptBlock()) {
    atl::shared_ptr<Block> funBlock = parseBlock();
    atl::shared_ptr<FunDef> fd(
        new FunDef(funModifiers, funIdentifier, funParams, funType, funBlock));
    return fd;
    // return atl::make_shared<FunDef>(
    //     FunDef(funBlock, funIdent, funParams, funType));
  } else {
    expect(TC::SC);
    atl::shared_ptr<FunDecl> fd(
        new FunDecl(funModifiers, funIdentifier, funParams, funType));
    return fd;
    // return atl::make_shared<FunDecl>(FunDecl(funIdent, funParams,
    // funType));
  }
}
atl::shared_ptr<StructTypeDecl> Parser::parseStructTypeDecl() {
  expect(TC::STRUCT);
  const atl::shared_ptr<Identifier> structIdentifier = parseIdentifier();
  atl::shared_ptr<StructType> structType(new StructType(structIdentifier));
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
  } else {
    aliasedType = parseType();
  }
  const atl::shared_ptr<Identifier> typeAlias = parseIdentifier();
  return atl::make_shared<TypeDefDecl>(TypeDefDecl(aliasedType, typeAlias));
}
atl::shared_ptr<VarDecl> Parser::parseVarDecl() {
  if (accept(TC::STATIC))
    // TODO: Handle Modifier
    expect(TC::STATIC);

  atl::shared_ptr<Type> varType = parseType();
  const atl::shared_ptr<Identifier> varIdentifier = parseIdentifier();
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    const atl::shared_ptr<Expr> arraySize = parseLitExpr();
    expect(TC::RSBR);
    varType = atl::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  if (accept(TC::ASSIGN)) {
    expect(TC::ASSIGN);
    atl::shared_ptr<Expr> assignExpr = parseExpr();
    return atl::make_shared<VarDef>(VarDef(varType, varIdentifier, assignExpr));
  } else if (accept(TC::LPAR)) {
    expect(TC::LPAR);
    if (varType->astClass() != "ClassType")
      throw ACC::Error(
          "Construction of Variable of non-class type not supported.",
          currToken.position);

    atl::shared_ptr<ClassType> classType =
        atl::static_pointer_cast<ClassType>(varType);
    /* Parse Params */
    atl::vector<atl::shared_ptr<Expr>> params;
    if (acceptExpr())
      params.push_back(parseLitExpr());
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      params.push_back(parseLitExpr());
    }
    expect(TC::RPAR);
    const atl::shared_ptr<FunCall> constructorCall(
        new FunCall(classType->identifier, params));
    return atl::make_shared<VarDef>(
        VarDef(varType, varIdentifier, constructorCall));
  } else {
    return atl::make_shared<VarDecl>(VarDecl(varType, varIdentifier));
  }
}

/* -- Types -- */
atl::shared_ptr<Type> Parser::parseType() {
  atl::set<Type::Modifiers> typeModifiers;
  if (accept(TC::CONST)) {
    expect(TC::CONST);
    typeModifiers.insert(Type::Modifiers::CONST);
  }
  atl::shared_ptr<Type> type;
  if (acceptStructType()) {
    expect(TC::STRUCT);
    const atl::shared_ptr<Identifier> structIdentifier = parseIdentifier();
    type = atl::make_shared<StructType>(StructType(structIdentifier));
  } else if (acceptClassType()) {
    const atl::shared_ptr<Identifier> classIdentifier = parseIdentifier();
    type = atl::make_shared<ClassType>(ClassType(classIdentifier));
  } else {
    const TC baseType =
        expect({TC::INT, TC::CHAR, TC::VOID, TC::SHORT, TC::UINT, TC::BOOL})
            .tokenClass;
    PrimitiveType pType;
    switch (baseType) {
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
    case TC::UINT:
      pType = PrimitiveType::UINT;
      break;
    case TC::BOOL:
      pType = PrimitiveType::BOOL;
      break;
    default:
      pType = PrimitiveType::VOID;
      break;
    }
    type = atl::shared_ptr<BaseType>(new BaseType(pType));
  }
  if (accept(TC::ASTERIX)) {
    while (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      type = atl::shared_ptr<PointerType>(new PointerType(type));
    }
  } else if (accept(TC::REF)) {
    expect(TC::REF);
    type = atl::shared_ptr<ReferenceType>(new ReferenceType(type));
  } else if (accept(TC::AND)) {
    expect(TC::AND);
    type = atl::shared_ptr<ReferenceType>(new ReferenceType(type));
    type = atl::shared_ptr<ReferenceType>(new ReferenceType(type));
  }

  type->typeModifiers = typeModifiers;
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
atl::shared_ptr<For> Parser::parseFor() {
  expect(TC::FOR);
  expect(TC::LPAR);
  const atl::shared_ptr<VarDecl> initialVarDecl = parseVarDecl();
  expect(TC::SC);
  const atl::shared_ptr<Expr> condition = parseExpr();
  expect(TC::SC);
  const atl::shared_ptr<Expr> endBodyExpr = parseExpr();
  /* TODO: Check this Stmt is Valid ASTNode Type. */
  if (endBodyExpr->astClass() != "PrefixOp")
    throw ACC::Error(
        "TEMP: For Loops do not support end of body expressions other than "
        "prefix operations");
  expect(TC::RPAR);
  const atl::shared_ptr<Stmt> body = parseStmt();
  return atl::make_shared<For>(
      For(initialVarDecl, condition, endBodyExpr, body));
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
  atl::shared_ptr<Identifier> namespaceIdent;
  if (accept(TC::IDENTIFIER))
    namespaceIdent = parseIdentifier();
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
  if (acceptDeletion()) {
    atl::shared_ptr<Deletion> d = parseDeletion();
    expect(TC::SC);
    return d;
  }

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

  if (acceptFor())
    return parseFor();

  if (acceptIf())
    return parseIf();

  if (acceptReturn())
    return parseReturn();

  if (acceptThrow())
    return parseThrow();

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
atl::shared_ptr<Throw> Parser::parseThrow() {
  expect(TC::THROW);
  const atl::string stringLiteral = expect(TC::STRING_LITERAL).data;
  expect(TC::SC);
  return atl::make_shared<Throw>(
      Throw(atl::make_shared<StringLiteral>(StringLiteral(stringLiteral))));
}
atl::shared_ptr<While> Parser::parseWhile() {
  expect(TC::WHILE);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  atl::shared_ptr<Stmt> whileBody = parseStmt();

  return atl::make_shared<While>(While(whileBody, whileCondition));
}

/* -- Exprs -- */
atl::shared_ptr<Expr> Parser::parseExpr() {
  atl::shared_ptr<Expr> output = parseBoolExpr();
  while (accept({TC::QMARK, TC::ASSIGNADD})) {
    if (accept(TC::QMARK)) {
      const atl::shared_ptr<Expr> &tertiaryCondition = output;
      expect(TC::QMARK);
      const atl::shared_ptr<Expr> tertiaryIfBody = parseBoolExpr();
      expect(TC::COLON);
      const atl::shared_ptr<Expr> tertiaryElseBody = parseBoolExpr();
      output = atl::make_shared<TertiaryExpr>(
          TertiaryExpr(tertiaryCondition, tertiaryIfBody, tertiaryElseBody));
    } else {
      const atl::shared_ptr<Expr> &lhs = output;
      expect(TC::ASSIGNADD);
      const atl::shared_ptr<Expr> rhs = parseBoolExpr();
      output = atl::make_shared<BinOp>(BinOp(lhs, Op::ASSIGNADD, rhs));
    }
  }
  return output;
}
atl::shared_ptr<Expr> Parser::parseBoolExpr() {
  atl::shared_ptr<Expr> lhs = parseEqualExpr();
  atl::shared_ptr<Expr> rhs;
  while (accept({TC::AND, TC::OR})) {
    if (accept(TC::AND)) {
      expect(TC::AND);
      rhs = parseEqualExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::AND, rhs));
    }
    if (accept(TC::OR)) {
      expect(TC::OR);
      rhs = parseEqualExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::OR, rhs));
    }
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseEqualExpr() {
  atl::shared_ptr<Expr> lhs = parseCompExpr();
  atl::shared_ptr<Expr> rhs;
  while (accept({TC::EQ, TC::NE})) {
    if (accept(TC::EQ)) {
      expect(TC::EQ);
      rhs = parseCompExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::EQ, rhs));
    }
    if (accept(TC::NE)) {
      expect(TC::NE);
      rhs = parseCompExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::NE, rhs));
    }
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseCompExpr() {
  atl::shared_ptr<Expr> lhs = parseAddExpr();
  atl::shared_ptr<Expr> rhs;
  while (accept({TC::LE, TC::LT, TC::GE, TC::GT})) {
    if (accept(TC::LE)) {
      expect(TC::LE);
      rhs = parseAddExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::LE, rhs));
    }
    if (accept(TC::LT)) {
      expect(TC::LT);
      rhs = parseAddExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::LT, rhs));
    }
    if (accept(TC::GE)) {
      expect(TC::GE);
      rhs = parseAddExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::GE, rhs));
    }
    if (accept(TC::GT)) {
      expect(TC::GT);
      rhs = parseAddExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::GT, rhs));
    }
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseAddExpr() {
  atl::shared_ptr<Expr> lhs = parseMulExpr();
  atl::shared_ptr<Expr> rhs;
  while (accept({TC::MINUS, TC::PLUS})) {
    if (accept(TC::MINUS)) {
      expect(TC::MINUS);
      rhs = parseMulExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::SUB, rhs));
    }
    if (accept(TC::PLUS)) {
      expect(TC::PLUS);
      rhs = parseMulExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::ADD, rhs));
    }
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseMulExpr() {
  atl::shared_ptr<Expr> lhs = parseUnaryExpr();
  atl::shared_ptr<Expr> rhs;
  while (accept({TC::ASTERIX, TC::DIV, TC::REM})) {
    if (accept(TC::ASTERIX)) {
      expect(TC::ASTERIX);
      rhs = parseUnaryExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::MUL, rhs));
    }
    if (accept(TC::DIV)) {
      expect(TC::DIV);
      rhs = parseUnaryExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::DIV, rhs));
    }
    if (accept(TC::REM)) {
      expect(TC::REM);
      rhs = parseUnaryExpr();
      lhs = atl::make_shared<BinOp>(BinOp(lhs, Op::MOD, rhs));
    }
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
  if (accept(TC::LPAR) && (acceptType(1) && !accept(TC::IDENTIFIER, 1))) {
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
  if (accept({TC::PREFIXINC, TC::PREFIXDEC})) {
    TC operatorToken = expect({TC::PREFIXINC, TC::PREFIXDEC}).tokenClass;
    PrefixOp::Op operation;
    if (operatorToken == TC::PREFIXINC)
      operation = PrefixOp::Op::INC;
    if (operatorToken == TC::PREFIXDEC)
      operation = PrefixOp::Op::DEC;
    atl::shared_ptr<Expr> incrementExpr = parseObjExpr();
    if (incrementExpr->astClass() != "VarExpr")
      throw ACC::Error("Attempted operator++ on Expr that was not a VarExpr");
    const atl::shared_ptr<VarExpr> variable =
        atl::static_pointer_cast<VarExpr>(incrementExpr);
    return atl::make_shared<PrefixOp>(PrefixOp(operation, variable));
  }
  if (accept(TC::NEW)) {
    /* TODO: Parse Heap Allocation. */
    expect(TC::NEW);
    /* Constructor Call */
    if (accept(TC::IDENTIFIER) && accept(TC::LPAR, 1)) {
      atl::shared_ptr<Expr> funCall = parseObjExpr();
      if (funCall->astClass() != "FunCall")
        throw ACC::Error("Parsing: Expected FunCall", currToken.position);
      return atl::make_shared<Allocation>(
          Allocation(atl::static_pointer_cast<FunCall>(funCall)));
    } else {
      atl::shared_ptr<Type> allocatedType = parseType();
      if (accept(TC::LSBR)) {
        expect(TC::LSBR);
        const atl::shared_ptr<Expr> arraySize = parseExpr();
        expect(TC::RSBR);
        allocatedType =
            atl::shared_ptr<ArrayType>(new ArrayType(allocatedType, arraySize));
        return atl::make_shared<Allocation>(Allocation(allocatedType));
      }
    }
  }
  if (accept(TC::REF)) {
    expect(TC::REF);
    const atl::shared_ptr<Expr> addrOfExpr = parseObjExpr();
    return atl::make_shared<AddressOf>(AddressOf(addrOfExpr));
  }
  if (accept(TC::NOT)) {
    expect(TC::NOT);
    // Parse NOT Node.
  }

  return parseObjExpr();
}
atl::shared_ptr<Expr> Parser::parseObjExpr() {
  if (accept(TC::IDENTIFIER) && accept(TC::LPAR, 1)) {
    atl::shared_ptr<Expr> output = parseFunCall();
    while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
      if (accept({TC::DOT, TC::PTRDOT})) {
        expect({TC::DOT, TC::PTRDOT});
        if (accept(TC::LPAR, 1)) {
          atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
          output =
              atl::make_shared<MemberCall>(MemberCall(output, memberFunCall));
        } else {
          const atl::string ident_str = expect(TC::IDENTIFIER).data;
          const atl::shared_ptr<Identifier> fieldIdentifier(
              new Identifier(ident_str));
          output = atl::make_shared<MemberAccess>(
              MemberAccess(output, fieldIdentifier));
        }
      } else {
        expect(TC::LSBR);
        atl::shared_ptr<Expr> arrayIndexExpr = parseObjExpr();
        expect(TC::RSBR);
        output =
            atl::make_shared<ArrayAccess>(ArrayAccess(output, arrayIndexExpr));
      }
    }
    return output;
  }
  if (accept(TC::IDENTIFIER)) {
    const atl::shared_ptr<Identifier> ident = parseIdentifier();
    atl::shared_ptr<Expr> output = atl::make_shared<VarExpr>(VarExpr(ident));
    while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
      if (accept({TC::DOT, TC::PTRDOT})) {
        expect({TC::DOT, TC::PTRDOT});
        if (accept(TC::LPAR, 1)) {
          atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
          output =
              atl::make_shared<MemberCall>(MemberCall(output, memberFunCall));
        } else {
          const atl::string ident_str = expect(TC::IDENTIFIER).data;
          const atl::shared_ptr<Identifier> fieldIdentifier(
              new Identifier(ident_str));
          output = atl::make_shared<MemberAccess>(
              MemberAccess(output, fieldIdentifier));
        }
      } else {
        expect(TC::LSBR);
        atl::shared_ptr<Expr> arrayIndexExpr = parseObjExpr();
        expect(TC::RSBR);
        output =
            atl::make_shared<ArrayAccess>(ArrayAccess(output, arrayIndexExpr));
      }
    }
    return output;
  }
  if (accept(TC::THIS)) {
    expect(TC::THIS);
    atl::shared_ptr<Expr> output = atl::make_shared<VarExpr>(
        VarExpr(atl::make_shared<Identifier>(Identifier("this"))));
    while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
      if (accept({TC::DOT, TC::PTRDOT})) {
        expect({TC::DOT, TC::PTRDOT});
        if (accept(TC::LPAR, 1)) {
          atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
          output =
              atl::make_shared<MemberCall>(MemberCall(output, memberFunCall));
        } else {
          const atl::string ident_str = expect(TC::IDENTIFIER).data;
          const atl::shared_ptr<Identifier> fieldIdentifier(
              new Identifier(ident_str));
          output = atl::make_shared<MemberAccess>(
              MemberAccess(output, fieldIdentifier));
        }
      } else {
        expect(TC::LSBR);
        atl::shared_ptr<Expr> arrayIndexExpr = parseObjExpr();
        expect(TC::RSBR);
        output =
            atl::make_shared<ArrayAccess>(ArrayAccess(output, arrayIndexExpr));
      }
    }
    return output;
  }

  atl::shared_ptr<Expr> output = parseLitExpr();
  while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
    if (accept({TC::DOT, TC::PTRDOT})) {
      expect({TC::DOT, TC::PTRDOT});
      if (accept(TC::LPAR, 1)) {
        atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
        output =
            atl::make_shared<MemberCall>(MemberCall(output, memberFunCall));
      } else {
        const atl::string ident_str = expect(TC::IDENTIFIER).data;
        const atl::shared_ptr<Identifier> fieldIdentifier(
            new Identifier(ident_str));
        output = atl::make_shared<MemberAccess>(
            MemberAccess(output, fieldIdentifier));
      }
    } else {
      expect(TC::LSBR);
      atl::shared_ptr<Expr> arrayIndexExpr = parseObjExpr();
      expect(TC::RSBR);
      output =
          atl::make_shared<ArrayAccess>(ArrayAccess(output, arrayIndexExpr));
    }
  }
  return output;
}
atl::shared_ptr<FunCall> Parser::parseFunCall() {
  const atl::shared_ptr<Identifier> ident = parseIdentifier();
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
  if (accept(TC::TRUE_VAL)) {
    expect(TC::TRUE_VAL);
    return atl::make_shared<BoolLiteral>(BoolLiteral("true"));
  }
  if (accept(TC::FALSE_VAL)) {
    expect(TC::FALSE_VAL);
    return atl::make_shared<BoolLiteral>(BoolLiteral("false"));
  }
  if (accept(TC::LPAR) && (!acceptType(1) || accept(TC::IDENTIFIER, 1))) {
    expect(TC::LPAR);
    atl::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return atl::make_shared<ParenthExpr>(ParenthExpr(innerExpr));
  }
  if (acceptExpr())
    return parseExpr();

  throw ACC::Error("Parser: Expected a Literal Expression.",
                   currToken.position);
}

atl::string Parser::parseOperatorOverload() {
  return expect({TC::OPASSIGN, TC::OPAT, TC::OPADD, TC::OPADDTO, TC::OPEQ,
                 TC::OPGE, TC::OPGT, TC::OPLE, TC::OPLT, TC::OPNE})
      .data;
}
atl::shared_ptr<VarDecl> Parser::parseParam() {
  atl::shared_ptr<Type> varType = parseType();
  atl::shared_ptr<Identifier> varIdentifier;
  if (accept(TC::IDENTIFIER))
    varIdentifier = parseIdentifier();
  if (accept(TC::LSBR)) {
    expect(TC::LSBR);
    const atl::shared_ptr<Expr> arraySize = parseLitExpr();
    expect(TC::RSBR);
    varType = atl::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
  }
  return atl::make_shared<VarDecl>(VarDecl(varType, varIdentifier));
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
  case TC::BOOL:
    return atl::make_shared<BaseType>(BaseType(PrimitiveType::BOOL));
  default:
    throw ACC::Error("Parser: Cannot resolve Token " + tokToStr(tc) +
                         "  to a type.",
                     currToken.position);
  }
}