#include "../../include/passes/DotGraph.h"

using namespace ACC;

DotGraph::DotGraph(std::shared_ptr<Program> progAST) : progAST(progAST) {}
void DotGraph::print() { visit(*progAST); }

void DotGraph::declare(const atl::string &nodeID, const atl::string &label) {
  put(nodeID + " [label=\"" + label + "\"];");
}

void DotGraph::join(const atl::string &lhs, const atl::string &rhs) {
  std::cout << lhs.c_str() << " -> " << rhs.c_str() << std::endl;
}

void DotGraph::put(const atl::string &str) {
  std::cout << str.c_str() << std::endl;
}

/* ---- Visit AST ---- */

atl::string DotGraph::visit(ArrayAccess &aa) {
  atl::string arrayAccessID =
      atl::string("ArrayAccess") + std::to_string(nodeCount++).c_str();
  declare(arrayAccessID, "ArrayAccess");

  atl::string arrayID = aa.array->accept(*this);
  atl::string indexID = aa.index->accept(*this);

  join(arrayAccessID, arrayID);
  join(arrayAccessID, indexID);

  return arrayAccessID;
}
atl::string DotGraph::visit(ArrayType &at) {
  return at.arrayType->accept(*this) + "[" + at.arraySize.c_str() + "]";
}
atl::string DotGraph::visit(Assign &as) {
  atl::string assignID =
      atl::string("Assign") + std::to_string(nodeCount++).c_str();
  declare(assignID, "=");

  atl::string lhsID = as.lhs->accept(*this);
  atl::string rhsID = as.rhs->accept(*this);

  join(assignID, lhsID);
  join(assignID, rhsID);

  return assignID;
}
atl::string DotGraph::visit(BaseType &bt) {
  switch (bt.primitiveType) {
  case PrimitiveType::CHAR:
    return "char";
  case PrimitiveType::INT:
    return "int";
  case PrimitiveType::SHORT:
    return "short";
  case PrimitiveType::VOID:
    return "void";
  }
}
atl::string DotGraph::visit(BinOp &bo) {
  atl::string binOpID =
      atl::string("BinOp") + std::to_string(nodeCount++).c_str();
  declare(binOpID, "BinOp");

  atl::string lhsId = bo.lhs->accept(*this);

  atl::string OpID = atl::string("Op") + std::to_string(nodeCount++).c_str();
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

  atl::string rhsId = bo.rhs->accept(*this);

  join(binOpID, lhsId);
  join(binOpID, OpID);
  join(binOpID, rhsId);

  return binOpID;
}
atl::string DotGraph::visit(Block &b) {
  atl::string blockID =
      atl::string("Block") + std::to_string(nodeCount++).c_str();
  declare(blockID, "{}");
  for (const auto stmt : b.blockStmts)
    join(blockID, stmt->accept(*this));
  return blockID;
}
atl::string DotGraph::visit(CharLiteral &cl) {
  atl::string charID =
      atl::string("CharLiteral") + std::to_string(nodeCount++).c_str();
  declare(charID, atl::string("'") + cl.getLiteral() + "'");
  return charID;
}
atl::string DotGraph::visit(DoWhile &dw) {
  atl::string whileID =
      atl::string("While") + std::to_string(nodeCount++).c_str();
  declare(whileID, "do {} while()");
  join(whileID, dw.body->accept(*this));
  join(whileID, dw.condition->accept(*this));
  return whileID;
}
atl::string DotGraph::visit(EnumTypeDecl &etd) { return ""; }
atl::string DotGraph::visit(FieldAccess &fa) {
  atl::string fieldAccessID =
      atl::string("FieldAccess") + std::to_string(nodeCount++).c_str();
  declare(fieldAccessID, atl::string("FieldAccess.") + fa.field.c_str());

  atl::string objID = fa.object->accept(*this);

  join(fieldAccessID, objID);

  return fieldAccessID;
}
atl::string DotGraph::visit(FunCall &fc) {
  atl::string funCallID =
      atl::string("FunCall") + std::to_string(nodeCount++).c_str();
  declare(funCallID, atl::string(fc.funName.c_str()) + "()");

  for (const auto arg : fc.funArgs)
    join(funCallID, arg->accept(*this));

  return funCallID;
}
atl::string DotGraph::visit(FunDecl &fd) {
  atl::string funcID =
      atl::string("FunDecl") + std::to_string(nodeCount++).c_str();
  // declare(funcID, fd.funName);
  // join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
atl::string DotGraph::visit(FunDef &fd) {
  atl::string funcID =
      atl::string("FunDecl") + std::to_string(nodeCount++).c_str();
  declare(funcID, fd.funName);
  join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
atl::string DotGraph::visit(If &i) {
  atl::string ifID = atl::string("If") + std::to_string(nodeCount++).c_str();
  declare(ifID, "if then else");

  join(ifID, i.ifCondition->accept(*this));
  join(ifID, i.ifBody->accept(*this));
  if (i.elseBody != nullptr) {
    join(ifID, i.elseBody->accept(*this));
  }

  return ifID;
}
atl::string DotGraph::visit(IntLiteral &il) {
  atl::string intID =
      atl::string("IntLiteral") + std::to_string(nodeCount++).c_str();
  declare(intID, il.getLiteral());
  return intID;
}
atl::string DotGraph::visit(Namespace &n) {
  return n.namespaceBlock->accept(*this);
}
atl::string DotGraph::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::string DotGraph::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
atl::string DotGraph::visit(Program &p) {
  std::cout << "digraph prog {" << std::endl;
  for (const auto decl : p.decls) {
    decl->accept(*this);
  }
  std::cout << "}" << std::endl;
  return "Node0";
}
atl::string DotGraph::visit(Return &r) {
  atl::string returnID =
      atl::string("Return") + std::to_string(nodeCount++).c_str();
  declare(returnID, "return");

  if (r.returnExpr != nullptr) {
    join(returnID, r.returnExpr->accept(*this));
  }

  return returnID;
}
atl::string DotGraph::visit(SizeOf &so) {
  atl::string returnID =
      atl::string("SizeOf") + std::to_string(nodeCount++).c_str();
  declare(returnID, atl::string("sizeof(") + so.type->accept(*this) + ")");

  return returnID;
}
atl::string DotGraph::visit(StringLiteral &sl) {
  atl::string strID =
      atl::string("StringLiteral") + std::to_string(nodeCount++).c_str();
  declare(strID, atl::string("\\\"") + sl.getLiteral() + "\\\"");
  return strID;
}
atl::string DotGraph::visit(StructType &st) {
  return atl::string("struct ") + st.identifier;
}
atl::string DotGraph::visit(StructTypeDecl &std) {
  atl::string structTypeDeclID =
      atl::string("StructTypeDecl") + std::to_string(nodeCount++).c_str();
  declare(structTypeDeclID, std.structType->accept(*this) + " = {}");
  for (const auto field : std.varDecls)
    join(structTypeDeclID, field->accept(*this));
  return structTypeDeclID;
}
atl::string DotGraph::visit(TypeCast &tc) {
  atl::string typecastID =
      atl::string("TypeCast") + std::to_string(nodeCount++).c_str();
  declare(typecastID, atl::string("(") + tc.type->accept(*this) + ")");
  join(typecastID, tc.expr->accept(*this));
  return typecastID;
}
atl::string DotGraph::visit(TypeDefDecl &td) { return ""; }
atl::string DotGraph::visit(ValueAt &va) {
  atl::string derefID =
      atl::string("Deref") + std::to_string(nodeCount++).c_str();
  declare(derefID, "\"Deref\"");
  join(derefID, va.derefExpr->accept(*this));
  return derefID;
}
atl::string DotGraph::visit(VarDecl &vd) {
  nodeCount++;
  atl::string varDeclID =
      atl::string("VarDecl") + std::to_string(nodeCount++).c_str();
  declare(varDeclID, vd.type->accept(*this) + " " + vd.identifer + ";");
  return varDeclID;
}
atl::string DotGraph::visit(VarExpr &ve) {
  atl::string varID =
      atl::string("VarExpr") + std::to_string(nodeCount++).c_str();
  declare(varID, ve.identifier.c_str());
  return varID;
}
atl::string DotGraph::visit(While &w) {
  atl::string whileID =
      atl::string("While") + std::to_string(nodeCount++).c_str();
  declare(whileID, "while()");
  join(whileID, w.condition->accept(*this));
  join(whileID, w.body->accept(*this));
  return whileID;
}