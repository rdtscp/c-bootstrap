#include "../../include/ast/Program.h"

using namespace ACC;

Program::Program(const std::vector<std::shared_ptr<Decl>> &decls)
    : decls(decls) {
  for (const std::shared_ptr<Decl> decl : decls) {
    if (decl->astClass() == "FunDef" || decl->astClass() == "FunDecl") {
      funDecls.push_back(std::static_pointer_cast<FunDecl>(decl));
    }
    if (decl->astClass() == "VarDecl") {
      globalVars.push_back(std::static_pointer_cast<VarDecl>(decl));
    }
  }
}

void Program::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Program::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> Program::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Program::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
Program::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
Program::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Program::astClass() const { return "Program"; }