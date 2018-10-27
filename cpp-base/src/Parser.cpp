#include <memory>
#include <stdexcept>

#include "../include/Parser.h"

/* Public */

ACC::Parser::Parser(const ACC::Tokeniser &tokeniser)
    : currToken(ACC::Token::TokenClass::INVALID, -1, -1), tokeniser(tokeniser) {
}

ACC::Program ACC::Parser::parse() {
  nextToken();
  return parseProgram();
}

/* Token Operations */

bool ACC::Parser::accept(ACC::Token::TokenClass expected) {
  return expected == currToken.tokenClass;
}

bool ACC::Parser::accept(std::vector<ACC::Token::TokenClass> expected) {
  bool output = false;
  for (const ACC::Token::TokenClass token : expected) {
    bool curr = accept(token);
    // printf("%s == %s => %i\n", ACC::tokToStr(token).c_str(),
    //        ACC::tokToStr(currToken.tokenClass).c_str(), curr);
    output |= curr;
  }
  return output;
}

ACC::Token ACC::Parser::expect(std::vector<ACC::Token::TokenClass> expected) {
  for (ACC::Token::TokenClass token : expected) {
    if (token == currToken.tokenClass) {
      Token output = currToken;
      nextToken();
      return output;
    }
  }
  throw std::runtime_error("Invalid Token at " + currToken.position.toString());
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
  ACC::Token::TokenClass twoAhead = lookAhead(2).tokenClass;
  ACC::Token::TokenClass threeAhead = lookAhead(3).tokenClass;
  ACC::Token::TokenClass fourAhead = lookAhead(4).tokenClass;
  if (twoAhead != ACC::Token::TokenClass::SC &&
      twoAhead != ACC::Token::TokenClass::LSBR &&
      threeAhead != ACC::Token::TokenClass::SC &&
      threeAhead != ACC::Token::TokenClass::LSBR &&
      fourAhead != ACC::Token::TokenClass::SC &&
      fourAhead != ACC::Token::TokenClass::LSBR) {
    return output;
  }
  if (accept({ACC::Token::TokenClass::STRUCT, ACC::Token::TokenClass::INT,
              ACC::Token::TokenClass::CHAR, ACC::Token::TokenClass::VOID})) {
    if (accept(ACC::Token::TokenClass::STRUCT)) {
      // std::string structType;
      // std::string varName;
      // Type varType;

      // expect(ACC::Token::TokenClass::STRUCT);
      // structType = expect(ACC::Token::TokenClass::IDENTIFIER).data;
      // varType = StructType(structType);

      // // Check if this is a pointer.
      // if (accept(ACC::Token::TokenClass::ASTERIX)) {
      //   expect(ACC::Token::TokenClass::ASTERIX);
      //   varType = new PointerType(varType);
      // }
      // varName = expect(ACC::Token::TokenClass::IDENTIFIER).data;

      // // Check for array declaration.
      // if (accept(ACC::Token::TokenClass::LSBR)) {
      //   expect(ACC::Token::TokenClass::LSBR);
      //   String arraySize = expect(ACC::Token::TokenClass::INT_LITERAL).data;
      //   expect(ACC::Token::TokenClass::RSBR);
      //   varType = new ArrayType(varType, arraySize);
      // }
      // expect(ACC::Token::TokenClass::SC);

      // output.add(new VarDecl(varType, varName));
    } else if (accept({ACC::Token::TokenClass::INT,
                       ACC::Token::TokenClass::CHAR,
                       ACC::Token::TokenClass::VOID})) {

      ACC::Token typeToken =
          expect({ACC::Token::TokenClass::INT, ACC::Token::TokenClass::CHAR,
                  ACC::Token::TokenClass::VOID});

      std::shared_ptr<ACC::Type> varType =
          tokenToType(typeToken.tokenClass);

      if (accept(ACC::Token::TokenClass::ASTERIX)) {
        expect(ACC::Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<ACC::Type>(new PointerType(varType));
      }

      if (accept(ACC::Token::TokenClass::ASTERIX)) {
        expect(ACC::Token::TokenClass::ASTERIX);
        varType = std::shared_ptr<ACC::Type>(new PointerType(varType));
      }

      std::string varName = expect(ACC::Token::TokenClass::IDENTIFIER).data;

      // // Check for array declaration.
      // if (accept(ACC::Token::TokenClass::LSBR)) {
      //   expect(ACC::Token::TokenClass::LSBR);
      //   String arraySize = expect(ACC::Token::TokenClass::INT_LITERAL).data;
      //   expect(ACC::Token::TokenClass::RSBR);
      //   varType = new ArrayType(varType, arraySize);
      // }
      expect(ACC::Token::TokenClass::SC);

      output.push_back(VarDecl(varType, varName));
    }

    // // Try to parse more vardecl
    std::vector<ACC::VarDecl> moreVarDecls = parseVarDecls();
    output.insert(output.end(), moreVarDecls.begin(), moreVarDecls.end());
  }
  return output;
}

void ACC::Parser::parseIncludes() {
  if (accept(ACC::Token::TokenClass::INCLUDE)) {
    expect(ACC::Token::TokenClass::INCLUDE);
    expect(ACC::Token::TokenClass::STRING_LITERAL);
    parseIncludes();
  }
}

/* Helpers */

std::shared_ptr<ACC::BaseType>
ACC::Parser::tokenToType(const ACC::Token::TokenClass &tc) {
  switch (tc) {
  case ACC::Token::TokenClass::INT:
    return std::make_shared<ACC::BaseType>(
        ACC::BaseType(ACC::PrimitiveType::INT));
  case ACC::Token::TokenClass::CHAR:
    return std::make_shared<ACC::BaseType>(
        ACC::BaseType(ACC::PrimitiveType::CHAR));
  case ACC::Token::TokenClass::VOID:
    return std::make_shared<ACC::BaseType>(
        ACC::BaseType(ACC::PrimitiveType::VOID));
  default:
    throw std::runtime_error("Cannot resolve Token " + ACC::tokToStr(tc) +
                             "  to a type.");
  }
}