#pragma once

#include <memory>

#include "Type.h"

namespace ACC {

class VarDecl {

public:
  std::shared_ptr<Type> type;
  std::string identifer;
  int numBytes;
  FunDecl parentFunc;
  int fpOffset;
  int spOffset;

  VarDecl(std::shared_ptr<Type> type_ptr, std::string p_identifer)
      : identifer(p_identifer), fpOffset(0) {
    type = type_ptr;
  }
  
};

}; // namespace ACC
