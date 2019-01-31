#pragma once

#ifndef ACC_TOKENISER_H
#define ACC_TOKENISER_H

#include "atl/include/string.h"
#include <utility>

#include "Scanner.h"
#include "SourceToken.h"

namespace ACC {

class Lexer final {

public:
  Lexer(Scanner &scanner);
  Lexer(Lexer &rhs) = delete;

  /* Parses the next Token out of the Input File. */
  SourceToken nextToken();

private:
  Scanner &scanner;

  SourceToken lexStringLiteral();
  void passComment();
  std::pair<bool, atl::string> tryLexKeyword(const atl::string &keyword);
};

}; // namespace ACC

#endif