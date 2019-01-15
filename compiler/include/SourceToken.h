#pragma once

#ifndef ACC_TOKEN_H
#define ACC_TOKEN_H

#include "Position.h"

namespace ACC {

class SourceToken final {

public:
  enum class Class {

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
    INT,   // "int"
    VOID,  // "void"
    CHAR,  // "char"
    SHORT, // "short"

    // keywords
    IF,        // "if"
    ELSE,      // "else"
    DO,        // "do"
    WHILE,     // "while"
    RETURN,    // "return"
    STRUCT,    // "struct"
    SIZEOF,    // "sizeof"
    CONST,     // "const"
    EXTERN,    // "extern"
    TYPEDEF,   // "typedef"
    ENUM,      // "enum"
    UNSIGNED,  // "unsigned"
    NAMESPACE, // "namespace"
    INLINE,    // "inline"

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
  Class tokenClass;

  SourceToken()
      : tokenClass(Class::INVALID), position(Position(-1, -1, "")), data("") {}

  SourceToken(Class tokenClass, Position p, std::string data = "")
      : tokenClass(tokenClass), position(p), data(data) {}

  bool operator==(const SourceToken &rhs) {
    return tokenClass == rhs.tokenClass;
  }
};

static std::string tokToStr(const SourceToken::Class &tok) {
  switch (tok) {
  case SourceToken::Class::IDENTIFIER:
    return "IDENTIFIER";
  case SourceToken::Class::ASSIGN:
    return "=";
  case SourceToken::Class::LBRA:
    return "{";
  case SourceToken::Class::RBRA:
    return "}";
  case SourceToken::Class::LPAR:
    return "(";
  case SourceToken::Class::RPAR:
    return ")";
  case SourceToken::Class::LSBR:
    return "[";
  case SourceToken::Class::RSBR:
    return "]";
  case SourceToken::Class::SC:
    return ";";
  case SourceToken::Class::COMMA:
    return "COMMA";
  case SourceToken::Class::INT:
    return "INT";
  case SourceToken::Class::VOID:
    return "VOID";
  case SourceToken::Class::CHAR:
    return "CHAR";
  case SourceToken::Class::IF:
    return "IF";
  case SourceToken::Class::ELSE:
    return "ELSE";
  case SourceToken::Class::DO:
    return "DO";
  case SourceToken::Class::WHILE:
    return "WHILE";
  case SourceToken::Class::RETURN:
    return "RETURN";
  case SourceToken::Class::STRUCT:
    return "STRUCT";
  case SourceToken::Class::SIZEOF:
    return "SIZEOF";
  case SourceToken::Class::STRING_LITERAL:
    return "STRING_LITERAL";
  case SourceToken::Class::INT_LITERAL:
    return "INT_LITERAL";
  case SourceToken::Class::CHAR_LITERAL:
    return "CHAR_LITERAL";
  case SourceToken::Class::AND:
    return "&&";
  case SourceToken::Class::OR:
    return "||";
  case SourceToken::Class::EQ:
    return "==";
  case SourceToken::Class::NE:
    return "!=";
  case SourceToken::Class::LT:
    return "<";
  case SourceToken::Class::GT:
    return ">";
  case SourceToken::Class::LE:
    return "<=";
  case SourceToken::Class::GE:
    return ">=";
  case SourceToken::Class::PLUS:
    return "+";
  case SourceToken::Class::MINUS:
    return "-";
  case SourceToken::Class::ASTERIX:
    return "*";
  case SourceToken::Class::DIV:
    return "/";
  case SourceToken::Class::REM:
    return "%";
  case SourceToken::Class::DOT:
    return ".";
  case SourceToken::Class::ENDOFFILE:
    return "EOF";
  case SourceToken::Class::INVALID:
    return "INVALID";
  case SourceToken::Class::REF:
    return "&";
  case SourceToken::Class::CONST:
    return "CONST";
  case SourceToken::Class::EXTERN:
    return "EXTERN";
  case SourceToken::Class::TYPEDEF:
    return "TYPEDEF";
  case SourceToken::Class::ENUM:
    return "ENUM";
  case SourceToken::Class::UNSIGNED:
    return "UNSIGNED";
  case SourceToken::Class::SHORT:
    return "SHORT";
  case SourceToken::Class::NAMESPACE:
    return "NAMESPACE";
  case SourceToken::Class::INLINE:
    return "INLINE";
  }
}

}; // namespace ACC

#endif