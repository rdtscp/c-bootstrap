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
  Preprocessor(
      const SourceHandler &src, const atl::vector<atl::string> &includePaths,
      const atl::shared_ptr<Preprocessor> parentPreprocessor = nullptr);

  Preprocessor(const SourceHandler &&src,
               const atl::vector<atl::string> &includePaths,
               const atl::shared_ptr<Preprocessor> parentPreprocessor) = delete;
  Preprocessor(const Preprocessor &rhs) = delete;
  Preprocessor(Preprocessor &&rhs) = delete;
  Preprocessor &operator=(const Preprocessor &rhs) = delete;
  Preprocessor &operator=(Preprocessor &&rhs) = delete;

  SourceHandler getSource();

  static atl::string formatIncludeDirective(const atl::string &filepath,
                                            const int lineNum = 1) {
    const atl::string lineNum_s = atl::to_string(lineNum);
    return "# " + lineNum_s + " \"" + filepath + "\"";
  }

  static bool fileExists(const atl::string &filepath) {
    return std::ifstream(filepath.c_str()).good();
  }

private:
  const atl::vector<atl::string> includePaths;
  const SourceHandler &src;
  const atl::shared_ptr<Preprocessor> parentPreprocessor;
  atl::shared_ptr<PPScanner> scanner;
  atl::vector<atl::string> filesPreprocessed;

  SourceHandler lexInclude();
  bool lexPragmaOnce();

  /* Helpers */
  void lexKeyword(const atl::string &keyword);
  atl::string lexStringLiteral();
  bool checkVisited(const atl::string &filepath) const;
  void markVisited(const atl::string &filepath);
  void passComment();
};

} // namespace ACC