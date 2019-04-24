#pragma once

#include "Position.h"
#include <stdio.h>

namespace ACC {

class error {
public:
  error(const atl::string &message, const Position &position = Position()) {
    printf("\nerror in file: %s\n%s\n\n", position.toString().c_str(),
           message.c_str());
  }
};

} // namespace ACC