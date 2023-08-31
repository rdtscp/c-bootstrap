#pragma once

#include "atl/include/unordered_map.h"

#include "ASTNode.h"
#include "ClassTypeDef.h"
#include "FunDef.h"
#include "Namespace.h"
#include "VarDef.h"

namespace ACC {

class Program : public ASTNode,
                public Scope,
                public atl::enable_shared_from_this<Program> {

public:
  unsigned int declsChecked = 0;
  atl::vector<atl::shared_ptr<Decl>> decls;
  atl::vector<atl::shared_ptr<FunDecl>> funDecls;
  atl::vector<atl::shared_ptr<VarDecl>> globalVars;

  Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls);

  bool operator==(const Program &rhs) const;
  bool operator!=(const Program &rhs) const;

  atl::shared_ptr<Program> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Program"; }

  SCOPE_OVERRIDES

  VISITOR_ACCEPTORS
};
} // namespace ACC
