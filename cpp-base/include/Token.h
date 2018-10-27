#pragma once

#include "Position.h"

namespace ACC {

class Token {

public:
  enum class TokenClass {

    // the \ (backslash) is used as an escape character in the regular
    // expression below
    // ' is used to enclose character while " is used to enclose strings

    IDENTIFIER, // ('a'|...|'z'|'A'|...|'Z'|'_')('0'|...|'9'|'a'|...|'z'|'A'|...|'Z'|'_')*

    ASSIGN, // '='

    // delimiters
    LBRA,  // '{' // left brace
    RBRA,  // '}' // right brace
    LPAR,  // '(' // left parenthesis
    RPAR,  // ')' // right parenthesis
    LSBR,  // '[' // left square brace
    RSBR,  // ']' // left square brace
    SC,    // ';'   // semicolon
    COMMA, // ','

    // types
    INT,  // "int"
    VOID, // "void"
    CHAR, // "char"

    // keywords
    IF,     // "if"
    ELSE,   // "else"
    WHILE,  // "while"
    RETURN, // "return"
    STRUCT, // "struct"
    SIZEOF, // "sizeof"

    // include
    INCLUDE, // "#include"

    // literals
    STRING_LITERAL, // \".*\"  any sequence of characters enclosed within two
                    // double quote " (please be aware of the escape character
                    // backslash \)
    INT_LITERAL,    // ('0'|...|'9')+
    CHAR_LITERAL, // \'('a'|...|'z'|'A'|...|'Z'|'\t'|'\b'|'\n'|'\r'|'\f'|'\''|'\"'|'\\'|'.'|','|'_'|...)\'
                  // a character starts and end with a single quote '

    // logical operators
    AND, // "&&"
    OR,  // "||"

    // comparisons
    EQ, // "=="
    NE, // "!="
    LT, // '<'
    GT, // '>'
    LE, // "<="
    GE, // ">="

    // operators
    PLUS,    // '+'
    MINUS,   // '-'
    ASTERIX, // '*'  // can be used for multiplication or pointers
    DIV,     // '/'
    REM,     // '%'

    // struct member access
    DOT, // '.'

    // special tokens
    ENDOFFILE, // signal end of file
    INVALID // in case we cannot recognise a character as part of a valid token
  };

  std::string data;
  Position position;
  TokenClass tokenClass;

  Token();

  Token(TokenClass tokenClass, int lineNum, int colNum, std::string data = "");
  
  Token(const ACC::Token& t);

  ACC::Token operator=(Token t);

};

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

}; // namespace ACC