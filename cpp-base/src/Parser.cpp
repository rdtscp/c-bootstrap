
#include <cassert>
#include <stdexcept>

#include "../include/Parser.h"

/* Public */

using namespace ACC;

Parser::Parser(const Lexer &lexer)
    : currToken(Token::TokenClass::INVALID, -1, -1), lexer(lexer) {}

Program Parser::parse() {
  nextToken();
  return parseProgram();
}

/* Token Operations */

bool Parser::accept(Token::TokenClass expected) {
  return expected == currToken.tokenClass;
}

bool Parser::accept(std::vector<Token::TokenClass> expected) {
  bool output = false;
  for (const Token::TokenClass token : expected) {
    bool curr = accept(token);
    output |= curr;
  }
  return output;
}

Token Parser::expect(Token::TokenClass expected) {
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

Token Parser::expect(std::vector<Token::TokenClass> expected) {
  for (Token::TokenClass token : expected) {
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

/* AST Construction */

std::shared_ptr<Type> Parser::expectType() {
  if (accept({Token::TokenClass::INT, Token::TokenClass::CHAR,
              Token::TokenClass::VOID})) {
    Token typeToken = expect({Token::TokenClass::INT, Token::TokenClass::CHAR,
                              Token::TokenClass::VOID});
    std::shared_ptr<Type> type = tokenToType(typeToken.tokenClass);
    while (accept(Token::TokenClass::ASTERIX)) {
      expect(Token::TokenClass::ASTERIX);
      type = std::shared_ptr<Type>(new PointerType(type));
    }
    return type;
  } else if (accept(Token::TokenClass::STRUCT)) {
    expect(Token::TokenClass::STRUCT);
    Token structIdentToken = expect(Token::TokenClass::IDENTIFIER);
    std::shared_ptr<Type> type(new StructType(structIdentToken.data));
    if (accept(Token::TokenClass::ASTERIX)) {
      expect(Token::TokenClass::ASTERIX);
      type = std::shared_ptr<Type>(new PointerType(type));
    }
    return type;
  } else {
    throw std::runtime_error("Parsing: Expected a Type at " +
                             currToken.position.toString());
  }
}

// Expects params
// params  -> [ type IDENT (COMMA type IDENT)* ]
std::vector<VarDecl> Parser::expectParams() {
  std::vector<VarDecl> output;
  // Check for vardecl && fundecl.
  Token::TokenClass twoAhead = lookAhead(2).tokenClass;
  Token::TokenClass threeAhead = lookAhead(3).tokenClass;
  Token::TokenClass fourAhead = lookAhead(4).tokenClass;
  if (twoAhead != Token::TokenClass::RPAR &&
      twoAhead != Token::TokenClass::COMMA &&
      threeAhead != Token::TokenClass::RPAR &&
      threeAhead != Token::TokenClass::COMMA &&
      fourAhead != Token::TokenClass::RPAR &&
      fourAhead != Token::TokenClass::COMMA) {
    return output;
  }
  if (accept({Token::TokenClass::STRUCT, Token::TokenClass::INT,
              Token::TokenClass::CHAR, Token::TokenClass::VOID})) {
    if (accept(Token::TokenClass::STRUCT)) {
      expect(Token::TokenClass::STRUCT);
      std::shared_ptr<Type> paramType = std::shared_ptr<StructType>(
          new StructType(expect(Token::TokenClass::IDENTIFIER).data));

      while (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        paramType = std::shared_ptr<PointerType>(new PointerType(paramType));
      }
      std::string paramName = expect(Token::TokenClass::IDENTIFIER).data;

      output.push_back(VarDecl(paramType, paramName));
    } else if (accept({Token::TokenClass::INT, Token::TokenClass::CHAR,
                       Token::TokenClass::VOID})) {
      std::shared_ptr<Type> paramType =
          tokenToType(expect({Token::TokenClass::INT, Token::TokenClass::CHAR,
                              Token::TokenClass::VOID})
                          .tokenClass);
      if (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        paramType = std::shared_ptr<PointerType>(new PointerType(paramType));
      }
      std::string paramName = expect(Token::TokenClass::IDENTIFIER).data;

      output.push_back(VarDecl(paramType, paramName));
    }

    while (accept(Token::TokenClass::COMMA)) {
      expect(Token::TokenClass::COMMA);
      std::shared_ptr<Type> paramType = expectType();
      std::string paramName = expect(Token::TokenClass::IDENTIFIER).data;

      output.push_back(VarDecl(paramType, paramName));
    }
  }
  return output;
}

VarDecl Parser::expectVarDecl() {
  std::shared_ptr<Type> type = expectType();
  std::string varName = expect(Token::TokenClass::IDENTIFIER).data;
  if (accept(Token::TokenClass::LSBR)) {
    expect(Token::TokenClass::LSBR);
    std::string arraySize = expect(Token::TokenClass::INT_LITERAL).data;
    expect(Token::TokenClass::RSBR);
    type = std::shared_ptr<ArrayType>(new ArrayType(type, arraySize));
  }
  expect(Token::TokenClass::SC);
  return VarDecl(type, varName);
}

Program Parser::parseProgram() {
  parseIncludes();
  std::vector<StructTypeDecl> stds = parseStructDecls();
  std::vector<VarDecl> vds = parseVarDecls();
  std::vector<FunDecl> fds = parseFunDecls();
  expect(Token::TokenClass::ENDOFFILE);
  return Program(fds, stds, vds);
}

std::vector<FunDecl> Parser::parseFunDecls() {
  std::vector<FunDecl> output;

  // Check if this will be an invalid fundecl.
  Token::TokenClass twoAhead = lookAhead(2).tokenClass;
  Token::TokenClass threeAhead = lookAhead(3).tokenClass;
  Token::TokenClass fourAhead = lookAhead(4).tokenClass;
  if (twoAhead != Token::TokenClass::LPAR &&
      threeAhead != Token::TokenClass::LPAR &&
      fourAhead != Token::TokenClass::LPAR) {
    return output;
  }

  if (accept({Token::TokenClass::STRUCT, Token::TokenClass::INT,
              Token::TokenClass::CHAR, Token::TokenClass::VOID})) {
    std::shared_ptr<Type> funType;
    std::string funName;
    std::vector<VarDecl> funArgs;

    Block funBlock;
    // std::vector<VarDecl> blockVars;
    // std::vector<Stmt> blockStmts;

    if (accept(Token::TokenClass::STRUCT)) {
      expect(Token::TokenClass::STRUCT);
      funType = std::shared_ptr<StructType>(
          new StructType(expect(Token::TokenClass::IDENTIFIER).data));
      while (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        funType = std::shared_ptr<PointerType>(new PointerType(funType));
      }
      funName = expect(Token::TokenClass::IDENTIFIER).data;
      expect(Token::TokenClass::LPAR);
      funArgs = expectParams();
      expect(Token::TokenClass::RPAR);
      expect(Token::TokenClass::LBRA);

      // std::vector<VarDecl> blockVars = parseVarDecls();
      // blockStmts = parseStmts();
      // funBlock = new Block(blockVars, blockStmts);

      expect(Token::TokenClass::RBRA);

      output.push_back(FunDecl(funType, funName, funArgs, funBlock));
    } else if (accept({Token::TokenClass::INT, Token::TokenClass::CHAR,
                       Token::TokenClass::VOID})) {
      funType =
          tokenToType(expect({Token::TokenClass::INT, Token::TokenClass::CHAR,
                              Token::TokenClass::VOID})
                          .tokenClass);
      while (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        funType = std::shared_ptr<PointerType>(new PointerType(funType));
      }
      funName = expect(Token::TokenClass::IDENTIFIER).data;
      expect(Token::TokenClass::LPAR);
      funArgs = expectParams();
      expect(Token::TokenClass::RPAR);
      expect(Token::TokenClass::LBRA);

      blockVars = parseVarDecls();
      // blockStmts = parseStmts();
      funBlock = Block(); // new Block(blockVars, blockStmts);

      expect(Token::TokenClass::RBRA);

      output.push_back(FunDecl(funType, funName, funArgs, funBlock));
    }

    // Try to parse more fundecl
    std::vector<FunDecl> moreFunDecls = parseFunDecls();
    output.insert(output.end(), moreFunDecls.begin(), moreFunDecls.end());
  }

  return output;
}

std::vector<StructTypeDecl> Parser::parseStructDecls() {
  std::vector<StructTypeDecl> output;
  // Check for struct being used as a vardecl.
  if (lookAhead(2).tokenClass != Token::TokenClass::LBRA)
    return output;

  // Parse the struct.
  if (accept(Token::TokenClass::STRUCT)) {

    // Define the struct info.
    std::vector<VarDecl> varDecls;

    expect(Token::TokenClass::STRUCT);
    std::string structName = expect(Token::TokenClass::IDENTIFIER).data;
    expect(Token::TokenClass::LBRA);

    // Add all VarDecl's to our varDecl List.
    varDecls.push_back(expectVarDecl());

    std::vector<VarDecl> moreVarDecls = parseVarDecls();
    varDecls.insert(varDecls.end(), moreVarDecls.begin(), moreVarDecls.end());

    expect(Token::TokenClass::RBRA);
    expect(Token::TokenClass::SC);

    // Add this StructDecl to our output.
    output.push_back(StructTypeDecl(
        std::shared_ptr<StructType>(new StructType(structName)), varDecls));

    // Try to parse more StructDecl's to add to our output.
    std::vector<StructTypeDecl> moreStructTypes = parseStructDecls();
    output.insert(output.end(), moreStructTypes.begin(), moreStructTypes.end());
  }
  return output;
}

std::vector<VarDecl> Parser::parseVarDecls() {
  std::vector<VarDecl> output;
  Token::TokenClass twoAhead = lookAhead(2).tokenClass;
  Token::TokenClass threeAhead = lookAhead(3).tokenClass;
  Token::TokenClass fourAhead = lookAhead(4).tokenClass;
  if (twoAhead != Token::TokenClass::SC &&
      twoAhead != Token::TokenClass::LSBR &&
      threeAhead != Token::TokenClass::SC &&
      threeAhead != Token::TokenClass::LSBR &&
      fourAhead != Token::TokenClass::SC &&
      fourAhead != Token::TokenClass::LSBR) {
    return output;
  }
  if (accept({Token::TokenClass::STRUCT, Token::TokenClass::INT,
              Token::TokenClass::CHAR, Token::TokenClass::VOID})) {
    if (accept(Token::TokenClass::STRUCT)) {
      expect(Token::TokenClass::STRUCT);
      std::string structType = expect(Token::TokenClass::IDENTIFIER).data;
      std::shared_ptr<Type> varType(new StructType(structType));

      // Check if this is a pointer.
      while (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<PointerType>(new PointerType(varType));
      }
      std::string varName = expect(Token::TokenClass::IDENTIFIER).data;

      // Check for array declaration.
      if (accept(Token::TokenClass::LSBR)) {
        expect(Token::TokenClass::LSBR);
        std::string arraySize = expect(Token::TokenClass::INT_LITERAL).data;
        expect(Token::TokenClass::RSBR);
        varType = std::shared_ptr<ArrayType>(new ArrayType(varType, arraySize));
      }
      expect(Token::TokenClass::SC);

      output.push_back(VarDecl(varType, varName));
    } else if (accept({Token::TokenClass::INT, Token::TokenClass::CHAR,
                       Token::TokenClass::VOID})) {

      Token typeToken = expect({Token::TokenClass::INT, Token::TokenClass::CHAR,
                                Token::TokenClass::VOID});

      std::shared_ptr<Type> varType = tokenToType(typeToken.tokenClass);

      while (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<Type>(new PointerType(varType));
      }

      std::string varName = expect(Token::TokenClass::IDENTIFIER).data;

      // Check for array declaration.
      if (accept(Token::TokenClass::LSBR)) {
        expect(Token::TokenClass::LSBR);
        std::string arraySize = expect(Token::TokenClass::INT_LITERAL).data;
        expect(Token::TokenClass::RSBR);
        varType = std::shared_ptr<Type>(new ArrayType(varType, arraySize));
      }

      expect(Token::TokenClass::SC);

      output.push_back(VarDecl(varType, varName));
    }

    // Try to parse more vardecl
    std::vector<VarDecl> moreVarDecls = parseVarDecls();
    output.insert(output.end(), moreVarDecls.begin(), moreVarDecls.end());
  }
  return output;
}

void Parser::parseIncludes() {
  if (accept(Token::TokenClass::INCLUDE)) {
    expect(Token::TokenClass::INCLUDE);
    expect(Token::TokenClass::STRING_LITERAL);
    parseIncludes();
  }
}

/* Helpers */

std::shared_ptr<BaseType> Parser::tokenToType(const Token::TokenClass &tc) {
  switch (tc) {
  case Token::TokenClass::INT:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  case Token::TokenClass::CHAR:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
  case Token::TokenClass::VOID:
    return std::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  default:
    throw std::runtime_error("Parsing: Cannot resolve Token " + tokToStr(tc) +
                             "  to a type.");
  }
}