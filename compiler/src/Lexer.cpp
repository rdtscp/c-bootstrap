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
          peek == 'f' || peek == '\'' || peek == '"' || peek == '\\' ||
          peek == '0') {
        c = scanner.next();
        if (scanner.peek() == '\'') {
          char val = c;
          scanner.next();
          return SourceToken(TC::CHAR_LITERAL, scanner.getPosition(),
                             atl::string(1, '\\') + val);
        }
      }
    }
    // Otherwise we have a normal char.
    else if (scanner.peek() == '\'') {
      char val = c;
      scanner.next();
      return SourceToken(TC::CHAR_LITERAL, scanner.getPosition(),
                         atl::string(1, val));
    }
  }
  // Recognise IDENTIFIERS & Keyword Tokens.
  if (isalpha(c) || c == '_') {
    atl::string literal(1, c);
    // Check for BOOL Token.
    if (c == 'b' && scanner.peek() == 'o') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("bool");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::BOOL, scanner.getPosition());
    }
    // Check for CHAR Token.
    else if (c == 'c' && scanner.peek() == 'h') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("char");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::CHAR, scanner.getPosition());
    }
    // Check for CLASS Token.
    else if (c == 'c' && scanner.peek() == 'l') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("class");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::CLASS, scanner.getPosition());
    }
    // Check for CONST Token.
    else if (c == 'c' && scanner.peek() == 'o') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("const");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::CONST, scanner.getPosition());
    }
    // Check for DELETE and DELETEARR Tokens.
    else if (c == 'd' && scanner.peek() == 'e') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("delete");
      literal = lexResult.second;

      if (lexResult.first) {
        if (scanner.peek() == '[') {
          scanner.next();
          if (scanner.peek() == ']') {
            scanner.next();
            return SourceToken(TC::DELETEARR, scanner.getPosition());
          } else {
            throw std::runtime_error(
                std::string(
                    "Lexer: Unexpected Token. Expected 'delete[]' at ") +
                scanner.getPosition().toString().c_str());
          }
        }
        return SourceToken(TC::DELETE, scanner.getPosition());
      }
    }
    // Check for DO Token.
    else if (c == 'd' && scanner.peek() == 'o') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("do");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::DO, scanner.getPosition());
    }
    // Check for ELSE Token.
    else if (c == 'e' && scanner.peek() == 'l') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("else");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::ELSE, scanner.getPosition());
    }
    // Check for ENUM Token.
    else if (c == 'e' && scanner.peek() == 'n') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("enum");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::ENUM, scanner.getPosition());
    }
    // Check for FALSE Token.
    else if (c == 'f' && scanner.peek() == 'a') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("false");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::FALSE_VAL, scanner.getPosition());
    } // Check for ENUM Token.
    else if (c == 'f' && scanner.peek() == 'o') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("for");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::FOR, scanner.getPosition());
    }
    // Check for IF Token.
    else if (c == 'i' && scanner.peek() == 'f') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("if");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::IF, scanner.getPosition());
    }
    // Check for INT Token.
    else if (c == 'i' && scanner.peek() == 'n') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("int");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::INT, scanner.getPosition());
    }
    // Check for NAMESPACE Token.
    else if (c == 'n' && scanner.peek() == 'a') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("namespace");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::NAMESPACE, scanner.getPosition());
    }
    // Check for NEW Token.
    else if (c == 'n' && scanner.peek() == 'e') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("new");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::NEW, scanner.getPosition());
    }
    // Check for operator overload Tokens.
    else if (c == 'o' && scanner.peek() == 'p') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("operator");
      literal = lexResult.second;

      if (lexResult.first) {
        c = scanner.next();
        if (c == '=' && scanner.peek() != '=') {
          return SourceToken(TC::OPASSIGN, scanner.getPosition(), "operator=");
        } else if (c == '=' && scanner.peek() == '=') {
          scanner.next();
          return SourceToken(TC::OPEQ, scanner.getPosition(), "operator==");
        } else if (c == '+' && scanner.peek() != '=') {
          return SourceToken(TC::OPADD, scanner.getPosition(), "operator+");
        } else if (c == '+' && scanner.peek() == '=') {
          scanner.next();
          return SourceToken(TC::OPADDTO, scanner.getPosition(), "operator+=");
        } else if (c == '[' && scanner.peek() == ']') {
          scanner.next();
          return SourceToken(TC::OPADDTO, scanner.getPosition(), "operator[]");
        } else if (c == '<' && scanner.peek() != '=') {
          return SourceToken(TC::OPLT, scanner.getPosition(), "operator<");
        } else if (c == '<' && scanner.peek() == '=') {
          scanner.next();
          return SourceToken(TC::OPLE, scanner.getPosition(), "operator<=");
        } else if (c == '>' && scanner.peek() != '=') {
          return SourceToken(TC::OPGT, scanner.getPosition(), "operator>");
        } else if (c == '>' && scanner.peek() == '=') {
          scanner.next();
          return SourceToken(TC::OPGE, scanner.getPosition(), "operator>=");
        } else if (c == '!' && scanner.peek() == '=') {
          scanner.next();
          return SourceToken(TC::OPNE, scanner.getPosition(), "operator!=");
        } else {
          atl::string error = atl::string("Could not Lex operator overload: ") +
                              scanner.getPosition().toString();
          throw std::runtime_error(error.c_str());
        }
      }
    }
    // Check for PRIVATE/PROTECTED Tokens.
    else if (c == 'p' && scanner.peek() == 'r') {
      c = scanner.next();
      literal += c;
      if (scanner.peek() == 'i') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("rivate:");
        literal = atl::string("p") + lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::PRIVATE, scanner.getPosition());
      } else if (scanner.peek() == 'o') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("otected:");
        literal = atl::string("p") + lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::PROTECTED, scanner.getPosition());
      }
    }
    // Check for PUBLIC Token.
    else if (c == 'p' && scanner.peek() == 'u') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("public:");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::PUBLIC, scanner.getPosition());
    }
    // Check for RETURN Token.
    else if (c == 'r' && scanner.peek() == 'e') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("return");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::RETURN, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'h') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("short");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::SHORT, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'i') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("sizeof");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::SIZEOF, scanner.getPosition());
    }
    // Check for STRUCT and STATIC Token.
    else if (c == 's' && scanner.peek() == 't') {
      c = scanner.next();
      literal += c;
      if (scanner.peek() == 'a') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("tatic");
        literal = lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::STATIC, scanner.getPosition());
      } else if (scanner.peek() == 'r') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("truct");
        literal = lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::STRUCT, scanner.getPosition());
      }
    }
    // Check for THIS and THROW Token.
    else if (c == 't' && scanner.peek() == 'h') {
      c = scanner.next();
      literal += c;
      if (scanner.peek() == 'i') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("his");
        literal = lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::THIS, scanner.getPosition());
      } else if (scanner.peek() == 'r') {
        atl::pair<bool, atl::string> lexResult = tryLexKeyword("hrow");
        literal = lexResult.second;

        if (lexResult.first)
          return SourceToken(TC::THROW, scanner.getPosition());
      }
    }
    // Check for TRUE Token.
    else if (c == 't' && scanner.peek() == 'r') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("true");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::TRUE_VAL, scanner.getPosition());
    }
    // Check for TYPEDEF Token.
    else if (c == 't' && scanner.peek() == 'y') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("typedef");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::TYPEDEF, scanner.getPosition());
    }
    // Check for UNSIGNED Token.
    else if (c == 'u' && scanner.peek() == 'n') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("unsigned int");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::UINT, scanner.getPosition());
    }
    // Check for WHILE Token.
    else if (c == 'w' && scanner.peek() == 'h') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("while");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::WHILE, scanner.getPosition());
    }
    // Check for VOID Token.
    else if (c == 'v' && scanner.peek() == 'o') {
      atl::pair<bool, atl::string> lexResult = tryLexKeyword("void");
      literal = lexResult.second;

      if (lexResult.first)
        return SourceToken(TC::VOID, scanner.getPosition());
    }

    // No keyword Token has been returned.
    // Now Lex an IDENTIFIER.
    while (true) {
      char peek = scanner.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (atl::isspace(peek)) {
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
    atl::string literal(1, c);
    if (scanner.peek() == 'x') {
      literal += scanner.next();
      while (atl::isalpha(scanner.peek()) || atl::isdigit(scanner.peek()))
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
  if (c == '+' && scanner.peek() == '=') {
    scanner.next();
    return SourceToken(TC::ASSIGNADD, scanner.getPosition());
  }
  if (c == '+' && scanner.peek() == '+') {
    scanner.next();
    return SourceToken(TC::PREFIXINC, scanner.getPosition());
  }
  if (c == '-' && scanner.peek() == '-') {
    scanner.next();
    return SourceToken(TC::PREFIXDEC, scanner.getPosition());
  }
  if (c == '-' && scanner.peek() == '>') {
    scanner.next();
    return SourceToken(TC::PTRDOT, scanner.getPosition());
  }

  /* Recognise One Symbol Tokens. */
  if (c == '~')
    return SourceToken(TC::DESTRUCTOR, scanner.getPosition());
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
  if (c == '+')
    return SourceToken(TC::PLUS, scanner.getPosition());
  if (c == '!')
    return SourceToken(TC::NOT, scanner.getPosition());
  if (c == '?')
    return SourceToken(TC::TERTIARYIF, scanner.getPosition());
  if (c == ':')
    return SourceToken(TC::TERTIARYELSE, scanner.getPosition());

  // Skip Whitespace.
  if (atl::isspace(c))
    return nextToken();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error(std::string("Lexer: Unexpected Token. ") +
                           scanner.getPosition().toString().c_str());
}

SourceToken Lexer::lexStringLiteral() {
  atl::string literal;
  int currLine = scanner.getPosition().line;

  while (true) {
    char c = scanner.next();
    if (c == '\0')
      throw std::runtime_error(
          std::string("Lexer: Unexpected EOF in String Literal. ") +
          scanner.getPosition().toString().c_str());
    if (c == '\n')
      throw std::runtime_error(
          std::string(
              "Lexer: Unexpected Newline Character in String Literal. ") +
          scanner.getPosition().toString().c_str());

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
      std::string(
          "Lexer: Lexing Comment Returned Unexpected SourceToken(s). ") +
      scanner.getPosition().toString().c_str());
}

atl::pair<bool, atl::string> Lexer::tryLexKeyword(const atl::string &keyword) {
  bool keywordMatch = false;
  atl::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); ++i) {
    if (scanner.peek() != keyword[i])
      return atl::pair<bool, atl::string>(false, literal);

    literal += scanner.next();
  }

  char peek = scanner.peek();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return atl::pair<bool, atl::string>(keywordMatch, literal);
}
