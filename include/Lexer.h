#pragma once

#ifndef TOKENISER_H
#define TOKENISER_H

#include <string>
#include <utility>

#include "Scanner.h"
#include "Token.h"

namespace ACC {

class Lexer {

public:
  Lexer(const Scanner &scanner);

  /* Parses the next Token out of the Input File. */
  Token nextToken();

private:
  Scanner scanner;

  Token lexStringLiteral();
  void passComment();
  std::pair<bool, std::string> tryLexKeyword(const std::string& keyword);
};

}; // namespace ACC

#endif