#pragma once

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <map>
#include <set>
#include <stack>
#include <string>

#include "Scanner.h"
#include "SourceCode.h"

namespace ACC {

class Preprocessor {

  typedef std::pair<std::string, std::vector<std::string>> PPFuncSig;

public:
  Preprocessor(const std::string &abspath);

  /* Preprocesses a c++ source file, removing all directives.
   * Inserts custom directives of syntax:
   *    #startFile=/abs/path/to/file
   *    #endFile
   * To inform SourceCode which file it is in.
   */
  SourceCode getSource();

private:
  std::string src;
  const std::string abspath;
  std::stack<std::shared_ptr<Scanner>> scanners;

  std::set<std::string> files;
  std::vector<std::pair<std::string, bool>> ifs;
  std::map<PPFuncSig, std::string> funcDefinitions;
  std::map<std::string, std::string> varDefinitions;

  char nextChar();
  char peekChar();

  /* ---- Functionality ---- */
  bool evalIfCondition(const std::string &condition);

  void preprocessDefine();
  void preprocessInclude();
  void preprocessPragma();
  void preprocessIf();
  void preprocessElif();
  void preprocessElse();
  void preprocessEndif();
  void preprocessIfdef();
  void preprocessIfndef();
  void preprocessUndef();

  /* ---- Helpers --- */
  void checkChar(char c);
  void addDefinition(const std::string &definition, const std::string &value);
  std::pair<bool, std::string> getNextIfDirective();
  void passComment();
  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

}; // namespace ACC

#endif