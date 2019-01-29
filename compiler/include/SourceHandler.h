#pragma once

#ifndef ACC_SOURCEHANDLER_H
#define ACC_SOURCEHANDLER_H

#include "atl/include/string.h"

namespace ACC {

class SourceHandler {
public:
  enum class Type { FILEPATH, RAW };
  SourceHandler(const Type type, const atl::string &value)
      : type(type), value(value) {}
  const Type type;
  const atl::string &value;
};

}; // namespace ACC

#endif