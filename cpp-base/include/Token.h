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

};

static std::string tokToStr(const Token::TokenClass &tok) {
  switch (tok) {
  case Token::TokenClass::IDENTIFIER:
    return "IDENTIFIER";
  case Token::TokenClass::ASSIGN:
    return "ASSIGN";
  case Token::TokenClass::LBRA:
    return "LBRA";
  case Token::TokenClass::RBRA:
    return "RBRA";
  case Token::TokenClass::LPAR:
    return "LPAR";
  case Token::TokenClass::RPAR:
    return "RPAR";
  case Token::TokenClass::LSBR:
    return "LSBR";
  case Token::TokenClass::RSBR:
    return "RSBR";
  case Token::TokenClass::SC:
    return "SC";
  case Token::TokenClass::COMMA:
    return "COMMA";
  case Token::TokenClass::INT:
    return "INT";
  case Token::TokenClass::VOID:
    return "VOID";
  case Token::TokenClass::CHAR:
    return "CHAR";
  case Token::TokenClass::IF:
    return "IF";
  case Token::TokenClass::ELSE:
    return "ELSE";
  case Token::TokenClass::WHILE:
    return "WHILE";
  case Token::TokenClass::RETURN:
    return "RETURN";
  case Token::TokenClass::STRUCT:
    return "STRUCT";
  case Token::TokenClass::SIZEOF:
    return "SIZEOF";
  case Token::TokenClass::INCLUDE:
    return "INCLUDE";
  case Token::TokenClass::STRING_LITERAL:
    return "STRING_LITERAL";
  case Token::TokenClass::INT_LITERAL:
    return "INT_LITERAL";
  case Token::TokenClass::CHAR_LITERAL:
    return "CHAR_LITERAL";
  case Token::TokenClass::AND:
    return "AND";
  case Token::TokenClass::OR:
    return "OR";
  case Token::TokenClass::EQ:
    return "EQ";
  case Token::TokenClass::NE:
    return "NE";
  case Token::TokenClass::LT:
    return "LT";
  case Token::TokenClass::GT:
    return "GT";
  case Token::TokenClass::LE:
    return "LE";
  case Token::TokenClass::GE:
    return "GE";
  case Token::TokenClass::PLUS:
    return "PLUS";
  case Token::TokenClass::MINUS:
    return "MINUS";
  case Token::TokenClass::ASTERIX:
    return "ASTERIX";
  case Token::TokenClass::DIV:
    return "DIV";
  case Token::TokenClass::REM:
    return "REM";
  case Token::TokenClass::DOT:
    return "DOT";
  case Token::TokenClass::ENDOFFILE:
    return "ENDOFFILE";
  case Token::TokenClass::INVALID:
    return "INVALID";
  }
}

}; // namespace ACC