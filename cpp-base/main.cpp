#include <iostream>
#include <string>

#include "include/Scanner.h"
#include "include/Tokeniser.h"

std::string tokToStr(ACC::Token::TokenClass tok) {
  switch (tok) {
    case ACC::Token::TokenClass::IDENTIFIER:
      return "IDENTIFIER";
    case ACC::Token::TokenClass::ASSIGN:
      return "ASSIGN";
    case ACC::Token::TokenClass::LBRA:
      return "LBRA";
    case ACC::Token::TokenClass::RBRA:
      return "RBRA";
    case ACC::Token::TokenClass::LPAR:
      return "LPAR";
    case ACC::Token::TokenClass::RPAR:
      return "RPAR";
    case ACC::Token::TokenClass::LSBR:
      return "LSBR";
    case ACC::Token::TokenClass::RSBR:
      return "RSBR";
    case ACC::Token::TokenClass::SC:
      return "SC";
    case ACC::Token::TokenClass::COMMA:
      return "COMMA";
    case ACC::Token::TokenClass::INT:
      return "INT";
    case ACC::Token::TokenClass::VOID:
      return "VOID";
    case ACC::Token::TokenClass::CHAR:
      return "CHAR";
    case ACC::Token::TokenClass::IF:
      return "IF";
    case ACC::Token::TokenClass::ELSE:
      return "ELSE";
    case ACC::Token::TokenClass::WHILE:
      return "WHILE";
    case ACC::Token::TokenClass::RETURN:
      return "RETURN";
    case ACC::Token::TokenClass::STRUCT:
      return "STRUCT";
    case ACC::Token::TokenClass::SIZEOF:
      return "SIZEOF";
    case ACC::Token::TokenClass::INCLUDE:
      return "INCLUDE";
    case ACC::Token::TokenClass::STRING_LITERAL:
      return "STRING_LITERAL";
    case ACC::Token::TokenClass::INT_LITERAL:
      return "INT_LITERAL";
    case ACC::Token::TokenClass::CHAR_LITERAL:
      return "CHAR_LITERAL";
    case ACC::Token::TokenClass::AND:
      return "AND";
    case ACC::Token::TokenClass::OR:
      return "OR";
    case ACC::Token::TokenClass::EQ:
      return "EQ";
    case ACC::Token::TokenClass::NE:
      return "NE";
    case ACC::Token::TokenClass::LT:
      return "LT";
    case ACC::Token::TokenClass::GT:
      return "GT";
    case ACC::Token::TokenClass::LE:
      return "LE";
    case ACC::Token::TokenClass::GE:
      return "GE";
    case ACC::Token::TokenClass::PLUS:
      return "PLUS";
    case ACC::Token::TokenClass::MINUS:
      return "MINUS";
    case ACC::Token::TokenClass::ASTERIX:
      return "ASTERIX";
    case ACC::Token::TokenClass::DIV:
      return "DIV";
    case ACC::Token::TokenClass::REM:
      return "REM";
    case ACC::Token::TokenClass::DOT:
      return "DOT";
    case ACC::Token::TokenClass::ENDOFFILE:
      return "ENDOFFILE";
    case ACC::Token::TokenClass::INVALID:
      return "INVALID";
  }
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Provide Filename" << std::endl;
    return 1;
  }

  std::string filename(argv[1]);

  ACC::Scanner scanner(filename);

  ACC::Tokeniser tokeniser(scanner);

  ACC::Token currToken = tokeniser.nextToken();
  while (currToken.tokenClass != ACC::Token::TokenClass::ENDOFFILE) {
    std::cout << "Token: " << tokToStr(currToken.tokenClass) << std::endl;
    currToken = tokeniser.nextToken();
  }
  
  /* code */
  return 0;
}
