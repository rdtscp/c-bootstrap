#pragma once

#include "Position.h"
#include <stdio.h>

namespace ACC {

class Error {
public:
  Error(const atl::string &message, const Position &position = Position()) {
    if (position.file != "")
      printf("\nError in file: %s", position.toString().c_str());
    printf("\n%s\n\n", message.c_str());
  }
};

} // namespace ACC
