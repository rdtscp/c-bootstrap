#pragma once

#ifndef ACC_PREPROCESSOR_H
#define ACC_PREPROCESSOR_H

#include <string>
#include <utility>

#include "Scanner.h"
#include "SourceHandler.h"

namespace ACC {

class Preprocessor final {

public:
  Preprocessor(const SourceHandler &src);

  Preprocessor(Preprocessor &rhs) = delete;

  SourceHandler preprocess(const SourceHandler &src);

private:
  Scanner scanner;

  void passComment();
  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

class PPNode {
public:
  virtual ~PPNode() {}
};

class

}; // namespace ACC

#endif