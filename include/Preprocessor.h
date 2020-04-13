#pragma once

#include "atl/include/ifstream.h"
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
  Preprocessor(
      const SourceHandler &src, const atl::vector<atl::string> &includePaths,
      Preprocessor *parentPreprocessor = nullptr);

  Preprocessor(const SourceHandler &&src,
               const atl::vector<atl::string> &includePaths,
               Preprocessor *parentPreprocessor) = delete;
  Preprocessor(const Preprocessor &rhs) = delete;
  Preprocessor(Preprocessor &&rhs) = delete;
  Preprocessor &operator=(const Preprocessor &rhs) = delete;
  Preprocessor &operator=(Preprocessor &&rhs) = delete;

  SourceHandler getSource();

  static atl::string formatIncludeDirective(const atl::string &filepath,
                                            const int lineNum = 0) {
    const atl::string lineNum_s = atl::to_string(lineNum);
    return "# " + lineNum_s + " \"" + filepath + "\"";
  }

  static bool fileExists(const atl::string &filepath) {
    return atl::ifstream(filepath).good();
  }

private:
  const atl::vector<atl::string> includePaths;
  const SourceHandler &src;
  // TODO: Use `observer_ptr` instead of raw pointer here.
  Preprocessor* parentPreprocessor;
  atl::shared_ptr<PPScanner> scanner;
  atl::vector<atl::string> filesPreprocessed;

  SourceHandler lexInclude();
  bool lexPragmaOnce();

  /* Helpers */
  void lexKeyword(const atl::string &keyword);
  atl::string lexStringLiteral();
  bool checkVisited(const atl::string &filepath) const;
  void markVisited(const atl::string &filepath);
  int passComment();
};

} // namespace ACC
