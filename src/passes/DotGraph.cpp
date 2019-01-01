#include "../../include/passes/DotGraph.h"

using namespace ACC;

DotGraph::DotGraph(std::shared_ptr<Program> progAST) : progAST(progAST) {}
void DotGraph::print() { visit(*progAST); }

void DotGraph::declare(const std::string &nodeID, const std::string &label) {
  put(nodeID + " [label=\"" + label + "\"];");
}

void DotGraph::join(const std::string &lhs, const std::string &rhs) {
  std::cerr << lhs << " -> " << rhs << std::endl;
}

void DotGraph::put(const std::string &str) { std::cout << str << std::endl; }

/* ---- Visit AST ---- */

std::string DotGraph::visit(ArrayAccess &aa) {
  std::string arrayAccessID = "ArrayAccess" + std::to_string(nodeCount++);
  declare(arrayAccessID, "ArrayAccess");

  std::string arrayID = aa.array->accept(*this);
  std::string indexID = aa.index->accept(*this);

  join(arrayAccessID, arrayID);
  join(arrayAccessID, indexID);

  return arrayAccessID;
}
std::string DotGraph::visit(ArrayType &at) {
  return at.arrayType->accept(*this) + "[" + at.arraySize + "]";
}
std::string DotGraph::visit(Assign &as) {
  std::string assignID = "Assign" + std::to_string(nodeCount++);
  declare(assignID, "=");

  std::string lhsID = as.lhs->accept(*this);
  std::string rhsID = as.rhs->accept(*this);

  join(assignID, lhsID);
  join(assignID, rhsID);

  return assignID;
}
std::string DotGraph::visit(BaseType &bt) {
  switch (bt.primitiveType) {
  case PrimitiveType::CHAR:
    return "char";
  case PrimitiveType::INT:
    return "int";
  case PrimitiveType::VOID:
    return "void";
  }
}
std::string DotGraph::visit(BinOp &bo) {
  std::string binOpID = "BinOp" + std::to_string(nodeCount++);
  declare(binOpID, "BinOp");

  std::string lhsId = bo.lhs->accept(*this);

  std::string OpID = "Op" + std::to_string(nodeCount++);
  switch (bo.operation) {
  case Op::ADD:
    declare(OpID, "+");
    break;
  case Op::AND:
    declare(OpID, "&&");
    break;
  case Op::DIV:
    declare(OpID, "/");
    break;
  case Op::EQ:
    declare(OpID, "==");
    break;
  case Op::GE:
    declare(OpID, ">=");
    break;
  case Op::GT:
    declare(OpID, ">");
    break;
  case Op::LE:
    declare(OpID, "<=");
    break;
  case Op::LT:
    declare(OpID, "<");
    break;
  case Op::MOD:
    declare(OpID, "%");
    break;
  case Op::MUL:
    declare(OpID, "*");
    break;
  case Op::NE:
    declare(OpID, "!=");
    break;
  case Op::OR:
    declare(OpID, "||");
    break;
  case Op::SUB:
    declare(OpID, "-");
    break;
  }

  std::string rhsId = bo.rhs->accept(*this);

  join(binOpID, lhsId);
  join(binOpID, OpID);
  join(binOpID, rhsId);

  return binOpID;
}
std::string DotGraph::visit(Block &b) {
  std::string blockID = "Block" + std::to_string(nodeCount++);
  declare(blockID, "{}");
  for (const auto stmt : b.blockStmts)
    join(blockID, stmt->accept(*this));
  return blockID;
}
std::string DotGraph::visit(CharLiteral &cl) {
  std::string charID = "CharLiteral" + std::to_string(nodeCount++);
  declare(charID, "'" + cl.value + "'");
  return charID;
}
std::string DotGraph::visit(DoWhile &dw) {
  std::string whileID = "While" + std::to_string(nodeCount++);
  declare(whileID, "do {} while()");
  join(whileID, dw.body->accept(*this));
  join(whileID, dw.condition->accept(*this));
  return whileID;
}
std::string DotGraph::visit(FieldAccess &fa) {
  std::string fieldAccessID = "FieldAccess" + std::to_string(nodeCount++);
  declare(fieldAccessID, "FieldAccess." + fa.field);

  std::string objID = fa.object->accept(*this);

  join(fieldAccessID, objID);

  return fieldAccessID;
}
std::string DotGraph::visit(FunCall &fc) {
  std::string funCallID = "FunCall" + std::to_string(nodeCount++);
  declare(funCallID, fc.funName + "()");

  for (const auto arg : fc.funArgs)
    join(funCallID, arg->accept(*this));

  return funCallID;
}
std::string DotGraph::visit(FunDecl &fd) {
  std::string funcID = "FunDecl" + std::to_string(nodeCount++);
  declare(funcID, fd.funName);
  join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
std::string DotGraph::visit(FunDef &fd) {
  std::string funcID = "FunDecl" + std::to_string(nodeCount++);
  declare(funcID, fd.funName);
  join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
std::string DotGraph::visit(If &i) {
  std::string ifID = "If" + std::to_string(nodeCount++);
  declare(ifID, "if then else");

  join(ifID, i.ifCondition->accept(*this));
  join(ifID, i.ifBody->accept(*this));
  if (i.elseBody != nullptr) {
    join(ifID, i.elseBody->accept(*this));
  }

  return ifID;
}
std::string DotGraph::visit(IntLiteral &il) {
  std::string intID = "IntLiteral" + std::to_string(nodeCount++);
  declare(intID, il.value);
  return intID;
}
std::string DotGraph::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
std::string DotGraph::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
std::string DotGraph::visit(Program &p) {
  std::cout << "digraph prog {" << std::endl;
  for (const auto decl : p.decls) {
    decl->accept(*this);
  }
  std::cout << "}" << std::endl;
  return "Node0";
}
std::string DotGraph::visit(Return &r) {
  std::string returnID = "Return" + std::to_string(nodeCount++);
  declare(returnID, "return");

  if (r.returnExpr != nullptr) {
    join(returnID, r.returnExpr->accept(*this));
  }

  return returnID;
}
std::string DotGraph::visit(SizeOf &so) {
  std::string returnID = "SizeOf" + std::to_string(nodeCount++);
  declare(returnID, "sizeof(" + so.type->accept(*this) + ")");

  return returnID;
}
std::string DotGraph::visit(StringLiteral &sl) {
  std::string strID = "StringLiteral" + std::to_string(nodeCount++);
  declare(strID, "\\\"" + sl.value + "\\\"");
  return strID;
}
std::string DotGraph::visit(StructType &st) {
  return "struct " + st.identifier;
}
std::string DotGraph::visit(StructTypeDecl &std) {
  std::string structTypeDeclID = "StructTypeDecl" + std::to_string(nodeCount++);
  declare(structTypeDeclID, std.structType->accept(*this) + " = {}");
  for (const auto field : std.varDecls)
    join(structTypeDeclID, field->accept(*this));
  return structTypeDeclID;
}
std::string DotGraph::visit(TypeCast &tc) {
  std::string typecastID = "TypeCast" + std::to_string(nodeCount++);
  declare(typecastID, "(" + tc.type->accept(*this) + ")");
  join(typecastID, tc.expr->accept(*this));
  return typecastID;
}
std::string DotGraph::visit(ValueAt &va) {
  std::string derefID = "Deref" + std::to_string(nodeCount++);
  declare(derefID, "\"Deref\"");
  join(derefID, va.derefExpr->accept(*this));
  return derefID;
}
std::string DotGraph::visit(VarDecl &vd) {
  nodeCount++;
  std::string varDeclID = "VarDecl" + std::to_string(nodeCount++);
  declare(varDeclID, vd.type->accept(*this) + " " + vd.identifer + ";");
  return varDeclID;
}
std::string DotGraph::visit(VarExpr &ve) {
  std::string varID = "VarExpr" + std::to_string(nodeCount++);
  declare(varID, ve.identifier);
  return varID;
}
std::string DotGraph::visit(While &w) {
  std::string whileID = "While" + std::to_string(nodeCount++);
  declare(whileID, "while()");
  join(whileID, w.condition->accept(*this));
  join(whileID, w.body->accept(*this));
  return whileID;
}