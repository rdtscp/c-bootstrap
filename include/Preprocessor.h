#pragma once

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <memory>
#include <set>
#include <string>

#include "Scanner.h"

namespace ACC {

class Preprocessor {

public:
  Preprocessor(Scanner &scanner);

  void preprocessDefinition(const std::string &definition);

  void preprocessIfNDef(const std::string &definition);

  void preprocessInclude(const bool localFile, const std::string &filename);

  void preprocessPragmaOnce(const std::string &filename);

  void preprocessUndef(const std::string &definition);

private:
  std::set<std::string> definitions;
  std::set<std::string> files;
  Scanner &scanner;
};

}; // namespace ACC

#endif