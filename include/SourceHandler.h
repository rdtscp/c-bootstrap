#pragma once

#include "atl/include/string.h"

namespace ACC {

class SourceHandler {
public:
  enum class Type { FILEPATH, RAW };
  SourceHandler(const Type type, const atl::string &value)
      : type(type), value(value) {}
  const Type type;
  atl::string value;

  atl::string getFilepath() const {
    if (type == Type::FILEPATH)
      return value;
    else
      return "RAW";
  }
};

} // namespace ACC
