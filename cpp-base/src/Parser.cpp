#include <memory>
#include <stdexcept>

#include "../include/Parser.h"

/* Public */

using namespace ACC;

Parser::Parser(const Tokeniser &tokeniser)
    : currToken(Token::TokenClass::INVALID, -1, -1), tokeniser(tokeniser) {}

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
    // printf("%s == %s => %i\n", tokToStr(token).c_str(),
    //        tokToStr(currToken.tokenClass).c_str(), curr);
    output |= curr;
  }
  return output;
}

Token Parser::expect(std::vector<Token::TokenClass> expected) {
  for (Token::TokenClass token : expected) {
    if (token == currToken.tokenClass) {
      Token output = currToken;
      nextToken();
      return output;
    }
  }
  throw std::runtime_error("Invalid Token at " + currToken.position.toString());
}

Token Parser::expect(Token::TokenClass expected) {
  if (expected == currToken.tokenClass) {
    Token temp = currToken;
    nextToken();
    return temp;
  }
  return currToken;
}

Token Parser::lookAhead(int i) {
  assert(i > 0);
  while (tokenBuffer.size() < i) {
    tokenBuffer.push_back(tokeniser.nextToken());
  }

  return tokenBuffer[i - 1];
}

void Parser::nextToken() {
  // Use Buffer
  if (tokenBuffer.size() != 0) {
    currToken = tokenBuffer.front();
    tokenBuffer.erase(tokenBuffer.begin());
  } else {
    currToken = tokeniser.nextToken();
  }
}

/* AST Construction */

std::shared_ptr<Type> Parser::expectType() {
  if (accept({Token::TokenClass::INT, Token::TokenClass::CHAR,
              Token::TokenClass::VOID})) {
    Token typeToken = expect({Token::TokenClass::INT, Token::TokenClass::CHAR,
                              Token::TokenClass::VOID});
    std::shared_ptr<Type> type = tokenToType(typeToken.tokenClass);
    if (accept(Token::TokenClass::ASTERIX)) {
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
    throw std::runtime_error("Expected a Type at " +
                             currToken.position.toString());
  }
}

VarDecl Parser::expectVarDecl() {
  std::shared_ptr<Type> type = expectType();
  std::string varName = expect(Token::TokenClass::IDENTIFIER).data;
  if (accept(Token::TokenClass::LSBR)) {
    expect(Token::TokenClass::LSBR);
    expect(Token::TokenClass::INT_LITERAL);
    expect(Token::TokenClass::RSBR);
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
  /* @TODO */
  std::vector<FunDecl> output;
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
    output.push_back(StructTypeDecl(std::shared_ptr<StructType>(new StructType(structName)), varDecls));

    // Try to parse more StructDecl's to add to our output.
    std::vector<StructTypeDecl> moreStructTypes = parseStructDecls();
    output.insert(output.end(), moreStructTypes.begin(), moreStructTypes.end());
  }
  return output;
}

std::vector<VarDecl> Parser::parseVarDecls() {
  /* @TODO */
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
      if (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<PointerType>(new PointerType(varType));
      }
      std::string varName = expect(Token::TokenClass::IDENTIFIER).data;

      // Check for array declaration.
      if (accept(Token::TokenClass::LSBR)) {
        expect(Token::TokenClass::LSBR);
        std::string arraySize =
        expect(Token::TokenClass::INT_LITERAL).data;
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

      if (accept(Token::TokenClass::ASTERIX)) {
        expect(Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<Type>(new PointerType(varType));
      }

      if (accept(Token::TokenClass::ASTERIX)) {
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
    throw std::runtime_error("Cannot resolve Token " + tokToStr(tc) +
                             "  to a type.");
  }
}