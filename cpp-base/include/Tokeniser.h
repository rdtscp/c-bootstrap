#pragma once

#include <string>

#include "Scanner.h"
#include "Token.h"

namespace ACC {

class Tokeniser {

public:
  Tokeniser(Scanner& scanner);

  ACC::Token nextToken();

private:
  Scanner scanner;
};

}; // namespace ACC
