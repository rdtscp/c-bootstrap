#include <iostream>
#include <stdexcept>

#include "../include/Lexer.h"
#include "../include/SourceCode.h"
#include "../include/Token.h"

using namespace ACC;

Lexer::Lexer(const SourceCode &i_src) : src(i_src) {}

Token Lexer::nextToken() {
  // Get the next Char.
  char c = src.next();

  // Find EOF.
  if (c == '\0')
    return Token(Token::TokenClass::ENDOFFILE, src.getPosition());

  // Skip through Comments.
  if (c == '/' && (src.peek() == '*' || src.peek() == '/')) {
    passComment();
    return nextToken();
  }

  /* Parse Multi Symbol Tokens */

  // Recognise STRING_LITERAL Token.
  if (c == '"')
    return lexStringLiteral();

  // Recognise CHAR_LITERAL token.
  if (c == '\'') {
    c = src.next();
    if (c == '\\') {
      char peek = src.peek();
      // Valid Escapes: '\t' | '\b' | '\n' | '\r' | '\f' | '\'' | '\"' | '\\'
      if (peek == 't' || peek == 'b' || peek == 'n' || peek == 'r' ||
          peek == 'f' || peek == '\'' || peek == '"' || peek == '\\') {
        c = src.next();
        if (src.peek() == '\'') {
          char val = c;
          src.next();
          return Token(Token::TokenClass::CHAR_LITERAL, src.getPosition(),
                       std::to_string('\\' + val));
        }
      }
    }
    // Otherwise we have a normal char.
    else if (src.peek() == '\'') {
      char val = c;
      src.next();
      return Token(Token::TokenClass::CHAR_LITERAL, src.getPosition(),
                   std::string(1, val));
    }
  }
  // Recognise IDENTIFIERS & Keyword Tokens.
  if (isalpha(c) || c == '_') {
    std::string literal(1, c);
    // Check for CHAR Token.
    if (c == 'c' && src.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("char");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CHAR, src.getPosition());
    }
    // Check for CONST Token.
    if (c == 'c' && src.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("const");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CONST, src.getPosition());
    }
    // Check for DO Token.
    if (c == 'd' && src.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("do");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::DO, src.getPosition());
    }
    // Check for ELSE Token.
    else if (c == 'e' && src.peek() == 'l') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("else");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::ELSE, src.getPosition());
    }
    // Check for ENUM Token.
    else if (c == 'e' && src.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("enum");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::ENUM, src.getPosition());
    }
    // Check for EXTERN Token.
    else if (c == 'e' && src.peek() == 'x') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("extern");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::EXTERN, src.getPosition());
    }
    // Check for IF Token.
    else if (c == 'i' && src.peek() == 'f') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("if");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::IF, src.getPosition());
    }
    // Check for INT token.
    else if (c == 'i' && src.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("int");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::INT, src.getPosition());
    }
    // Check for RETURN Token.
    else if (c == 'r' && src.peek() == 'e') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("return");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::RETURN, src.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && src.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("short");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::SHORT, src.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && src.peek() == 'i') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("sizeof");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::SIZEOF, src.getPosition());
    }
    // Check for STRUCT Token.
    else if (c == 's' && src.peek() == 't') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("struct");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::STRUCT, src.getPosition());
    }
    // Check for TYPEDEF Token.
    else if (c == 't' && src.peek() == 'y') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("typedef");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::TYPEDEF, src.getPosition());
    }
    // Check for UNSIGNED Token.
    else if (c == 'u' && src.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("unsigned");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::UNSIGNED, src.getPosition());
    }
    // Check for WHILE Token.
    else if (c == 'w' && src.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("while");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::WHILE, src.getPosition());
    }
    // Check for VOID Token.
    else if (c == 'v' && src.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("void");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::VOID, src.getPosition());
    }

    // No keyword Token has been returned.
    // Now Lex an IDENTIFIER.
    char peek;
    while (true) {
      peek = src.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (std::isspace(peek)) {
        return Token(Token::TokenClass::IDENTIFIER, src.getPosition(), literal);
      }
      // If the next character is an illegal characater for an IDENTIFIER, we
      // have finished finding the token.
      if (!isalpha(peek) && !isdigit(peek) && peek != '_' && peek != '$') {
        return Token(Token::TokenClass::IDENTIFIER, src.getPosition(), literal);
      }
      // We are still Lexing the token.
      c = src.next();
      literal += c;
    }
  }
  // Recognise INT_LITERAL Token.
  if (isdigit(c)) {
    std::string literal(1, c);
    if (src.peek() == 'x') {
      literal += src.next();
    }
    while (isdigit(src.peek()))
      literal += src.next();

    return Token(Token::TokenClass::INT_LITERAL, src.getPosition(), literal);
  }

  /* Recognise Two Symbol Tokens. */
  if (c == '=' && src.peek() == '=') {
    src.next();
    return Token(Token::TokenClass::EQ, src.getPosition());
  }
  if (c == '!' && src.peek() == '=') {
    src.next();
    return Token(Token::TokenClass::NE, src.getPosition());
  }
  if (c == '<' && src.peek() == '=') {
    src.next();
    return Token(Token::TokenClass::LE, src.getPosition());
  }
  if (c == '>' && src.peek() == '=') {
    src.next();
    return Token(Token::TokenClass::GE, src.getPosition());
  }
  if (c == '&' && src.peek() == '&') {
    src.next();
    return Token(Token::TokenClass::AND, src.getPosition());
  }
  if (c == '|' && src.peek() == '|') {
    src.next();
    return Token(Token::TokenClass::OR, src.getPosition());
  }

  /* Recognise One Symbol Tokens. */
  if (c == '=')
    return Token(Token::TokenClass::ASSIGN, src.getPosition());
  if (c == '{')
    return Token(Token::TokenClass::LBRA, src.getPosition());
  if (c == '}')
    return Token(Token::TokenClass::RBRA, src.getPosition());
  if (c == '(')
    return Token(Token::TokenClass::LPAR, src.getPosition());
  if (c == ')')
    return Token(Token::TokenClass::RPAR, src.getPosition());
  if (c == '[')
    return Token(Token::TokenClass::LSBR, src.getPosition());
  if (c == ']')
    return Token(Token::TokenClass::RSBR, src.getPosition());
  if (c == ';')
    return Token(Token::TokenClass::SC, src.getPosition());
  if (c == ',')
    return Token(Token::TokenClass::COMMA, src.getPosition());
  if (c == '+')
    return Token(Token::TokenClass::PLUS, src.getPosition());
  if (c == '-')
    return Token(Token::TokenClass::MINUS, src.getPosition());
  if (c == '*')
    return Token(Token::TokenClass::ASTERIX, src.getPosition());
  if (c == '%')
    return Token(Token::TokenClass::REM, src.getPosition());
  if (c == '.')
    return Token(Token::TokenClass::DOT, src.getPosition());
  if (c == '/')
    return Token(Token::TokenClass::DIV, src.getPosition());
  if (c == '>')
    return Token(Token::TokenClass::GT, src.getPosition());
  if (c == '<')
    return Token(Token::TokenClass::LT, src.getPosition());
  if (c == '&')
    return Token(Token::TokenClass::REF, src.getPosition());

  // Skip Whitespace.
  if (std::isspace(c))
    return nextToken();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error("Lexer: Unexpected Token. " +
                           src.getPosition().toString());
}

Token Lexer::lexStringLiteral() {
  std::string literal;
  int currLine = src.getPosition().line;

  char c;
  while (true) {
    c = src.next();
    if (c == '\0')
      throw std::runtime_error("Lexer: Unexpected EOF in String Literal. " +
                               src.getPosition().toString());
    if (c == '\n')
      throw std::runtime_error(
          "Lexer: Unexpected Newline Character in String Literal. " +
          src.getPosition().toString());

    // Check if we are about to see an escaped character.
    if (c == '\\') {
      literal += c;
      c = src.next();
      literal += c;
    } else if (c == '"') {
      break;
    } else {
      literal += c;
    }
  }
  return Token(Token::TokenClass::STRING_LITERAL, src.getPosition(), literal);
}

void Lexer::passComment() {
  // Consume the '/' or '*' character.
  char c = src.next();
  int currLine = src.getPosition().line;
  if (c == '/') {
    while (src.getPosition().line == currLine)
      src.next();
    return;
  } else if (c == '*') {
    c = src.next();
    while (true) {
      c = src.next();
      if (c == '*' && src.peek() == '/') {
        src.next(); // Consume the closing DIV.
        c = src.next();
        char temp = src.peek();
        return;
      }
      if (c == '\0')
        break;
    }
  }
  throw std::runtime_error(
      "Lexer: Parsing Comment Returned Unexpected Token(s). " +
      src.getPosition().toString());
}

std::pair<bool, std::string> Lexer::tryLexKeyword(const std::string &keyword) {
  bool keywordMatch = false;
  std::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); i++) {
    char peek = src.peek();
    if (src.peek() != keyword[i])
      return std::pair<bool, std::string>(false, literal);

    literal += src.next();
  }

  char peek = src.peek();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return std::pair<bool, std::string>(keywordMatch, literal);
}
