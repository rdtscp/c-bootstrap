#include <cctype>
#include <fstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include "Token.h"
#include "Tokeniser.h"

ACC::Tokeniser::Tokeniser(Scanner &scanner) : scanner(scanner) {}

ACC::Token ACC::Tokeniser::next() {
  // Get the next Char.
  char c = scanner.next();

  // if ((int)c == -1)
  return ACC::Token(ACC::Token::TokenClass::ENDOFFILE, scanner.line,
                    scanner.column);

  // Parse DIV token taking into account comments.
  if (c == '/') {
    char peek = scanner.peek();
    // We have a regular line comment.
    if (peek == '/') {
      // Move into the comment content.
      c = scanner.next();
      int currLine = scanner.line;
      // Loop until we find the end of the comment.
      while (scanner.line == currLine) {
        c = scanner.next();
      }
    }
    // We have a block comment.
    else if (peek == '*') {
      // Move into the comment content.
      c = scanner.next();
      // Loop until we find the end of the comment.
      while (true) {
        c = scanner.next();
        peek = scanner.peek();
        if (c == '*' && peek == '/') {
          scanner.next();
          break;
        }
      }
      c = scanner.next();
    } else
      return ACC::Token(ACC::Token::TokenClass::DIV, scanner.line,
                        scanner.column);
  }

  // Recognise STRING_LITERAL token. @TODO, multiple lines fix.
  if (c == '"') {
    // We are now expecting any set of characters, terminated by a single ".
    // With care taken for escaped characters.
    std::string literal;
    int currLine = scanner.line;
    while (true) {
      c = scanner.next();
      if ((int)c == -1) {
        throw std::runtime_error("Unexpected EOF at Line " + scanner.line +
                                 ", Column " + scanner.column);
      }
      // If hit new-line before STRING_LITERAL terminator, we have invalid
      // token.
      if (scanner.line != currLine) {
        throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                                 ", Column " + scanner.column);
      }
      // Check if we are about to see an escaped character.
      if (c == '\\') {
        literal += c;
        c = scanner.next();
        literal += c;
      }
      // End of string.
      else if (c == '"') {
        break;
      } else {
        literal += c;
      }
    }
    return ACC::Token(ACC::Token::TokenClass::STRING_LITERAL, scanner.line,
                      scanner.column, literal);
  }

  // Recognise IDENTIFIER/keywords/types token.
  if (isalpha(c) || c == '_') {
    /* Alphabetically go throuh possible keywords/types before assuming
     * IDENTIFIER. */
    // Create a StringBuilder in case of IDENTIFIER.
    std::string literal;
    literal += c;
    // Check for CHAR token.
    if (c == 'c') {
      std::string expected = "har"; // "char" but with first char removed since
                                    // that has already been checked.
      char peek;                    // Peek into next char of stream.
      char expt;                    // What we expect the peek to be.
      bool isChar = true;           // Track if this is a CHAR token.
      for (int i = 0; i < expected.length(); i++) {
        // Peek the next char, and the expected char.
        peek = scanner.peek();
        expt = expected[i];
        if (peek != expt) {
          isChar = false; // This is not a CHAR token.
          break;
        }
        // It is potentially part of a CHAR token, save this char.
        c = scanner.next();
        literal += c;
      }
      peek = scanner.peek();
      if (isChar && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
        return ACC::Token(ACC::Token::TokenClass::CHAR, scanner.line,
                          scanner.column);
      }
    }
    // Check for ELSE token.
    else if (c == 'e') {
      std::string expected = "lse"; // "else" but with first char removed since
                                    // that has already been checked.
      // Flag to track if this stream of characters is an ELSE token.
      bool isElse = true;
      char expt_c;
      char peek;
      for (int i = 0; i < expected.length(); i++) {
        // Get the next char, and the expected char.
        peek = scanner.peek();
        expt_c = expected[i];
        if (peek != expt_c) {
          // This is not an ELSE token.
          isElse = false;
          break;
        }
        c = scanner.next();
        literal += c;
      }
      peek = scanner.peek();
      if (isElse && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
        return ACC::Token(ACC::Token::TokenClass::ELSE, scanner.line,
                          scanner.column);
      }
    }
    // Check for IF/INT token.
    else if (c == 'i') {
      // Check for IF token.
      if (scanner.peek() == 'f') {
        c = scanner.next();
        literal += c;
        char peek = scanner.peek();
        // If the char following IF is valid for an IF statement, return the
        // token.
        if ((!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
          return ACC::Token(ACC::Token::TokenClass::IF, scanner.line,
                            scanner.column);
        }
      }
      // Check for INT token.
      else {
        std::string expected = "nt"; // "int" but with first char removed since
                                     // that has already been checked.
        // Flag to track if this stream of characters is an INT token.
        bool isInt = true;
        char expt_c;
        char peek;
        for (int i = 0; i < expected.length(); i++) {
          // Get the next char, and the expected char.
          peek = scanner.peek();
          expt_c = expected[i];
          if (peek != expt_c) {
            // This is not an ELSE token.
            isInt = false;
            break;
          }
          c = scanner.next();
          literal += c;
        }
        peek = scanner.peek();
        if (isInt && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
          return ACC::Token(ACC::Token::TokenClass::INT, scanner.line,
                            scanner.column);
        }
      }
    }
    // Check for RETURN token.
    else if (c == 'r') {
      std::string expected = "eturn"; // "return" but with first char removed
                                      // since that has already been checked.
      // Flag to track if this stream of characters is a RETURN token.
      bool isReturn = true;
      char expt_c;
      char peek;
      for (int i = 0; i < expected.length(); i++) {
        // Get the next char, and the expected char.
        peek = scanner.peek();
        expt_c = expected[i];
        if (peek != expt_c) {
          // This is not a RETURN token.
          isReturn = false;
          break;
        }
        c = scanner.next();
        literal += c;
      }
      peek = scanner.peek();
      if (isReturn && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
        return ACC::Token(ACC::Token::TokenClass::RETURN, scanner.line,
                          scanner.column);
      }
    }
    // Check for SIZEOF/STRUCT token.
    else if (c == 's') {
      // Check for SIZEOF token.
      if (scanner.peek() == 'i') {
        std::string expected = "izeof"; // "sizeof" but with first char removed
                                        // since that has already been checked.
        // Flag to track if this stream of characters is an SIZEOF token.
        bool isSizeof = true;
        char expt_c;
        char peek;
        for (int i = 0; i < expected.length(); i++) {
          // Get the next char, and the expected char.
          peek = scanner.peek();
          expt_c = expected[i];
          if (peek != expt_c) {
            // This is not an SIZEOF token.
            isSizeof = false;
            break;
          }
          c = scanner.next();
          literal += c;
        }
        peek = scanner.peek();
        if (isSizeof && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
          return ACC::Token(ACC::Token::TokenClass::SIZEOF, scanner.line,
                            scanner.column);
        }
      }
      // Check for STRUCT token.
      else {
        std::string expected = "truct"; // "struct" but with first char removed
                                        // since that has already been checked.
        // Flag to track if this stream of characters is an STRUCT token.
        bool isStruct = true;
        char expt_c;
        char peek;
        for (int i = 0; i < expected.length(); i++) {
          // Get the next char, and the expected char.
          peek = scanner.peek();
          expt_c = expected[i];
          if (peek != expt_c) {
            // This is not a STRUCT token.
            isStruct = false;
            break;
          }
          c = scanner.next();
          literal += c;
        }
        peek = scanner.peek();
        if (isStruct && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
                  return ACC::Token(ACC::Token::TokenClass::STRUCT, scanner.line,scanner.column));
        }
      }
    }
    // Check for WHILE token.
    else if (c == 'w') {
      std::string expected = "hile"; // "while" but with first char removed
                                     // since that has already been checked.
      // Flag to track if this stream of characters is a WHILE token.
      bool isWhile = true;
      char expt_c;
      char peek;
      for (int i = 0; i < expected.length(); i++) {
        // Get the next char, and the expected char.
        peek = scanner.peek();
        expt_c = expected.[i];
        if (peek != expt_c) {
          // This is not a WHILE token.
          isWhile = false;
          break;
        }
        c = scanner.next();
        literal += c;
      }
      peek = scanner.peek();
      if (isWhile && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
        return ACC::Token(ACC::Token::TokenClass::WHILE, scanner.line,
                          scanner.column);
      }
      c = scanner.next();
      literal += c;
    }
    // Check for VOID token.
    else if (c == 'v') {
      std::string expected = "oid"; // "void" but with first char removed since
                                    // that has already been checked.
      // Flag to track if this stream of characters is a VOID token.
      bool isVoid = true;
      char expt_c;
      char peek;
      for (int i = 0; i < expected.length(); i++) {
        // Get the next char, and the expected char.
        peek = scanner.peek();
        expt_c = expected[i];
        if (peek != expt_c) {
          // This is not a VOID token.
          isVoid = false;
          break;
        }
        c = scanner.next();
        literal += c;
      }
      peek = scanner.peek();
      if (isVoid && (!isalpha(peek)) && (!isdigit(peek)) && (peek != '_')) {
        return ACC::Token(ACC::Token::TokenClass::VOID, scanner.line,
                          scanner.column);
      }
    }

    // If we have reached here, no token has been returned.
    // Now Lex an IDENTIFIER, where c is the last character in sb.
    char peek;
    while (true) {
      peek = scanner.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (std::isspace(peek)) {
        return ACC::Token(ACC::Token::TokenClass::IDENTIFIER, scanner.line,
                          scanner.column, literal);
      }
      // If the next character is an illegal characater for an IDENTIFIER, we
      // have finished finding the token.
      if (!isalpha(peek) && !isdigit(peek) && peek != '_') {
        return ACC::Token(ACC::Token::TokenClass::IDENTIFIER, scanner.line,
                          scanner.column, literal);
      }
      // We are still Lexing the token.
      c = scanner.next();
      literal += c;
    }
  }

  // Recognise INT_LITERAL token.
  if (isdigit(c)) {
    std::string literal;
    literal += c;
    char peek;
    while (true) {
      peek = scanner.peek();
      // Check that next char is a digit.
      if (!isdigit(peek)) {
        return ACC::Token(ACC::Token::TokenClass::INT_LITERAL, scanner.line,
                          scanner.column, literal);
      }
      c = scanner.next();
      literal += c;
    }
  }

  // Recognise ASSIGN/EQ tokens.
  if (c == '=') {
    // EQ Token.
    if (scanner.peek() == '=') {
      scanner.next();
      return ACC::Token(ACC::Token::TokenClass::EQ, scanner.line,
                        scanner.column);
    }
    // ASSIGN Token.
    else
      return ACC::Token(ACC::Token::TokenClass::ASSIGN, scanner.line,
                        scanner.column);
  }
  // Recognise NE token.
  if (c == '!') {
    if (scanner.peek() == '=') {
      c = scanner.next();
      return ACC::Token(ACC::Token::TokenClass::NE, scanner.line,
                        scanner.column);
    } else {
      return ACC::Token(ACC::Token::TokenClass::INVALID, scanner.line,
                        scanner.column);
    }
  }
  // Recognise CHAR_LITERAL token.
  if (c == '\'') {
    c = scanner.next();
    char peek = scanner.peek();
    // Check for escape character: '\t' | '\b' | '\n' | '\r' | '\f' | '\'' |
    // '\"' | '\\'
    if (c == '\\') {
      std::string value = "\\";
      // Our valid set of escaped characters.
      if (peek == 't' || peek == 'b' || peek == 'n' || peek == 'r' ||
          peek == 'f' || peek == '\'' || peek == '"' || peek == '\\') {
        c = scanner.next();
        value += c;
        peek = scanner.peek();
        // Next character must be a closing single quote to be a valid
        // CHAR_LITERAL.
        if (peek == '\'') {
          std::string ltr = "\\" + c;
          c = scanner.next();
          return ACC::Token(ACC::Token::TokenClass::CHAR_LITERAL, scanner.line,
                            scanner.column, ltr);
        } else {
          throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                                   ", Column " + scanner.column);
        }
      } else {
        throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                                 ", Column " + scanner.column);
      }
    }
    // Otherwise we have a normal char.
    else {
      // Check the CHAR_LITERAL is closed correctly.
      if (peek == '\'') {
        char val = c;
        c = scanner.next();
        return ACC::Token(ACC::Token::TokenClass::CHAR_LITERAL, scanner.line,
                          scanner.column, std::string("" + val));
      } else {
        throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                                 ", Column " + scanner.column);
      }
    }
  }
  // Recognise LT/LE tokens.
  if (c == '<') {
    if (scanner.peek() == '=') {
      c = scanner.next();
      return ACC::Token(ACC::Token::TokenClass::LE, scanner.line,
                        scanner.column);
    } else
      return ACC::Token(ACC::Token::TokenClass::LT, scanner.line,
                        scanner.column);
  }
  // Recognise GT/GE tokens.
  if (c == '>') {
    if (scanner.peek() == '=') {
      c = scanner.next();
      return ACC::Token(ACC::Token::TokenClass::GE, scanner.line,
                        scanner.column);
    }
    return ACC::Token(ACC::Token::TokenClass::GT, scanner.line, scanner.column);
  }
  // Recognise AND token.
  if (c == '&') {
    if (scanner.peek() == '&') {
      c = scanner.next();
      return ACC::Token(ACC::Token::TokenClass::AND, scanner.line,
                        scanner.column);
    } else {
      throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                               ", Column " + scanner.column);
    }
  }
  // Recognise OR token.
  if (c == '|') {
    if (scanner.peek() == '|') {
      c = scanner.next();
      return ACC::Token(ACC::Token::TokenClass::OR, scanner.line,
                        scanner.column);
    } else {
      throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                               ", Column " + scanner.column);
    }
  }
  // Recognise INCLUDE token.
  if (c == '#') {
    // The only valid characters that can proceed a '#'' are "include"
    std::string expected = "include";
    char expt_c;
    for (int i = 0; i < expected.length(); i++) {
      // Get the current and expected char.
      c = scanner.next();
      expt_c = expected[i];
      // If the current character is not expected.
      if (c != expt_c) {
        throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                                 ", Column " + scanner.column);
      }
    }
    // We have found "#include".
    return ACC::Token(ACC::Token::TokenClass::INCLUDE, scanner.line,
                      scanner.column);
  }

  /* Recognise simple tokens. */
  if (c == '{')
    return ACC::Token(ACC::Token::TokenClass::LBRA, scanner.line,
                      scanner.column);
  if (c == '}')
    return ACC::Token(ACC::Token::TokenClass::RBRA, scanner.line,
                      scanner.column);
  if (c == '(')
    return ACC::Token(ACC::Token::TokenClass::LPAR, scanner.line,
                      scanner.column);
  if (c == ')')
    return ACC::Token(ACC::Token::TokenClass::RPAR, scanner.line,
                      scanner.column);
  if (c == '[')
    return ACC::Token(ACC::Token::TokenClass::LSBR, scanner.line,
                      scanner.column);
  if (c == ']')
    return ACC::Token(ACC::Token::TokenClass::RSBR, scanner.line,
                      scanner.column);
  if (c == ';')
    return ACC::Token(ACC::Token::TokenClass::SC, scanner.line, scanner.column);
  if (c == ',')
    return ACC::Token(ACC::Token::TokenClass::COMMA, scanner.line,
                      scanner.column);
  if (c == '+')
    return ACC::Token(ACC::Token::TokenClass::PLUS, scanner.line,
                      scanner.column);
  if (c == '-')
    return ACC::Token(ACC::Token::TokenClass::MINUS, scanner.line,
                      scanner.column);
  if (c == '*')
    return ACC::Token(ACC::Token::TokenClass::ASTERIX, scanner.line,
                      scanner.column);
  if (c == '%')
    return ACC::Token(ACC::Token::TokenClass::REM, scanner.line,
                      scanner.column);
  if (c == '.')
    return ACC::Token(ACC::Token::TokenClass::DOT, scanner.line,
                      scanner.column);

  // Skip Whitespace.
  if (std::isspace(c))
    return next();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error("Unexpected Token at Line " + scanner.line +
                           ", Column " + scanner.column);
}