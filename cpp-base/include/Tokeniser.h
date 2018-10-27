#include <string>

#include "Scanner.h"
#include "Token.h"

namespace ACC {

class Tokeniser {

public:
  Tokeniser(const Scanner &scanner);

  ACC::Token next();

private:
  Scanner scanner;
};

}; // namespace ACC
