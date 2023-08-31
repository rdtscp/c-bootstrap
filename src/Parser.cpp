#include "atl/include/set.h"

#include "Error.h"
#include "Parser.h"

using namespace ACC;
using TC = SourceToken::Class;

Parser::Parser(Lexer &lexer)
    : prevPosition(Position()), currToken(TC::INVALID, Position(-1, -1, "")),
      lexer(lexer) {}

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
  for (unsigned int idx = 0; idx < expected.size(); ++idx) {
    if (accept(expected[idx], offset))
      return true;
  }
  return false;
}

SourceToken Parser::expect(TC expected) {
  if (expected == currToken.tokenClass) {
    SourceToken output = currToken;
    prevPosition = currToken.position;
    nextToken();
    return output;
  }
  throw ACC::Error("Parser: Expected Token " + ACC::tokToStr(expected) +
                       " but found " + ACC::tokToStr(currToken),
                   currToken.position);
}

SourceToken Parser::expect(atl::vector<TC> expected) {
  for (unsigned int idx = 0; idx < expected.size(); ++idx) {
    if (expected[idx] == currToken.tokenClass) {
      SourceToken output = currToken;
      prevPosition = currToken.position;
      nextToken();
      return output;
    }
  }
  throw ACC::Error("Parser: Unexpected Token " + ACC::tokToStr(currToken),
                   currToken.position);
}

SourceToken Parser::lookAhead(unsigned int i) {
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
  return accept(TC::PUBLIC, offset) || accept(TC::PRIVATE, offset) ||
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
  if (acceptTemplateDef(offset))
    return true;

  if (acceptClassTypeDecl(offset))
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

  if (acceptType(offset)) {
    if (accept(TC::CONST))
      ++offset;
    ++offset;

    while (accept(TC::NAMESPACEACCESS, offset) &&
           accept(TC::IDENTIFIER, offset + 1))
      offset += 2;

    if (accept(TC::ASTERIX, offset) && !accept(TC::REF, offset))
      while (accept(TC::ASTERIX, offset))
        ++offset;

    else if (accept(TC::REF, offset)) {
      ++offset;
    } else if (accept(TC::AND, offset)) {
      ++offset;
      ++offset;
    }

    // while (accept(TC::NAMESPACEACCESS, offset) &&
    //        accept(TC::IDENTIFIER, offset + 1)) {
    //   offset = offset + 2;
    // }

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
bool Parser::acceptTemplateDef(int offset) { return accept(TC::TEMPLATE); }
bool Parser::acceptTypeDefDecl(int offset) { return accept(TC::TYPEDEF); }
bool Parser::acceptVarDecl(int offset) {
  if (accept(TC::STATIC))
    ++offset;

  if (!acceptType(offset))
    return false;

  if (accept(TC::CONST))
    ++offset;

  if (accept({TC::INT, TC::CHAR, TC::SHORT, TC::VOID, TC::UINT, TC::BOOL},
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
bool Parser::acceptType(int offset) {
  if (accept(TC::CONST, offset))
    ++offset;

  if (accept({TC::INT, TC::CHAR, TC::VOID, TC::UINT, TC::BOOL}, offset))
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
                 TC::THIS, TC::STATIC_CAST},
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
bool Parser::acceptFunCall(int offset) {
  // Check for namespaced function.
  while (accept(TC::IDENTIFIER, offset)) {
    ++offset;
    if (accept(TC::NAMESPACEACCESS, offset)) {
      ++offset;
    }
  }
  // Check for template params.
  if (accept(TC::LT, offset)) {
    ++offset;
    while (acceptType(offset)) {
      ++offset;
      if (accept(TC::COMMA, offset)) {
        ++offset;
      }
    }
    if (accept(TC::GT, offset)) {
      ++offset;
    }
  }
  // Check for parenthesis.
  return accept(TC::LPAR, offset);
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
  return createNode(new Program(decls));
}

atl::shared_ptr<Identifier> Parser::parseIdentifier() {

  if (acceptOperatorOverload()) {
    return createNode(new Identifier(parseOperatorOverload()));
  }
  atl::shared_ptr<Identifier> identifier =
      createNode(new Identifier(expect(TC::IDENTIFIER).data));
  while (accept(TC::NAMESPACEACCESS)) {
    expect(TC::NAMESPACEACCESS);
    if (acceptOperatorOverload()) {
      identifier->insert(createNode(new Identifier(parseOperatorOverload())));
      break;
    }
    const atl::string identData = expect(TC::IDENTIFIER).data;
    identifier->insert(createNode(new Identifier(identData)));
  }
  return identifier;
}

/* -- Decls -- */
atl::shared_ptr<ClassTypeDecl> Parser::parseClassTypeDecl() {
  expect(TC::CLASS);
  const atl::shared_ptr<Identifier> classIdentifier = parseIdentifier();
  if (accept(TC::SC)) {
    return createNode(new ClassTypeDecl(classIdentifier));
  }

  // TODO: Parse Inheritance

  expect(TC::LBRA);

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
  return createNode(new ClassTypeDef(classIdentifier, classDecls));
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
      // Create the `this` member.
      const atl::shared_ptr<VarExpr> initialiserMember =
          createNode(new VarExpr(parseIdentifier()));
      expect(TC::LPAR);
      const atl::shared_ptr<Expr> initialiseValue = parseExpr();
      expect(TC::RPAR);
      const atl::shared_ptr<Assign> intialiserStmt =
          createNode(new Assign(initialiserMember, initialiseValue));
      initialiserList.push_back(intialiserStmt);
    }
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      const atl::shared_ptr<VarExpr> initialiserMember =
          createNode(new VarExpr(parseIdentifier()));
      expect(TC::LPAR);
      const atl::shared_ptr<Expr> initialiseValue = parseExpr();
      expect(TC::RPAR);
      const atl::shared_ptr<Assign> intialiserStmt =
          createNode(new Assign(initialiserMember, initialiseValue));
      initialiserList.push_back(intialiserStmt);
    }
    atl::shared_ptr<Block> constructorBlock = parseBlock();
    atl::shared_ptr<ConstructorDef> cd = createNode(new ConstructorDef(
        classType, constructorParams, initialiserList, constructorBlock));
    return cd;
  } else {
    expect(TC::SC);
    atl::shared_ptr<ConstructorDecl> cd =
        createNode(new ConstructorDecl(classType, constructorParams));
    return cd;
  }
}
atl::shared_ptr<Decl> Parser::parseDecl() {
  if (acceptTemplateDef()) {
    atl::shared_ptr<TemplateDef> td = parseTemplateDef();
    return td;
  }

  if (acceptClassTypeDecl()) {
    atl::shared_ptr<ClassTypeDecl> ctd = parseClassTypeDecl();
    expect(TC::SC);
    return ctd;
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

  throw ACC::Error("Parser: Expected a Class/Variable/Function Declaration "
                   "but none was found.",
                   currToken.position);
}
atl::shared_ptr<Deletion> Parser::parseDeletion() {
  TC deletionToken = expect({TC::DELETE, TC::DELETEARR}).tokenClass;
  Deletion::DelType deletionType = Deletion::DelType::OBJECT;
  if (deletionToken == TC::DELETEARR)
    deletionType = Deletion::DelType::ARRAY;

  const atl::shared_ptr<VarExpr> deletionVar = parseObjExpr();
  return createNode(new Deletion(deletionType, deletionVar));
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
  return createNode(new EnumClassTypeDecl(enumIdentifier, states));
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
    funIdentifier = createNode(new Identifier(identifier));
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

  if (acceptBlock() && !isDef) {
    throw ACC::Error(
        "Parser: FunDef signature did not provide names for parameters.",
        currToken.position);
  } else if (acceptBlock()) {
    atl::shared_ptr<Block> funBlock = parseBlock();
    return createNode(
        new FunDef(funModifiers, funIdentifier, funParams, funType, funBlock));
  } else {
    expect(TC::SC);
    return createNode(
        new FunDecl(funModifiers, funIdentifier, funParams, funType));
  }
}
atl::shared_ptr<TemplateDef> Parser::parseTemplateDef() {
  expect(TC::TEMPLATE);
  expect(TC::LT);
  expect(TC::TYPENAME);
  atl::vector<atl::shared_ptr<Identifier>> templateParams;
  templateParams.push_back(parseIdentifier());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    expect(TC::TYPENAME);
    templateParams.push_back(parseIdentifier());
  }
  expect(TC::GT);
  atl::shared_ptr<Decl> templatedDecl;
  if (acceptClassTypeDecl()) {
    templatedDecl = parseClassTypeDecl();
  } else {
    templatedDecl = parseFunDecl();
  }
  return createNode(new TemplateDef(templateParams, templatedDecl));
}
atl::shared_ptr<TypeDefDecl> Parser::parseTypeDefDecl() {
  expect(TC::TYPEDEF);
  atl::shared_ptr<Type> aliasedType = parseType();
  const atl::shared_ptr<Identifier> typeAlias = parseIdentifier();
  return createNode(new TypeDefDecl(aliasedType, typeAlias));
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
    atl::shared_ptr<VarDef> output =
        createNode(new VarDef(varType, varIdentifier, assignExpr));
    return output;
  } else if (accept(TC::LPAR)) {
    expect(TC::LPAR);
    if (varType->astClass() != "ClassType")
      throw ACC::Error(
          "Parser: Construction of Variable of non-class type not supported.",
          currToken.position);

    atl::shared_ptr<ClassType> classType =
        atl::static_pointer_cast<ClassType>(varType);
    /* Parse Params */
    atl::vector<atl::shared_ptr<Expr>> params;
    if (acceptExpr())
      params.push_back(parseExpr());
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      params.push_back(parseExpr());
    }
    expect(TC::RPAR);
    const atl::shared_ptr<ConstructorCall> constructorCall =
        createNode(new ConstructorCall(classType->identifier, params));
    atl::shared_ptr<VarDef> output =
        createNode(new VarDef(varType, varIdentifier, constructorCall));
    constructorCall->objectToConstruct = output;
    return output;
  } else {
    return createNode(new VarDecl(varType, varIdentifier));
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
  if (acceptClassType()) {
    const atl::shared_ptr<Identifier> classIdentifier = parseIdentifier();
    type = createNode(new ClassType(classIdentifier));
    type->typeModifiers = typeModifiers;
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
    type = createNode(new BaseType(pType));
  }
  if (accept(TC::ASTERIX)) {
    while (accept(TC::ASTERIX) || accept(TC::REF)) {
      if (accept(TC::ASTERIX)) {
        expect(TC::ASTERIX);
        type = createNode(new PointerType(type));
        if (accept(TC::CONST)) {
          expect(TC::CONST);
          type->typeModifiers.insert(Type::Modifiers::CONST);
        }
      } else if (accept(TC::REF)) {
        /*  We can have:
         *    void foo(char *& ptr);
         */
        expect(TC::REF);
        type = createNode(new ReferenceType(type));
        if (accept(TC::CONST)) {
          expect(TC::CONST);
          type->typeModifiers.insert(Type::Modifiers::CONST);
        }
        break;
      }
    }
  } else if (accept(TC::REF)) {
    expect(TC::REF);
    type = createNode(new ReferenceType(type));
    if (accept(TC::CONST)) {
      expect(TC::CONST);
      type->typeModifiers.insert(Type::Modifiers::CONST);
    }
  } else if (accept(TC::AND)) {
    expect(TC::AND);
    type = createNode(new ReferenceType(type));
    type = createNode(new ReferenceType(type));
    if (accept(TC::CONST)) {
      expect(TC::CONST);
      type->typeModifiers.insert(Type::Modifiers::CONST);
    }
  }

  return type;
}

/* -- Stmts -- */
atl::shared_ptr<Assign> Parser::parseAssign() {
  atl::shared_ptr<Expr> lhs = parseExpr();
  expect(TC::ASSIGN);
  atl::shared_ptr<Expr> rhs = parseExpr();
  return createNode(new Assign(lhs, rhs));
}
atl::shared_ptr<Block> Parser::parseBlock() {
  expect(TC::LBRA);

  atl::vector<atl::shared_ptr<Stmt>> blockStmts;

  while (acceptStmt()) {
    blockStmts.push_back(parseStmt());
  }

  expect(TC::RBRA);
  return createNode(new Block(blockStmts));
}
atl::shared_ptr<DoWhile> Parser::parseDoWhile() {
  expect(TC::DO);
  atl::shared_ptr<Stmt> whileBody = parseStmt();
  expect(TC::WHILE);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  expect(TC::SC);

  return createNode(new DoWhile(whileBody, whileCondition));
}
atl::shared_ptr<For> Parser::parseFor() {
  expect(TC::FOR);
  expect(TC::LPAR);
  const atl::shared_ptr<VarDecl> initialVarDecl = parseVarDecl();
  expect(TC::SC);
  const atl::shared_ptr<Expr> condition = parseExpr();
  expect(TC::SC);
  const atl::shared_ptr<Expr> endBodyExpr = parseExpr();
  if (endBodyExpr->astClass() != "PrefixOp")
    throw ACC::Error("Parser TEMP: For Loops do not support end of body "
                     "expressions other than "
                     "prefix operations",
                     currToken.position);
  expect(TC::RPAR);
  const atl::shared_ptr<Stmt> body = parseStmt();
  return createNode(new For(initialVarDecl, condition, endBodyExpr, body));
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
    return createNode(new If(ifCondition, ifBody, elseBody));
  } else {
    return createNode(new If(ifCondition, ifBody));
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
  return createNode(new Namespace(namespaceIdent, namespaceDecls));
}
atl::shared_ptr<Return> Parser::parseReturn() {
  expect(TC::RETURN);
  if (acceptExpr()) {
    atl::shared_ptr<Expr> returnExpr = parseExpr();
    expect(TC::SC);
    return createNode(new Return(returnExpr));
  } else {
    expect(TC::SC);
    return createNode(new Return());
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
    return createNode(new Assign(expr, rhs));
  }

  expect(TC::SC);
  return expr;
}
atl::shared_ptr<Throw> Parser::parseThrow() {
  expect(TC::THROW);
  if (accept(TC::STRING_LITERAL)) {
    const atl::shared_ptr<StringLiteral> stringLiteral = parseStringLiteral();
    expect(TC::SC);
    return createNode(new Throw(stringLiteral));
  } else {
    expect(TC::SC);
    return createNode(new Throw());
  }
}
atl::shared_ptr<While> Parser::parseWhile() {
  expect(TC::WHILE);
  expect(TC::LPAR);
  atl::shared_ptr<Expr> whileCondition = parseExpr();
  expect(TC::RPAR);
  atl::shared_ptr<Stmt> whileBody = parseStmt();

  return createNode(new While(whileBody, whileCondition));
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
      output = createNode(new TertiaryExpr(tertiaryCondition, tertiaryIfBody,
                                           tertiaryElseBody));
    } else {
      const atl::shared_ptr<Expr> &lhs = output;
      expect(TC::ASSIGNADD);
      const atl::shared_ptr<Expr> rhs = parseBoolExpr();
      output = createNode(new BinOp(lhs, Op::ASSIGNADD, rhs));
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
      lhs = createNode(new BinOp(lhs, Op::AND, rhs));
    }
    if (accept(TC::OR)) {
      expect(TC::OR);
      rhs = parseEqualExpr();
      lhs = createNode(new BinOp(lhs, Op::OR, rhs));
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
      lhs = createNode(new BinOp(lhs, Op::EQ, rhs));
    }
    if (accept(TC::NE)) {
      expect(TC::NE);
      rhs = parseCompExpr();
      lhs = createNode(new BinOp(lhs, Op::NE, rhs));
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
      lhs = createNode(new BinOp(lhs, Op::LE, rhs));
    }
    if (accept(TC::LT)) {
      expect(TC::LT);
      rhs = parseAddExpr();
      lhs = createNode(new BinOp(lhs, Op::LT, rhs));
    }
    if (accept(TC::GE)) {
      expect(TC::GE);
      rhs = parseAddExpr();
      lhs = createNode(new BinOp(lhs, Op::GE, rhs));
    }
    if (accept(TC::GT)) {
      expect(TC::GT);
      rhs = parseAddExpr();
      lhs = createNode(new BinOp(lhs, Op::GT, rhs));
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
      lhs = createNode(new BinOp(lhs, Op::SUB, rhs));
    }
    if (accept(TC::PLUS)) {
      expect(TC::PLUS);
      rhs = parseMulExpr();
      lhs = createNode(new BinOp(lhs, Op::ADD, rhs));
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
      lhs = createNode(new BinOp(lhs, Op::MUL, rhs));
    }
    if (accept(TC::DIV)) {
      expect(TC::DIV);
      rhs = parseUnaryExpr();
      lhs = createNode(new BinOp(lhs, Op::DIV, rhs));
    }
    if (accept(TC::REM)) {
      expect(TC::REM);
      rhs = parseUnaryExpr();
      lhs = createNode(new BinOp(lhs, Op::MOD, rhs));
    }
  }
  return lhs;
}
atl::shared_ptr<Expr> Parser::parseUnaryExpr() {
  if (accept(TC::LPAR)) {
    expect(TC::LPAR);
    const atl::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return createNode(new ParenthExpr(innerExpr));
  }
  if (accept(TC::SIZEOF)) {
    expect(TC::SIZEOF);
    expect(TC::LPAR);
    atl::shared_ptr<Type> type = parseType();
    expect(TC::RPAR);
    return createNode(new SizeOf(type));
  }
  if (accept(TC::ASTERIX)) {
    expect(TC::ASTERIX);
    atl::shared_ptr<Expr> rhs = parseObjExpr();
    return createNode(new ValueAt(rhs));
  }
  if (accept(TC::MINUS)) {
    expect(TC::MINUS);
    atl::shared_ptr<IntLiteral> lhs(new IntLiteral("0"));
    atl::shared_ptr<Expr> rhs = parseObjExpr();
    return createNode(new BinOp(lhs, Op::SUB, rhs));
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
      throw ACC::Error(
          "Parser: Attempted operator++ on Expr that was not a VarExpr",
          currToken.position);
    const atl::shared_ptr<VarExpr> variable =
        atl::static_pointer_cast<VarExpr>(incrementExpr);
    return createNode(new PrefixOp(operation, variable));
  }
  if (accept(TC::NEW)) {
    expect(TC::NEW);
    /* Constructor Call */
    if (accept(TC::IDENTIFIER) && accept(TC::LPAR, 1)) {
      atl::shared_ptr<Expr> funCall = parseObjExpr();
      if (funCall->astClass() != "FunCall")
        throw ACC::Error("Parser: Expected FunCall", currToken.position);
      return createNode(
          new Allocation(atl::static_pointer_cast<FunCall>(funCall)));
    } else {
      atl::shared_ptr<Type> allocatedType = parseType();
      expect(TC::LSBR);
      const atl::shared_ptr<Expr> arraySize = parseExpr();
      expect(TC::RSBR);
      allocatedType =
          atl::shared_ptr<ArrayType>(new ArrayType(allocatedType, arraySize));
      return createNode(new Allocation(allocatedType));
    }
  }
  if (accept(TC::REF)) {
    expect(TC::REF);
    const atl::shared_ptr<Expr> addrOfExpr = parseObjExpr();
    return createNode(new AddressOf(addrOfExpr));
  }
  if (accept(TC::NOT)) {
    expect(TC::NOT);
    const atl::shared_ptr<Expr> notExpr = parseObjExpr();
    return createNode(new Not(notExpr));
  }
  if (accept(TC::STATIC_CAST)) {
    expect(TC::STATIC_CAST);
    expect(TC::LT);
    const atl::shared_ptr<Type> castType = parseType();
    expect(TC::GT);
    expect(TC::LPAR);
    const atl::shared_ptr<Expr> castExpr = parseExpr();
    expect(TC::RPAR);
    return createNode(new StaticCast(castType, castExpr));
  }

  atl::shared_ptr<Expr> objExpr = parseObjExpr();

  // if (accept(TC::LSBR)) {
  //   expect(TC::LSBR);
  // }

  return objExpr;
}
atl::shared_ptr<Expr> Parser::parseObjExpr() {
  if (accept(TC::LPAR)) {
    expect(TC::LPAR);
    const atl::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return createNode(new ParenthExpr(innerExpr));
  }
  if (accept(TC::IDENTIFIER)) {
    // Parsing a FunCall
    atl::shared_ptr<Expr> objExpr;
    if (acceptFunCall()) {
      objExpr = parseFunCall();
    } else {
      const atl::shared_ptr<Identifier> ident = parseIdentifier();
      objExpr = createNode(new VarExpr(ident));
    }

    while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
      if (accept({TC::DOT, TC::PTRDOT})) {
        const SourceToken::Class accessType =
            expect({TC::DOT, TC::PTRDOT}).tokenClass;
        if (accept(TC::LPAR, 1)) {
          atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
          objExpr =
              createNode(new MemberCall(objExpr, memberFunCall, accessType));
        } else {
          const atl::shared_ptr<Identifier> fieldIdentifier = parseIdentifier();
          const atl::shared_ptr<VarExpr> fieldVarExpr =
              createNode(new VarExpr(fieldIdentifier));
          objExpr =
              createNode(new MemberAccess(objExpr, fieldVarExpr, accessType));
        }
      } else {
        expect(TC::LSBR);
        atl::shared_ptr<Expr> indexExpr = parseObjExpr();
        expect(TC::RSBR);
        objExpr = createNode(new SubscriptOp(objExpr, indexExpr));
      }
    }
    return objExpr;
  }
  if (accept(TC::THIS)) {
    expect(TC::THIS);
    atl::shared_ptr<Expr> output =
        createNode(new VarExpr(createNode(new Identifier("this"))));
    while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
      if (accept({TC::DOT, TC::PTRDOT})) {
        const SourceToken::Class accessType =
            expect({TC::DOT, TC::PTRDOT}).tokenClass;
        if (accept(TC::LPAR, 1)) {
          atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
          output =
              createNode(new MemberCall(output, memberFunCall, accessType));
        } else {
          const atl::shared_ptr<Identifier> fieldIdentifier = parseIdentifier();
          const atl::shared_ptr<VarExpr> fieldVarExpr =
              createNode(new VarExpr(fieldIdentifier));
          output =
              createNode(new MemberAccess(output, fieldVarExpr, accessType));
        }
      } else {
        expect(TC::LSBR);
        atl::shared_ptr<Expr> indexExpr = parseObjExpr();
        expect(TC::RSBR);
        output = createNode(new SubscriptOp(output, indexExpr));
      }
    }
    return output;
  }

  atl::shared_ptr<Expr> output = parseLitExpr();
  while (accept({TC::DOT, TC::PTRDOT, TC::LSBR})) {
    if (accept({TC::DOT, TC::PTRDOT})) {
      const SourceToken::Class accessType =
          expect({TC::DOT, TC::PTRDOT}).tokenClass;
      if (accept(TC::LPAR, 1)) {
        atl::shared_ptr<FunCall> memberFunCall = parseFunCall();
        output = createNode(new MemberCall(output, memberFunCall, accessType));
      } else {
        const atl::shared_ptr<Identifier> fieldIdentifier = parseIdentifier();
        const atl::shared_ptr<VarExpr> fieldVarExpr =
            createNode(new VarExpr(fieldIdentifier));
        output = createNode(new MemberAccess(output, fieldVarExpr, accessType));
      }
    } else {
      expect(TC::LSBR);
      atl::shared_ptr<Expr> indexExpr = parseObjExpr();
      expect(TC::RSBR);
      output = createNode(new SubscriptOp(output, indexExpr));
    }
  }
  return output;
}
atl::shared_ptr<FunCall> Parser::parseFunCall() {
  const atl::shared_ptr<Identifier> ident = parseIdentifier();
  // Templated Function Call
  if (accept(TC::LT)) {
    expect(TC::LT);
    atl::vector<atl::shared_ptr<Type>> templateArgs;
    templateArgs.push_back(parseType());
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      templateArgs.push_back(parseType());
    }
    expect(TC::GT);
    expect(TC::LPAR);
    atl::vector<atl::shared_ptr<Expr>> params;
    if (acceptExpr())
      params.push_back(parseExpr());
    while (accept(TC::COMMA)) {
      expect(TC::COMMA);
      params.push_back(parseExpr());
    }
    expect(TC::RPAR);
    return createNode(new TemplatedFunCall(ident, params, templateArgs));
  }
  expect(TC::LPAR);
  atl::vector<atl::shared_ptr<Expr>> params;
  if (acceptExpr())
    params.push_back(parseExpr());
  while (accept(TC::COMMA)) {
    expect(TC::COMMA);
    params.push_back(parseExpr());
  }

  expect(TC::RPAR);
  return createNode(new FunCall(ident, params));
}
atl::shared_ptr<Expr> Parser::parseLitExpr() {
  if (accept(TC::LPAR)) {
    expect(TC::LPAR);
    const atl::shared_ptr<Expr> innerExpr = parseExpr();
    expect(TC::RPAR);
    return createNode(new ParenthExpr(innerExpr));
  }
  if (accept(TC::INT_LITERAL)) {
    return createNode(new IntLiteral(expect(TC::INT_LITERAL).data));
  }
  if (accept(TC::CHAR_LITERAL)) {
    return createNode(new CharLiteral(expect(TC::CHAR_LITERAL).data));
  }
  if (accept(TC::STRING_LITERAL)) {
    return parseStringLiteral();
  }
  if (accept(TC::TRUE_VAL)) {
    expect(TC::TRUE_VAL);
    return createNode(new BoolLiteral("true"));
  }
  if (accept(TC::FALSE_VAL)) {
    expect(TC::FALSE_VAL);
    return createNode(new BoolLiteral("false"));
  }
  if (accept(TC::NULLPTR)) {
    expect(TC::NULLPTR);
    return createNode(new Nullptr());
  }
  // if (acceptExpr())
  //   return parseExpr();

  throw ACC::Error("Parser: Expected a Literal Expression.",
                   currToken.position);
}

atl::shared_ptr<StringLiteral> Parser::parseStringLiteral() {
  return createNode(new StringLiteral(expect(TC::STRING_LITERAL).data));
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
  return createNode(new VarDecl(varType, varIdentifier));
}

/* ---- Helpers ---- */

atl::shared_ptr<BaseType> Parser::tokenToType(const TC &tc) {
  switch (tc) {
  case TC::INT:
    return createNode(new BaseType(PrimitiveType::INT));
  case TC::CHAR:
    return createNode(new BaseType(PrimitiveType::CHAR));
  case TC::SHORT:
    return createNode(new BaseType(PrimitiveType::SHORT));
  case TC::VOID:
    return createNode(new BaseType(PrimitiveType::VOID));
  case TC::BOOL:
    return createNode(new BaseType(PrimitiveType::BOOL));
  default:
    throw ACC::Error("Parser: Cannot resolve Token " + tokToStr(tc) +
                         "  to a type.",
                     currToken.position);
  }
}