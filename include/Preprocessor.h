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
  std::vector<std::shared_ptr<Scanner>> scanners;

  std::set<std::string> files;
  std::stack<std::pair<std::string, bool>> ifs;
  std::map<PPFuncSig, std::string> funcDefinitions;
  std::map<std::string, std::string> varDefinitions;

  std::string currFilename() const;
  std::string currFilepath() const;
  int currLine() const;
  char nextChar();
  char peekChar();

  /* ---- Functionality ---- */
  bool evalIfCondition(const std::string &condition);

  void preprocessDefine();
  void preprocessElif();
  void preprocessElse();
  void preprocessEndif();
  void preprocessIf();
  void preprocessIfdef();
  void preprocessIfndef();
  void preprocessInclude();
  void preprocessPragma();
  void preprocessUndef();

  /* ---- Helpers --- */
  void checkChar(char c) const;

  void addDefinition(const std::string &definition, const std::string &value);
  std::pair<bool, std::string> getNextIfDirective();
  std::string getStackPosition() const;
  bool isCppHeader(const std::string &filename) const;
  std::string parseCondition();
  void passComment();
  void preprocessIfCondition(const std::string &condition);
  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

}; // namespace ACC

#endif