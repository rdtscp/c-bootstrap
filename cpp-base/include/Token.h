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

  Token(TokenClass tokenClass, int lineNum, int colNum, std::string data = "");

  std::string toString();
};

}; // namespace ACC