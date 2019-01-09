#pragma once

#ifndef ACC_PREPROCESSOR_H
#define ACC_PREPROCESSOR_H

#include <string>
#include <utility>

#include "Scanner.h"

namespace ACC {

class Preprocessor final {

public:
  Preprocessor(Scanner &src);
  Preprocessor(Preprocessor &rhs) = delete;

  /* Preprocess the file, and return it as a std::string. */
  std::string preprocess();

  /* Preprocess the file, and write the result to the filepath. */
  void preprocess(const std::string &filepath);

private:
  Scanner &scanner;

  void passComment();
  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

}; // namespace ACC

#endif