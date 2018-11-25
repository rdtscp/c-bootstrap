#include <iostream>
#include <stdexcept>

#include "../include/Lexer.h"
#include "../include/Scanner.h"
#include "../include/Token.h"

using namespace ACC;

Lexer::Lexer(const Scanner &scanner) : scanner(scanner) {}

Token Lexer::lexStringLiteral() {
  std::string literal;
  int currLine = scanner.line;

  char c;
  while (true) {
    c = scanner.next();
    if (c == '\0')
      throw std::runtime_error(
          "Lexer: Unexpected EOF in String Literal at Line " +
          std::to_string(scanner.line) + ", Column " +
          std::to_string(scanner.column));
    if (c == '\n')
      throw std::runtime_error(
          "Lexer: Unexpected Newline Character in String Literal at Line " +
          std::to_string(scanner.line) + ", Column " +
          std::to_string(scanner.column));

    // Check if we are about to see an escaped character.
    if (c == '\\') {
      literal += c;
      c = scanner.next();
      literal += c;
    } else if (c == '"') {
      break;
    } else {
      literal += c;
    }
  }
  return Token(Token::TokenClass::STRING_LITERAL, scanner.line, scanner.column,
               literal);
}

void Lexer::passComment() {
  // Consume the '/' or '*' character.
  char c = scanner.next();
  int currLine = scanner.line;
  if (c == '/') {
    while (scanner.line == currLine)
      scanner.next();
    return;
  } else if (c == '*') {
    c = scanner.next();
    while (true) {
      c = scanner.next();
      if (c == '*' && scanner.peek() == '/') {
        scanner.next(); // Consume the closing DIV.
        c = scanner.next();
        char temp = scanner.peek();
        return;
      }
      if (c == '\0')
        break;
    }
  }
  throw std::runtime_error(
      "Lexer: Parsing Comment Returned Unexpected Token(s). Line " +
      std::to_string(scanner.line) + ", Column " +
      std::to_string(scanner.column));
}

std::pair<bool, std::string> Lexer::tryLexKeyword(const std::string &keyword) {
  bool keywordMatch = false;
  std::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); i++) {
    char peek = scanner.peek();
    if (scanner.peek() != keyword[i])
      return std::pair<bool, std::string>(false, literal);

    literal += scanner.next();
  }

  char peek = scanner.peek();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return std::pair<bool, std::string>(keywordMatch, literal);
}

Token Lexer::nextToken() {
  // Get the next Char.
  char c = scanner.next();

  // Find EOF.
  if (c == '\0')
    return Token(Token::TokenClass::ENDOFFILE, scanner.line, scanner.column);

  // Skip through Comments.
  if (c == '/' && (scanner.peek() == '*' || scanner.peek() == '/')) {
    passComment();
    return nextToken();
  }

  /* Parse Multi Symbol Tokens */

  // Recognise STRING_LITERAL Token.
  if (c == '"')
    return lexStringLiteral();

  // Recognise CHAR_LITERAL token.
  if (c == '\'') {
    c = scanner.next();
    if (c == '\\') {
      char peek = scanner.peek();
      // Valid Escapes: '\t' | '\b' | '\n' | '\r' | '\f' | '\'' | '\"' | '\\'
      if (peek == 't' || peek == 'b' || peek == 'n' || peek == 'r' ||
          peek == 'f' || peek == '\'' || peek == '"' || peek == '\\') {
        c = scanner.next();
        if (scanner.peek() == '\'') {
          char val = c;
          scanner.next();
          return Token(Token::TokenClass::CHAR_LITERAL, scanner.line,
                       scanner.column, std::to_string('\\' + val));
        }
      }
    }
    // Otherwise we have a normal char.
    else if (scanner.peek() == '\'') {
      char val = c;
      scanner.next();
      return Token(Token::TokenClass::CHAR_LITERAL, scanner.line,
                   scanner.column, std::to_string(val));
    }
  }
  // Recognise IDENTIFIERS & Keyword Tokens.
  if (isalpha(c) || c == '_') {
    std::string literal(1, c);
    // Check for CHAR Token.
    if (c == 'c' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("char");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CHAR, scanner.line, scanner.column);
    }
    // Check for CONST Token.
    if (c == 'c' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("const");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CONST, scanner.line, scanner.column);
    }
    // Check for ELSE Token.
    else if (c == 'e' && scanner.peek() == 'l') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("else");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::ELSE, scanner.line, scanner.column);
    }
    // Check for IF Token.
    else if (c == 'i' && scanner.peek() == 'f') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("if");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::IF, scanner.line, scanner.column);
    }
    // Check for INT token.
    else if (c == 'i' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("int");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::INT, scanner.line, scanner.column);
    }
    // Check for RETURN Token.
    else if (c == 'r' && scanner.peek() == 'e') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("return");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::RETURN, scanner.line, scanner.column);
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'i') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("sizeof");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::SIZEOF, scanner.line, scanner.column);
    }
    // Check for STRUCT Token.
    else if (c == 's' && scanner.peek() == 't') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("struct");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::STRUCT, scanner.line, scanner.column);
    }
    // Check for WHILE Token.
    else if (c == 'w' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("while");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::WHILE, scanner.line, scanner.column);
    }
    // Check for VOID Token.
    else if (c == 'v' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("void");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::VOID, scanner.line, scanner.column);
    }

    // No keyword Token has been returned.
    // Now Lex an IDENTIFIER.
    char peek;
    while (true) {
      peek = scanner.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (std::isspace(peek)) {
        return Token(Token::TokenClass::IDENTIFIER, scanner.line,
                     scanner.column, literal);
      }
      // If the next character is an illegal characater for an IDENTIFIER, we
      // have finished finding the token.
      if (!isalpha(peek) && !isdigit(peek) && peek != '_') {
        return Token(Token::TokenClass::IDENTIFIER, scanner.line,
                     scanner.column, literal);
      }
      // We are still Lexing the token.
      c = scanner.next();
      literal += c;
    }
  }
  // Recognise INT_LITERAL Token.
  if (isdigit(c)) {
    std::string literal;
    while (true) {
      literal += c;
      if (!isdigit(scanner.peek()))
        break;
      c = scanner.next();
    }
    return Token(Token::TokenClass::INT_LITERAL, scanner.line, scanner.column,
                 literal);
  }

  // Recognise INCLUDE token.
  if (c == '#' && scanner.peek() == 'i') {
    scanner.next();
    std::pair<bool, std::string> lexResult = tryLexKeyword("include");

    c = lexResult.second[lexResult.second.length() - 1];

    if (lexResult.first)
      return Token(Token::TokenClass::INCLUDE, scanner.line, scanner.column);
  }

  /* Recognise Two Symbol Tokens. */
  if (c == '=' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::EQ, scanner.line, scanner.column);
  }
  if (c == '!' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::NE, scanner.line, scanner.column);
  }
  if (c == '<' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::LE, scanner.line, scanner.column);
  }
  if (c == '>' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::GE, scanner.line, scanner.column);
  }
  if (c == '&' && scanner.peek() == '&') {
    scanner.next();
    return Token(Token::TokenClass::AND, scanner.line, scanner.column);
  }
  if (c == '|' && scanner.peek() == '|') {
    scanner.next();
    return Token(Token::TokenClass::OR, scanner.line, scanner.column);
  }

  /* Recognise One Symbol Tokens. */
  if (c == '=')
    return Token(Token::TokenClass::ASSIGN, scanner.line, scanner.column);
  if (c == '{')
    return Token(Token::TokenClass::LBRA, scanner.line, scanner.column);
  if (c == '}')
    return Token(Token::TokenClass::RBRA, scanner.line, scanner.column);
  if (c == '(')
    return Token(Token::TokenClass::LPAR, scanner.line, scanner.column);
  if (c == ')')
    return Token(Token::TokenClass::RPAR, scanner.line, scanner.column);
  if (c == '[')
    return Token(Token::TokenClass::LSBR, scanner.line, scanner.column);
  if (c == ']')
    return Token(Token::TokenClass::RSBR, scanner.line, scanner.column);
  if (c == ';')
    return Token(Token::TokenClass::SC, scanner.line, scanner.column);
  if (c == ',')
    return Token(Token::TokenClass::COMMA, scanner.line, scanner.column);
  if (c == '+')
    return Token(Token::TokenClass::PLUS, scanner.line, scanner.column);
  if (c == '-')
    return Token(Token::TokenClass::MINUS, scanner.line, scanner.column);
  if (c == '*')
    return Token(Token::TokenClass::ASTERIX, scanner.line, scanner.column);
  if (c == '%')
    return Token(Token::TokenClass::REM, scanner.line, scanner.column);
  if (c == '.')
    return Token(Token::TokenClass::DOT, scanner.line, scanner.column);
  if (c == '/')
    return Token(Token::TokenClass::DIV, scanner.line, scanner.column);
  if (c == '>')
    return Token(Token::TokenClass::GT, scanner.line, scanner.column);
  if (c == '<')
    return Token(Token::TokenClass::LT, scanner.line, scanner.column);
  if (c == '&')
    return Token(Token::TokenClass::REF, scanner.line, scanner.column);

  // Skip Whitespace.
  if (std::isspace(c))
    return nextToken();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error("Lexer: Unexpected Token at Line " +
                           std::to_string(scanner.line) + ", Column " +
                           std::to_string(scanner.column));
}
