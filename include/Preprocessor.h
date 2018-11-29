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

private:
  std::set<std::string> definitions;
  Scanner &scanner;
};

}; // namespace ACC

#endif