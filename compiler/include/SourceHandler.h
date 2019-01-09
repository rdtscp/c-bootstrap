#pragma once

#ifndef ACC_SOURCEHANDLER_H
#define ACC_SOURCEHANDLER_H

#include <string>

namespace ACC {

class SourceHandler {
public:
  enum class Type { FILEPATH, RAW };
  SourceHandler(const Type type, const std::string &value)
      : type(type), value(value) {}
  const Type type;
  const std::string &value;
};

}; // namespace ACC

#endif