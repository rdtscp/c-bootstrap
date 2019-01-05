#pragma once

#ifndef SOURCECODE_H
#define SOURCECODE_H

#include <stack>
#include <string>

#include "Position.h"

namespace ACC {

class SourceCode {

public:
  SourceCode(const std::string &src, const std::string &abspath);

  /* Returns the next character in the source. */
  char next();

  /* Returns one character ahead of next() in the source. */
  char peek();

  /* Returns the file:line:column of the current character in the source. */
  Position getPosition() const;

private:
  /* Position. */
  int currColumn;
  int currLine;
  std::stack<std::string> abspaths;

  /* Source Code */
  int currIdx;
  std::string src;

  std::string getFilename() const;
  std::string getFilepath() const;
  void preprocessorNote();
};

}; // namespace ACC

#endif