#include "ast/Block.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/VarDef.h"

using namespace ACC;

Block::Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts)
    : stmts(p_stmts) {}

bool Block::operator==(const Block &rhs) const {
  if (stmts.size() != rhs.stmts.size())
    return false;

  for (unsigned int i = 0; i < stmts.size(); ++i)
    /* TODO: Implement comparitors for Stmts. */
    if (stmts[i]->astClass() != rhs.stmts[i]->astClass())
      return false;

  return true;
}

bool Block::operator!=(const Block &rhs) const { return !(*this == rhs); }

atl::shared_ptr<ClassTypeDecl>
Block::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                     const atl::shared_ptr<Decl> &exemptDecl) const {
  return outerScope->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
Block::findClassDef(const atl::shared_ptr<Identifier> identifier,
                    const atl::shared_ptr<Decl> &exemptDecl) const {
  return outerScope->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
Block::findFunDecl(const FunSignature &funSignature,
                   const atl::shared_ptr<Decl> &exemptDecl) const {
  return outerScope->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
Block::findFunDeclLocal(const FunSignature &funSignature,
                        const atl::shared_ptr<Decl> &exemptDecl) const {
  return nullptr;
}

atl::shared_ptr<VarDecl>
Block::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> &exemptDecl) const {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findVarDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
Block::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                        const atl::shared_ptr<Decl> &exemptDecl) const {
  for (int idx = stmtsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Stmt> currStmt = stmts[idx];
    if (currStmt->astClass() != "VarDecl" && currStmt->astClass() != "VarDef")
      continue;
    const atl::shared_ptr<VarDecl> currVarDecl =
        atl::static_pointer_cast<VarDecl>(currStmt);
    if (currVarDecl.get() == exemptDecl.get())
      continue;
    if (*currVarDecl->getIdentifier() != *identifier)
      continue;

    return currVarDecl;
  }

  return nullptr;
}