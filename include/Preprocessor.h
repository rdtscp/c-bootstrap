#pragma once

#include "atl/include/pair.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/stack.h"
#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "Scanner.h"
#include "SourceHandler.h"

namespace ACC {

class PPScanner : Scanner {
public:
  PPScanner(const SourceHandler &src);

  char next() override;
};

class Preprocessor {
public:
  Preprocessor(const SourceHandler &src,
               const atl::vector<atl::string> &includePaths);

  SourceHandler getSource();

private:
  const atl::vector<atl::string> &includePaths;
  atl::stack<atl::shared_ptr<PPScanner>> fileScanners;
};

} // namespace ACC