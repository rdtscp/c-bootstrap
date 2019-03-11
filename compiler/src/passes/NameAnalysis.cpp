#include "../../include/passes/NameAnalysis.h"

#include "atl/include/set.h"

using namespace ACC;

NameAnalysis::NameAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}

void NameAnalysis::error(const atl::string &error) {
  errorCount++;
  errors.push_back(error);
}

void NameAnalysis::printErrors() {
  printf("Name Analysis Errors:\n");
  for (int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void NameAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

void NameAnalysis::visit(AddressOf &ao) { ao.addressOfExpr->accept(*this); }
void NameAnalysis::visit(Allocation &a) {}
void NameAnalysis::visit(ArrayAccess &aa) { aa.array->accept(*this); }
void NameAnalysis::visit(ArrayType &at) {}
void NameAnalysis::visit(Assign &as) {
  as.lhs->accept(*this);
  as.rhs->accept(*this);
}
void NameAnalysis::visit(BaseType &bt) {}
void NameAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
}
void NameAnalysis::visit(Block &b) {
  if (b.outerBlock == nullptr) {
    b.setOuterBlock(currScope);
    currScope = b.getptr();
  }
  for (int idx = 0; idx < b.blockStmts.size(); ++idx)
    b.blockStmts[idx]->accept(*this);
  currScope = b.outerBlock;
}
void NameAnalysis::visit(CharLiteral &cl) {}
void NameAnalysis::visit(ClassType &ct) {}
void NameAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->findLocal(ctd.getIdentifier()))
    return error(
        atl::string("Attempted to declare a Class with an identifier that is "
                    "already in use: ") +
        ctd.getIdentifier());

  currScope->insertDecl(ctd.getptr());

  /* Check that the fields in this struct are unique */
  // atl::set<atl::string> structTypeFields;
  // for (int idx = 0; idx < ctd.varDecls.size(); ++idx) {
  //   const atl::shared_ptr<VarDecl> field = std.varDecls[idx];
  //   if (structTypeFields.find(field->getIdentifier()))
  //     return error(atl::string("Struct ") + std.getIdentifier() +
  //                  " contained multiple fields with the same identifier: " +
  //                  field->getIdentifier());
  //   structTypeFields.insert(field->identifer);
  // }
}
void NameAnalysis::visit(ConstructorDecl &cd) {}
void NameAnalysis::visit(ConstructorDef &cd) {}

void NameAnalysis::visit(Deletion &d) {}
void NameAnalysis::visit(DestructorDecl &dd) {}
void NameAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);
}
void NameAnalysis::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
}
void NameAnalysis::visit(EnumTypeDecl &etd) {}
void NameAnalysis::visit(For &f) {
  // TODO: Scope the entire for loop.
  f.initialVarDecl->accept(*this);
  f.condition->accept(*this);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);
}
void NameAnalysis::visit(FunCall &fc) {
  if (currScope->find(fc.funName) == nullptr)
    return error(atl::string("Attempted to call undeclared function: ") +
                 fc.funName);
  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);
}
void NameAnalysis::visit(FunDecl &fd) {
  // if (currScope->findLocal(fd.getIdentifier()))
  //   return error("Attempted to declare a Function with an identifier that is
  //   "
  //                "already in use: " +
  //                fd.getIdentifier());
  // currScope->insertDecl(fd.getptr());

  // fd.funBlock->setOuterBlock(currScope);
  // currScope = fd.funBlock;

  // for (const auto &param : fd.funParams)
  //   param->accept(*this);
  // fd.funBlock->accept(*this);
  // currScope = fd.funBlock->outerBlock;
}
void NameAnalysis::visit(FunDef &fd) {
  if (currScope->findLocal(fd.getIdentifier()))
    return error(
        atl::string(
            "Attempted to declare a Function with an identifier that is "
            "already in use: ") +
        fd.getIdentifier());
  currScope->insertDecl(fd.getptr());

  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  for (int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.funBlock->outerBlock;
}
void NameAnalysis::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
}
void NameAnalysis::visit(IntLiteral &il) {}
void NameAnalysis::visit(MemberAccess &ma) { ma.object->accept(*this); }
void NameAnalysis::visit(MemberCall &mc) {}

void NameAnalysis::visit(Namespace &n) {
  for (int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);
}
void NameAnalysis::visit(ParenthExpr &pe) { pe.innerExpr->accept(*this); }
void NameAnalysis::visit(PointerType &pt) {}
void NameAnalysis::visit(PrefixInc &pi) { pi.incrementVar->accept(*this); }
void NameAnalysis::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));
  for (int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);
  // /* Check for main() function */
  // atl::shared_ptr<Decl> mainDecl = currScope->find("main");
  // if (mainDecl == nullptr || mainDecl->astClass() != "FunDef")
  //   error("Program did not contain a main() Function.");
  p.globalScope = currScope;
}
void NameAnalysis::visit(ReferenceType &rt) {
  rt.referencedType->accept(*this);
}
void NameAnalysis::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
}
void NameAnalysis::visit(SizeOf &so) {}
void NameAnalysis::visit(StringLiteral &sl) {}
void NameAnalysis::visit(StructType &st) {}
void NameAnalysis::visit(StructTypeDecl &std) {
  if (currScope->findLocal(std.getIdentifier()))
    return error(
        atl::string("Attempted to declare a Struct with an identifier that is "
                    "already in use: ") +
        std.getIdentifier());

  currScope->insertDecl(std.getptr());

  /* Check that the fields in this struct are unique */
  atl::set<atl::string> structTypeFields;
  for (int idx = 0; idx < std.varDecls.size(); ++idx) {
    const atl::shared_ptr<VarDecl> field = std.varDecls[idx];
    if (structTypeFields.find(field->getIdentifier()))
      return error(atl::string("Struct ") + std.getIdentifier() +
                   " contained multiple fields with the same identifier: " +
                   field->getIdentifier());
    structTypeFields.insert(field->identifer);
  }
}
void NameAnalysis::visit(TypeCast &tc) { tc.expr->accept(*this); }
void NameAnalysis::visit(TypeDefDecl &td) {}
void NameAnalysis::visit(ValueAt &va) { va.derefExpr->accept(*this); }
void NameAnalysis::visit(VarDecl &vd) {
  if (currScope->findLocal(vd.getIdentifier()))
    return error(
        atl::string(
            "Attempted to declare a Variable with an identifier that is "
            "already in use: ") +
        vd.getIdentifier());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarDef &vd) {
  if (currScope->findLocal(vd.getIdentifier()))
    return error(
        atl::string(
            "Attempted to declare a Variable with an identifier that is "
            "already in use: ") +
        vd.getIdentifier());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarExpr &ve) {
  if (currScope->find(ve.identifier) == nullptr)
    return error(atl::string("Attempted to reference undeclared variable: ") +
                 ve.identifier);
}
void NameAnalysis::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
}