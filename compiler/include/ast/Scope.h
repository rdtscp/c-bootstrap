#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/vector.h"

namespace ACC {
class Decl;

class Scope {
public:
  atl::vector<atl::shared_ptr<Decl>> decls;
  atl::shared_ptr<Scope> outerScope;

  Scope() : decls({}), outerScope(nullptr) {}
  ~Scope() {}

  void insertDecl(const atl::shared_ptr<Decl> &decl) { decls.push_back(decl); }
  atl::shared_ptr<Decl> find(const atl::shared_ptr<Identifier> &identifier) {
    atl::shared_ptr<Decl> local = findLocal(identifier);
    if (local == nullptr && outerScope != nullptr)
      return outerScope->find(identifier);

    return local;
  }
  atl::shared_ptr<Decl>
  findLocal(const atl::shared_ptr<Identifier> &identifier) {
    // Reverse iterate decls.
    for (int idx = decls.size() - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> &currDecl = decls[idx];
      if (*identifier == *currDecl->getIdentifier())
        return decls[idx];
    }

    return nullptr;
  }
};
} // namespace ACC
