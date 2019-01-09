#pragma once

#ifndef ACC_SOURCEHANDLER_H
#define ACC_SOURCEHANDLER_H

#include <string>

namespace ACC {

class SourceHandler {
public:
  enum class SourceType { FILEPATH, RAW };
  SourceHandler(const SourceType type, const std::string &value)
      : type(type), value(value) {}
  const SourceType type;
  const std::string &value;
};

}; // namespace ACC

#endif