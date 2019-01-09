#include <iostream>
#include <stdexcept>

#include "../include/Lexer.h"

using namespace ACC;

using TC = SourceToken::Class;

Lexer::Lexer(Scanner &scanner) : scanner(scanner) {}

SourceToken Lexer::nextToken() {
  // Get the next Char.
  char c = scanner.next();

  // Find EOF.
  if (c == '\0')
    return SourceToken(TC::ENDOFFILE, scanner.getPosition());

  // Skip through Comments.
  if (c == '/' && (scanner.peek() == '*' || scanner.peek() == '/')) {
    passComment();
    return nextToken();
  }

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
          return SourceToken(TC::CHAR_LITERAL, scanner.getPosition(),
                             std::to_string('\\' + val));
        }
      }
    }
    // Otherwise we have a normal char.
    else if (scanner.peek() == '\'') {
      char val = c;
      scanner.next();
      return SourceToken(TC::CHAR_LITERAL, scanner.getPosition(),
                         std::string(1, val));
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
        return SourceToken(TC::CHAR, scanner.getPosition());
    }
    // Check for CONST Token.
    if (c == 'c' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("const");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::CONST, scanner.getPosition());
    }
    // Check for DO Token.
    if (c == 'd' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("do");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::DO, scanner.getPosition());
    }
    // Check for ELSE Token.
    else if (c == 'e' && scanner.peek() == 'l') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("else");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::ELSE, scanner.getPosition());
    }
    // Check for ENUM Token.
    else if (c == 'e' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("enum");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::ENUM, scanner.getPosition());
    }
    // Check for EXTERN Token.
    else if (c == 'e' && scanner.peek() == 'x') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("extern");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::EXTERN, scanner.getPosition());
    }
    // Check for IF Token.
    else if (c == 'i' && scanner.peek() == 'f') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("if");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::IF, scanner.getPosition());
    }
    // Check for INT token.
    else if (c == 'i' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("int");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::INT, scanner.getPosition());
    }
    // Check for RETURN Token.
    else if (c == 'r' && scanner.peek() == 'e') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("return");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::RETURN, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("short");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::SHORT, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'i') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("sizeof");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::SIZEOF, scanner.getPosition());
    }
    // Check for STRUCT Token.
    else if (c == 's' && scanner.peek() == 't') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("struct");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::STRUCT, scanner.getPosition());
    }
    // Check for TYPEDEF Token.
    else if (c == 't' && scanner.peek() == 'y') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("typedef");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::TYPEDEF, scanner.getPosition());
    }
    // Check for UNSIGNED Token.
    else if (c == 'u' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("unsigned");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::UNSIGNED, scanner.getPosition());
    }
    // Check for WHILE Token.
    else if (c == 'w' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("while");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::WHILE, scanner.getPosition());
    }
    // Check for VOID Token.
    else if (c == 'v' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("void");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::VOID, scanner.getPosition());
    }

    // No keyword Token has been returned.
    // Now Lex an IDENTIFIER.
    char peek;
    while (true) {
      peek = scanner.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (std::isspace(peek)) {
        return SourceToken(TC::IDENTIFIER, scanner.getPosition(), literal);
      }
      // If the next character is an illegal characater for an IDENTIFIER, we
      // have finished finding the token.
      if (!isalpha(peek) && !isdigit(peek) && peek != '_' && peek != '$') {
        return SourceToken(TC::IDENTIFIER, scanner.getPosition(), literal);
      }
      // We are still Lexing the token.
      c = scanner.next();
      literal += c;
    }
  }
  // Recognise INT_LITERAL Token.
  if (isdigit(c)) {
    std::string literal(1, c);
    if (scanner.peek() == 'x') {
      literal += scanner.next();
      while (std::isalpha(scanner.peek()) || std::isdigit(scanner.peek()))
        literal += scanner.next();
    } else {
      while (isdigit(scanner.peek()))
        literal += scanner.next();
    }

    return SourceToken(TC::INT_LITERAL, scanner.getPosition(), literal);
  }

  /* Recognise Two Symbol Tokens. */
  if (c == '=' && scanner.peek() == '=') {
    scanner.next();
    return SourceToken(TC::EQ, scanner.getPosition());
  }
  if (c == '!' && scanner.peek() == '=') {
    scanner.next();
    return SourceToken(TC::NE, scanner.getPosition());
  }
  if (c == '<' && scanner.peek() == '=') {
    scanner.next();
    return SourceToken(TC::LE, scanner.getPosition());
  }
  if (c == '>' && scanner.peek() == '=') {
    scanner.next();
    return SourceToken(TC::GE, scanner.getPosition());
  }
  if (c == '&' && scanner.peek() == '&') {
    scanner.next();
    return SourceToken(TC::AND, scanner.getPosition());
  }
  if (c == '|' && scanner.peek() == '|') {
    scanner.next();
    return SourceToken(TC::OR, scanner.getPosition());
  }

  /* Recognise One Symbol Tokens. */
  if (c == '=')
    return SourceToken(TC::ASSIGN, scanner.getPosition());
  if (c == '{')
    return SourceToken(TC::LBRA, scanner.getPosition());
  if (c == '}')
    return SourceToken(TC::RBRA, scanner.getPosition());
  if (c == '(')
    return SourceToken(TC::LPAR, scanner.getPosition());
  if (c == ')')
    return SourceToken(TC::RPAR, scanner.getPosition());
  if (c == '[')
    return SourceToken(TC::LSBR, scanner.getPosition());
  if (c == ']')
    return SourceToken(TC::RSBR, scanner.getPosition());
  if (c == ';')
    return SourceToken(TC::SC, scanner.getPosition());
  if (c == ',')
    return SourceToken(TC::COMMA, scanner.getPosition());
  if (c == '+')
    return SourceToken(TC::PLUS, scanner.getPosition());
  if (c == '-')
    return SourceToken(TC::MINUS, scanner.getPosition());
  if (c == '*')
    return SourceToken(TC::ASTERIX, scanner.getPosition());
  if (c == '%')
    return SourceToken(TC::REM, scanner.getPosition());
  if (c == '.')
    return SourceToken(TC::DOT, scanner.getPosition());
  if (c == '/')
    return SourceToken(TC::DIV, scanner.getPosition());
  if (c == '>')
    return SourceToken(TC::GT, scanner.getPosition());
  if (c == '<')
    return SourceToken(TC::LT, scanner.getPosition());
  if (c == '&')
    return SourceToken(TC::REF, scanner.getPosition());

  // Skip Whitespace.
  if (std::isspace(c))
    return nextToken();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error("Lexer: Unexpected Token. " +
                           scanner.getPosition().toString());
}

SourceToken Lexer::lexStringLiteral() {
  std::string literal;
  int currLine = scanner.getPosition().line;

  char c;
  while (true) {
    c = scanner.next();
    if (c == '\0')
      throw std::runtime_error("Lexer: Unexpected EOF in String Literal. " +
                               scanner.getPosition().toString());
    if (c == '\n')
      throw std::runtime_error(
          "Lexer: Unexpected Newline Character in String Literal. " +
          scanner.getPosition().toString());

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
  return SourceToken(TC::STRING_LITERAL, scanner.getPosition(), literal);
}

void Lexer::passComment() {
  // Consume the '/' or '*' character.
  char c = scanner.next();
  int currLine = scanner.getPosition().line;
  if (c == '/') {
    while (scanner.getPosition().line == currLine)
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
      "Lexer: Parsing Comment Returned Unexpected SourceToken(s). " +
      scanner.getPosition().toString());
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
