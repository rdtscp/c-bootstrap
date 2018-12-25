#include "../../include/ast/Program.h"

using namespace ACC;

Program::Program(const std::vector<std::shared_ptr<Decl>> &decls)
    : decls(decls) {
  for (const std::shared_ptr<Decl> decl : decls) {
    if (decl->astClass() == "FunDecl") {
      funDecls.push_back(std::static_pointer_cast<FunDecl>(decl));
    }
    if (decl->astClass() == "VarDecl") {
      globalVars.push_back(std::static_pointer_cast<VarDecl>(decl));
    }
  }
}

void Program::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string Program::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> Program::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Program::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

X86::Register Program::accept(ASTVisitor<X86::Register> &v) {
  return v.visit(*this);
}

std::string Program::astClass() const { return "Program"; }