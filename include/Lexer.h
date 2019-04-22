#pragma once

#include "atl/include/pair.h"
#include "atl/include/string.h"

#include "Scanner.h"
#include "SourceToken.h"

namespace ACC {

class Lexer final {

public:
  Lexer(Scanner &scanner);
  Lexer(const Lexer &rhs) = delete;
  Lexer(Lexer &&rhs) = delete;
  Lexer &operator=(const Lexer &rhs) = delete;
  Lexer &operator=(Lexer &&rhs) = delete;

  /* Parses the next Token out of the Input File. */
  SourceToken nextToken();

private:
  Scanner &scanner;

  SourceToken lexStringLiteral();
  void passComment();
  atl::pair<bool, atl::string> tryLexKeyword(const atl::string &keyword);
};

} // namespace ACC
