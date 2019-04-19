#pragma once

#include <fstream>

#include "atl/include/pair.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/stack.h"
#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "Scanner.h"
#include "SourceHandler.h"

namespace ACC {

class PPScanner : public Scanner {
public:
  PPScanner(const SourceHandler &src);

  char next() override;
};

class Preprocessor {
public:
  Preprocessor(const atl::vector<atl::string> &includePaths);

  SourceHandler getSource(const SourceHandler &src);

  static atl::string formatIncludeDirective(const atl::string &filepath,
                                            const int lineNum = 1) {
    const atl::string lineNum_s = atl::to_string(lineNum);
    return "# " + lineNum_s + " \"" + filepath + "\"";
  }

  static bool fileExists(const atl::string &filepath) {
    return std::ifstream(filepath.c_str()).good();
  }

private:
  const atl::vector<atl::string> &includePaths;
  atl::shared_ptr<PPScanner> scanner;

  atl::string lexInclude();

  /* Helpers */
  void lexKeyword(const atl::string &keyword);
  atl::string lexStringLiteral();
};

} // namespace ACC