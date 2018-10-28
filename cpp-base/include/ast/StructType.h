#pragma once

#include <memory>
#include <string>

#include "Type.h"

namespace ACC {

class StructType : public Type {

public:
  std::string identifier;

  StructType(const std::string &identifier) : identifier(identifier) {}
};

}; // namespace ACC
