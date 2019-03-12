#pragma once

#ifndef ACC_TOKEN_H
#define ACC_TOKEN_H

#include "atl/include/string.h"

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
    UINT,  // "unsigned int"

    // keywords
    IF,         // "if"
    ELSE,       // "else"
    DO,         // "do"
    WHILE,      // "while"
    RETURN,     // "return"
    STRUCT,     // "struct"
    SIZEOF,     // "sizeof"
    CONST,      // "const"
    TYPEDEF,    // "typedef"
    ENUM,       // "enum"
    NAMESPACE,  // "namespace"
    CLASS,      // "class"
    PUBLIC,     // "public:"
    PRIVATE,    // "private:"
    PROTECTED,  // "protected:"
    NEW,        // "new"
    FOR,        // "for"
    PREFIXINC,  // "++{}",
    THIS,       // "this"
    DESTRUCTOR, // "~"
    DELETE,     // "delete"
    DELETEARR,  // "delete[]"

    // operators overrides
    OPASSIGN, // "operator="
    OPEQ,     // "operator=="
    OPADD,    // "operator+"
    OPADDTO,  // "operator+="
    OPAT,     // "operator[]"
    OPLT,     // "operator<"
    OPLE,     // "operator<="
    OPGT,     // "operator>"
    OPGE,     // "operator>="
    OPNE,     // "operator!="

    // literals
    STRING_LITERAL, // \".*\"  any sequence of characters enclosed within
                    // two double quote " (please be aware of the escape
                    // character backslash \)
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
    DOT,    // '.'
    PTRDOT, // "->"

    // special tokens
    ENDOFFILE, // signal end of file
    INVALID // in case we cannot recognise a character as part of a valid token
  };

  atl::string data;
  Position position;
  Class tokenClass;

  SourceToken()
      : tokenClass(Class::INVALID), position(Position(-1, -1, "")), data("") {}

  SourceToken(Class tokenClass, Position p, atl::string data = "")
      : tokenClass(tokenClass), position(p), data(data) {}

  bool operator==(const SourceToken &rhs) {
    return tokenClass == rhs.tokenClass;
  }
};

static atl::string tokToStr(const SourceToken::Class &tok) {
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
  case SourceToken::Class::UINT:
    return "UINT";
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
  case SourceToken::Class::PTRDOT:
    return "->";
  case SourceToken::Class::ENDOFFILE:
    return "EOF";
  case SourceToken::Class::INVALID:
    return "INVALID";
  case SourceToken::Class::REF:
    return "&";
  case SourceToken::Class::CONST:
    return "CONST";
  case SourceToken::Class::TYPEDEF:
    return "TYPEDEF";
  case SourceToken::Class::ENUM:
    return "ENUM";
  case SourceToken::Class::SHORT:
    return "SHORT";
  case SourceToken::Class::NAMESPACE:
    return "NAMESPACE";
  case SourceToken::Class::CLASS:
    return "CLASS";
  case SourceToken::Class::PUBLIC:
    return "public:";
  case SourceToken::Class::PRIVATE:
    return "private:";
  case SourceToken::Class::PROTECTED:
    return "protected:";
  case SourceToken::Class::NEW:
    return "new";
  case SourceToken::Class::FOR:
    return "for";
  case SourceToken::Class::PREFIXINC:
    return "++";
  case SourceToken::Class::THIS:
    return "this";
  case SourceToken::Class::DESTRUCTOR:
    return "~";
  case SourceToken::Class::DELETE:
    return "delete";
  case SourceToken::Class::DELETEARR:
    return "delete[]";
  case SourceToken::Class::OPASSIGN:
    return "operator=";
  case SourceToken::Class::OPEQ:
    return "operator==";
  case SourceToken::Class::OPADD:
    return "operator+";
  case SourceToken::Class::OPADDTO:
    return "operator+=";
  case SourceToken::Class::OPAT:
    return "operator[]";
  case SourceToken::Class::OPLT:
    return "operator<";
  case SourceToken::Class::OPLE:
    return "operator<=";
  case SourceToken::Class::OPGT:
    return "operator>";
  case SourceToken::Class::OPGE:
    return "operator>=";
  case SourceToken::Class::OPNE:
    return "operator!=";
  }
}

}; // namespace ACC

#endif