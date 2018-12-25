#include "../../include/passes/TypeAnalysis.h"

using namespace ACC;
TypeAnalysis::TypeAnalysis(std::shared_ptr<Program> progAST)
    : progAST(progAST) {}

std::shared_ptr<Type> TypeAnalysis::error(std::string error) {
  errorCount++;
  errors.push_back(error);
  return nullptr;
}

void TypeAnalysis::printErrors() {
  std::cerr << "Type Analysis Errors:" << std::endl;
  for (const auto &error : errors)
    std::cerr << "\t" << error << std::endl;
}

void TypeAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

std::shared_ptr<Type> TypeAnalysis::visit(ArrayAccess &aa) {
  std::shared_ptr<Type> arrayExprType = aa.array->accept(*this);
  std::shared_ptr<Type> arrayIndex = aa.index->accept(*this);
  if (arrayExprType->astClass() != "ArrayType")
    return error(
        "Type Error: Attempted to index an expression which was not an "
        "array. Was of type: " +
        arrayExprType->astClass());
  if (arrayIndex->astClass() != "BaseType")
    return error(
        "Type Error: Attempted to index an array using an expression which "
        "was not of type int. Was of type: " +
        arrayIndex->astClass());

  return std::static_pointer_cast<ArrayType>(arrayExprType)->arrayType;
}
std::shared_ptr<Type> TypeAnalysis::visit(ArrayType &at) {
  return std::make_shared<ArrayType>(at);
}
std::shared_ptr<Type> TypeAnalysis::visit(Assign &as) {
  std::shared_ptr<Type> lhs = as.lhs->accept(*this);
  std::shared_ptr<Type> rhs = as.rhs->accept(*this);
  if (*lhs != *rhs)
    return error("Assignation has mismatched types.");
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(BaseType &bt) {
  return std::make_shared<BaseType>(bt);
}
std::shared_ptr<Type> TypeAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
  return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
std::shared_ptr<Type> TypeAnalysis::visit(Block &b) {
  if (b.outerBlock == nullptr) {
    b.setOuterBlock(currScope);
    currScope = b.getptr();
  }
  for (const auto &stmt : b.blockStmts)
    stmt->accept(*this);
  currScope = b.outerBlock;
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(CharLiteral &cl) {
  return std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
}
std::shared_ptr<Type> TypeAnalysis::visit(FieldAccess &fa) {
  std::shared_ptr<Type> objType = fa.object->accept(*this);
  if (objType->astClass() != "StructType")
    return error("Type Analysis: Attempted to access field on expression "
                 "that is not a struct");

  std::shared_ptr<StructType> structType =
      std::static_pointer_cast<StructType>(objType);
  std::shared_ptr<Decl> identDecl = currScope->find(structType->identifier);
  if (identDecl == nullptr)
    return error("Type Analysis: Attempted to access field on expression "
                 "that does not have a type definition.");
  if (identDecl->astClass() != "StructTypeDecl")
    return error("Type Analysis: Attempted to access field on expression "
                 "that does not have a StructTypeDecl");

  std::shared_ptr<StructTypeDecl> structTypeDecl =
      std::static_pointer_cast<StructTypeDecl>(identDecl);

  for (const auto &field : structTypeDecl->varDecls)
    if (field->identifer == fa.field)
      return field->type;

  return error("Type Analysis: Attempted to access field on a struct that "
               "does not exist.");
}
std::shared_ptr<Type> TypeAnalysis::visit(FunCall &fc) {
  std::shared_ptr<Decl> identDecl = currScope->find(fc.funName);
  if (identDecl == nullptr)
    return error("Type Analysis: Attempted to call undeclared function: " +
                 fc.funName);

  if (identDecl->astClass() != "FunDecl")
    return error("Type Analysis: Attempted to call undeclared function: " +
                 fc.funName);

  std::shared_ptr<FunDecl> funDecl =
      std::static_pointer_cast<FunDecl>(identDecl);

  if (funDecl->funParams.size() != fc.funArgs.size())
    return error("Type Analysis: Attempted to call function: " + fc.funName +
                 " with incorrect number of arguments");

  for (int i = 0; i < fc.funArgs.size(); i++) {
    std::shared_ptr<Type> argType = fc.funArgs[i]->accept(*this);
    std::shared_ptr<Type> paramType = funDecl->funParams[i]->type;
    if (*argType != *paramType)
      return error("Type Analysis: Attempted to call function: " + fc.funName +
                   " with arguments of incorrect type.");
  }
  return funDecl->funType;
}
std::shared_ptr<Type> TypeAnalysis::visit(FunDecl &fd) {
  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  for (const auto &param : fd.funParams)
    param->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.funBlock->outerBlock;
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(If &i) {
  std::shared_ptr<Type> condType = i.ifCondition->accept(*this);

  if (condType->astClass() != "BaseType")
    return error("Type Analysis: Type of While condition is invalid.");

  std::shared_ptr<BaseType> condBaseType =
      std::static_pointer_cast<BaseType>(condType);

  if (condBaseType->primitiveType != PrimitiveType::INT)
    return error("Type Analysis: Type of While condition is not INT.");

  i.ifBody->accept(*this);

  if (i.elseBody)
    i.elseBody->accept(*this);
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(IntLiteral &il) {
  return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
std::shared_ptr<Type> TypeAnalysis::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
std::shared_ptr<Type> TypeAnalysis::visit(PointerType &pt) {
  return std::make_shared<PointerType>(pt);
}
std::shared_ptr<Type> TypeAnalysis::visit(Program &p) {
  currScope = p.globalScope;
  for (const std::shared_ptr<Decl> &decl : p.decls)
    decl->accept(*this);
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(Return &r) {
  if (r.returnExpr)
    return r.returnExpr->accept(*this);
  return std::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  ;
}
std::shared_ptr<Type> TypeAnalysis::visit(SizeOf &so) {
  return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
std::shared_ptr<Type> TypeAnalysis::visit(StringLiteral &sl) { return nullptr; }
std::shared_ptr<Type> TypeAnalysis::visit(StructType &st) {
  std::shared_ptr<Decl> findDecl = currScope->find(st.identifier);
  if (findDecl->astClass() != "StructTypeDecl")
    return error("Attempted to use a StructType that was not declared.");

  st.typeDefinition = std::static_pointer_cast<StructTypeDecl>(findDecl);
  return std::make_shared<StructType>(st);
}
std::shared_ptr<Type> TypeAnalysis::visit(StructTypeDecl &std) {
  return nullptr;
}
std::shared_ptr<Type> TypeAnalysis::visit(TypeCast &tc) { return tc.type; }
std::shared_ptr<Type> TypeAnalysis::visit(ValueAt &va) {
  std::shared_ptr<Type> exprType = va.derefExpr->accept(*this);
  if (exprType->astClass() != "PointerType")
    return error("Attempted to dereference variable that wasn't a pointer. "
                 "Was of type: " +
                 exprType->astClass());
  return std::static_pointer_cast<PointerType>(exprType)->pointedType;
}
std::shared_ptr<Type> TypeAnalysis::visit(VarDecl &vd) { return nullptr; }
std::shared_ptr<Type> TypeAnalysis::visit(VarExpr &ve) {
  std::shared_ptr<Decl> identDecl = currScope->find(ve.identifier);

  if (identDecl->astClass() != "VarDecl")
    return error("Attempted to reference " + identDecl->astClass() +
                 " as a variable.");
  std::shared_ptr<VarDecl> veDecl =
      std::static_pointer_cast<VarDecl>(identDecl);
  ve.variableDecl = veDecl;
  return veDecl->type;
}
std::shared_ptr<Type> TypeAnalysis::visit(While &w) {
  std::shared_ptr<Type> conditionType = w.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis: Type of While condition is invalid.");
  std::shared_ptr<BaseType> conditionBaseType =
      std::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT)
    return error("Type Analysis: Type of While condition is not INT.");
  w.body->accept(*this);
  return nullptr;
}
