#include "ast/Program.h"

using namespace ACC;

Program::Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls)
    : decls(p_decls), globalScope(nullptr) {
  for (unsigned int idx = 0; idx < decls.size(); ++idx) {
    const atl::shared_ptr<Decl> decl = decls[idx];
    if (decl->astClass() == "FunDef" || decl->astClass() == "FunDecl") {
      funDecls.push_back(atl::static_pointer_cast<FunDecl>(decl));
    }
    if (decl->astClass() == "VarDecl") {
      globalVars.push_back(atl::static_pointer_cast<VarDecl>(decl));
    }
  }
}
