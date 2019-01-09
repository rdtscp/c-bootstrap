#include <iostream>

#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(const SourceHandler &src) : scanner(src) {}

SourceHandler Preprocessor::preprocess(const SourceHandler &src) {}

void Preprocessor::passComment() {
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
  throw std::runtime_error("Preprocessing: Preprocessing Comment Returned "
                           "Unexpected SourceToken(s). " +
                           scanner.getPosition().toString());
}

std::pair<bool, std::string>
Preprocessor::tryLexKeyword(const std::string &keyword) {
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