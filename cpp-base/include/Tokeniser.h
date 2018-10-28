#pragma once

#include <string>

#include "Scanner.h"
#include "Token.h"

namespace ACC {

class Tokeniser {

public:
  Tokeniser(const Scanner &scanner);

  /* Parses the next Token out of the Input File. */
  Token nextToken();

private:
  Scanner scanner;
};

}; // namespace ACC
