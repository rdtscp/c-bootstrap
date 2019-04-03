#pragma once

#include <map>

#include "ASTNode.h"
#include "ast/FunDecl.h"
#include "ast/Namespace.h"
#include "ast/StructTypeDecl.h"
#include "ast/VarDecl.h"

namespace ACC {

class Program : public ASTNode {

public:
  atl::vector<atl::shared_ptr<Decl>> decls;
  atl::vector<atl::shared_ptr<FunDecl>> funDecls;
  atl::vector<atl::shared_ptr<VarDecl>> globalVars;
  atl::shared_ptr<Block> globalScope;
  std::map<std::string, Namespace> namespaces;

  Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls);

  atl::string astClass() const override { return "Program"; }

  VISITOR_ACCEPTORS
};
} // namespace ACC
