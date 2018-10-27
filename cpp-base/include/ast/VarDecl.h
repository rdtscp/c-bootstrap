#pragma once

#include "Type.h"

namespace ACC {

class VarDecl {

public:
  Type type;
  std::string identifer;
  int numBytes;
  ACC::FunDecl parentFunc;
  int fpOffset;
  int spOffset;

public:
  VarDecl(Type p_type, std::string p_identifer)
      : type(p_type), identifer(p_identifer), fpOffset(0) {}
};

}; // namespace ACC
