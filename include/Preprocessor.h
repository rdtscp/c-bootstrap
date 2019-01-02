#pragma once

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include "Scanner.h"

namespace ACC {

class Preprocessor {

public:
  Preprocessor(Scanner &scanner);

  void preprocessDefinition(const std::string &definition,
                            const std::string &value);

  void preprocessIf(const std::string &condition);

  void preprocessElse();

  void preprocessEndif();

  void preprocessIfDef(const std::string &definition);

  void preprocessIfNDef(const std::string &definition);

  void preprocessInclude(const bool localFile, const std::string &filename);

  void preprocessPragmaOnce(const std::string &filename);

  void preprocessUndef(const std::string &definition);

private:
  /* Bool marks if we should include preprocessing within the if. */
  std::vector<std::pair<std::string, bool>> ifs;
  std::map<std::string, std::string> definitions;
  std::set<std::string> files;
  Scanner &scanner;

  void checkChar(char c);

  bool evalCondition(const std::string &condition);

  std::pair<bool, std::string> getNextIfDirective();

  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

}; // namespace ACC

#endif