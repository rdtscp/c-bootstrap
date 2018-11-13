#pragma once

#ifndef VARDECL_H
#define VARDECL_H

#include <memory>

namespace ACC {

class FunDecl;

class VarDecl {

public:
  std::shared_ptr<Type> type;
  std::string identifer;
  int numBytes;
  std::shared_ptr<FunDecl> parentFunc;
  int fpOffset;
  int spOffset;

  VarDecl(std::shared_ptr<Type> type_ptr, std::string p_identifer)
      : identifer(p_identifer), numBytes(0), fpOffset(0), spOffset(0) {
    type = type_ptr;
  }

  bool operator==(const VarDecl &vd) const {
    bool typesEqual = (*type == *vd.type);
    bool namesEqual = (identifer == vd.identifer);
    bool bytesEqual = (numBytes == vd.numBytes);
    bool parentEqual = true; // @TODO Implement FunDecl Comparisons.
    bool fpEqual = (fpOffset == vd.fpOffset);
    bool spEqual = (spOffset == vd.spOffset);

    return typesEqual && namesEqual && bytesEqual && parentEqual && fpEqual &&
           spEqual;
  }
};

}; // namespace ACC

#endif