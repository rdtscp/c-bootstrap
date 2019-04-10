#pragma once

#include "atl/include/pair.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

#include "Scanner.h"
#include "SourceHandler.h"

namespace ACC {

class Preprocessor : public Scanner {
public:
  Preprocessor(const SourceHandler &src);

  SourceHandler getSource();

private:
  SourceHandler src;
};

} // namespace ACC