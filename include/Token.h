#pragma once

#ifndef TOKEN_H
#define TOKEN_H

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
    CONST,  // "const"

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
    REF,     // '&'

    // struct member access
    DOT, // '.'

    // special tokens
    ENDOFFILE, // signal end of file
    INVALID // in case we cannot recognise a character as part of a valid token
  };

  std::string data;
  Position position;
  TokenClass tokenClass;

  Token()
      : tokenClass(Token::TokenClass::INVALID), position(Position(-1, -1)),
        data("") {}

  Token(TokenClass tokenClass, int lineNum, int colNum, std::string data = "")
      : tokenClass(tokenClass), position(Position(lineNum, colNum)),
        data(data) {}

  bool operator==(const Token& t) {
    return tokenClass == t.tokenClass;
  }
};

static std::string tokToStr(const Token::TokenClass &tok) {
  switch (tok) {
  case Token::TokenClass::IDENTIFIER:
    return "IDENTIFIER";
  case Token::TokenClass::ASSIGN:
    return "=";
  case Token::TokenClass::LBRA:
    return "{";
  case Token::TokenClass::RBRA:
    return "}";
  case Token::TokenClass::LPAR:
    return "(";
  case Token::TokenClass::RPAR:
    return ")";
  case Token::TokenClass::LSBR:
    return "[";
  case Token::TokenClass::RSBR:
    return "]";
  case Token::TokenClass::SC:
    return ";";
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
    return "&&";
  case Token::TokenClass::OR:
    return "||";
  case Token::TokenClass::EQ:
    return "==";
  case Token::TokenClass::NE:
    return "!=";
  case Token::TokenClass::LT:
    return "<";
  case Token::TokenClass::GT:
    return ">";
  case Token::TokenClass::LE:
    return "<=";
  case Token::TokenClass::GE:
    return ">=";
  case Token::TokenClass::PLUS:
    return "+";
  case Token::TokenClass::MINUS:
    return "-";
  case Token::TokenClass::ASTERIX:
    return "*";
  case Token::TokenClass::DIV:
    return "/";
  case Token::TokenClass::REM:
    return "%";
  case Token::TokenClass::DOT:
    return ".";
  case Token::TokenClass::ENDOFFILE:
    return "EOF";
  case Token::TokenClass::INVALID:
    return "INVALID";
  case Token::TokenClass::REF:
    return "&";
  case Token::TokenClass::CONST:
    return "CONST";
  }
}

}; // namespace ACC

#endif