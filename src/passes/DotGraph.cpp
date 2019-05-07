#include "passes/DotGraph.h"

using namespace ACC;

DotGraph::DotGraph(atl::shared_ptr<Program> progAST) : progAST(progAST) {}
void DotGraph::print() { visit(*progAST); }

void DotGraph::declare(const atl::string &nodeID, const atl::string &label) {
  put(nodeID + " [label=\"" + label + "\"];");
}

void DotGraph::join(const atl::string &lhs, const atl::string &rhs) {
  atl::string output = lhs + " -> " + rhs;
  printf("%s\n", output.c_str());
}

void DotGraph::put(const atl::string &str) { printf("%s\n", str.c_str()); }

/* ---- Visit AST ---- */

atl::string DotGraph::visit(AddressOf &ao) {
  const atl::string addressOfID = "AddressOf" + atl::to_string(++nodeCount);
  declare(addressOfID, "AddressOf");
  join(addressOfID, ao.addressOfExpr->accept(*this));
  return addressOfID;
}
atl::string DotGraph::visit(Allocation &a) {
  const atl::string allocationID = "Allocation" + atl::to_string(++nodeCount);
  declare(allocationID, "new");
  atl::string allocationExprID;
  if (a.varConstructorCall) {
    allocationExprID = a.varConstructorCall->accept(*this);
    join(allocationID, allocationExprID);
    return allocationID;
  } else {
    const atl::string allocationTypeID =
        "AllocationType" + atl::to_string(++nodeCount);
    join(allocationID, a.varType->accept(*this));
    return allocationID;
  }
}
atl::string DotGraph::visit(ArrayAccess &aa) {
  const atl::string arrayAccessID = "ArrayAccess" + atl::to_string(++nodeCount);
  declare(arrayAccessID, "ArrayAccess");

  const atl::string arrayID = aa.array->accept(*this);
  const atl::string indexID = aa.index->accept(*this);

  join(arrayAccessID, arrayID);
  join(arrayAccessID, indexID);

  return arrayAccessID;
}
atl::string DotGraph::visit(ArrayType &at) {
  const atl::string arrayTypeID = "ArrayType" + atl::to_string(++nodeCount);
  declare(arrayTypeID, at.type->accept(*this) + "[]");
  const atl::string arraySize = at.size->accept(*this);
  join(arrayTypeID, arraySize);
  return arrayTypeID;
}
atl::string DotGraph::visit(Assign &as) {
  const atl::string assignID = "Assign" + atl::to_string(++nodeCount);
  declare(assignID, "=");

  const atl::string lhsID = as.lhs->accept(*this);
  const atl::string rhsID = as.rhs->accept(*this);

  join(assignID, lhsID);
  join(assignID, rhsID);

  return assignID;
}
atl::string DotGraph::visit(BaseType &bt) {
  atl::string output;
  switch (bt.primitiveType) {
  case PrimitiveType::CHAR:
    output = "char";
    break;
  case PrimitiveType::INT:
    output = "int";
    break;
  case PrimitiveType::SHORT:
    output = "short";
    break;
  case PrimitiveType::VOID:
    output = "void";
    break;
  case PrimitiveType::UINT:
    output = "unsigned int";
    break;
  case PrimitiveType::BOOL:
    output = "bool";
    break;
  }
  if (bt.typeModifiers.find(Type::Modifiers::CONST)) {
    output += " const ";
  }
  return output;
}
atl::string DotGraph::visit(BinOp &bo) {
  const atl::string binOpID = "BinOp" + atl::to_string(++nodeCount);
  declare(binOpID, "BinOp");

  const atl::string lhsId = bo.lhs->accept(*this);

  const atl::string OpID = "Op" + atl::to_string(++nodeCount);
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
  case Op::ASSIGNADD:
    declare(OpID, "+=");
    break;
  }

  const atl::string rhsId = bo.rhs->accept(*this);

  join(binOpID, lhsId);
  join(binOpID, OpID);
  join(binOpID, rhsId);

  return binOpID;
}
atl::string DotGraph::visit(Block &b) {
  const atl::string blockID = "Block" + atl::to_string(++nodeCount);
  declare(blockID, "{}");

  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx) {
    const atl::string currStmt = b.stmts[idx]->accept(*this);
    join(blockID, currStmt);
  }

  return blockID;
}
atl::string DotGraph::visit(BoolLiteral &bl) {
  const atl::string boolID = "BoolLiteral" + atl::to_string(++nodeCount);
  declare(boolID, "'" + bl.getLiteral() + "'");
  return boolID;
}
atl::string DotGraph::visit(CharLiteral &cl) {
  const atl::string charID = "CharLiteral" + atl::to_string(++nodeCount);
  declare(charID, "'" + cl.getLiteral() + "'");
  return charID;
}
atl::string DotGraph::visit(ClassType &ct) {
  const atl::string classTypeID = "ClassType" + atl::to_string(++nodeCount);
  atl::string output = ct.identifier->toString();
  if (ct.typeModifiers.find(Type::Modifiers::CONST)) {
    output += " const";
  }
  return output;
}
atl::string DotGraph::visit(ClassTypeDecl &ctd) {
  const atl::string classID = "Class" + atl::to_string(++nodeCount);
  declare(classID, ctd.getIdentifier()->toString());
  return classID;
}
atl::string DotGraph::visit(ClassTypeDef &ctd) {
  const atl::string classID = "Class" + atl::to_string(++nodeCount);
  declare(classID, ctd.getIdentifier()->toString());

  const atl::string publicDecls = "public" + atl::to_string(++nodeCount);
  const atl::string privateDecls = "private" + atl::to_string(++nodeCount);
  const atl::string protectedDecls = "protected" + atl::to_string(++nodeCount);

  declare(publicDecls, "public:");
  declare(privateDecls, "private:");
  declare(protectedDecls, "protected:");
  join(classID, publicDecls);
  join(classID, privateDecls);
  join(classID, protectedDecls);

  for (unsigned int i = 0; i < ctd.classDecls.size(); ++i) {
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PUBLIC)
      join(publicDecls, ctd.classDecls[i]->accept(*this));
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PRIVATE)
      join(privateDecls, ctd.classDecls[i]->accept(*this));
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PROTECTED)
      join(protectedDecls, ctd.classDecls[i]->accept(*this));
  }

  classTypeDeclIDs[ctd.classType->identifier->toString().c_str()] = classID;
  return classID;
}
atl::string DotGraph::visit(ConstructorDecl &cd) {
  const atl::string constructorID =
      "ConstructorDecl" + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (unsigned int i = 0; i < cd.constructorParams.size(); ++i) {
    atl::string currParam =
        cd.constructorParams[i]->type->accept(*this) + " " +
        cd.constructorParams[i]->getIdentifier()->toString();
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ");";
  declare(constructorID, cd.classType->identifier->toString() + funParams);
  return constructorID;
}
atl::string DotGraph::visit(ConstructorDef &cd) {
  const atl::string constructorID =
      "ConstructorDef" + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (unsigned int i = 0; i < cd.constructorParams.size(); ++i) {
    atl::string currParam =
        cd.constructorParams[i]->type->accept(*this) + " " +
        cd.constructorParams[i]->getIdentifier()->toString();
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ")";
  declare(constructorID, cd.classType->identifier->toString() + funParams);

  atl::string initialiserListID =
      "ConstructorDef_IL" + atl::to_string(++nodeCount);
  declare(initialiserListID, ":");
  for (unsigned int idx = 0; idx < cd.initialiserList.size(); ++idx) {
    join(initialiserListID, cd.initialiserList[idx]->accept(*this));
  }
  join(constructorID, initialiserListID);

  join(constructorID, cd.constructorBlock->accept(*this));
  return constructorID;
}
atl::string DotGraph::visit(Deletion &d) {
  const atl::string deletionID = "Deletion" + atl::to_string(++nodeCount);
  atl::string label;
  switch (d.deletionType) {
  case Deletion::DelType::OBJECT:
    label = "delete";
    break;
  case Deletion::DelType::ARRAY:
    label = "delete[]";
    break;
  }
  declare(deletionID, label);
  join(deletionID, d.deletionVar->accept(*this));
  return deletionID;
}
atl::string DotGraph::visit(DestructorDecl &dd) {
  const atl::string destructorID =
      "ConstructorDecl" + atl::to_string(++nodeCount);
  declare(destructorID, dd.classType->identifier->toString() + "();");
  return destructorID;
}
atl::string DotGraph::visit(DestructorDef &dd) {
  const atl::string destructorID =
      "ConstructorDef" + atl::to_string(++nodeCount);
  declare(destructorID, dd.classType->identifier->toString() + "()");
  join(destructorID, dd.destructorBlock->accept(*this));
  return destructorID;
}
atl::string DotGraph::visit(DoWhile &dw) {
  const atl::string whileID = "While" + atl::to_string(++nodeCount);
  declare(whileID, "do {} while()");
  join(whileID, dw.body->accept(*this));
  join(whileID, dw.condition->accept(*this));
  return whileID;
}
atl::string DotGraph::visit(EnumClassTypeDecl &ectd) {
  const atl::string enumClassTypeDeclID =
      "EnumClassTypeDecl" + atl::to_string(++nodeCount);
  declare(enumClassTypeDeclID,
          "enum class " + ectd.getIdentifier()->toString());

  const atl::vector<atl::string> state_keys = ectd.states.keys();
  for (unsigned int idx = 0; idx < state_keys.size(); ++idx) {
    const atl::pair<atl::string, atl::string> name_val =
        ectd.states.find(state_keys[idx]);
    const atl::string stateID =
        "EnumClassTypeDeclState" + atl::to_string(++nodeCount);
    declare(stateID, name_val.first.c_str());
    join(enumClassTypeDeclID, stateID);
    if (name_val.second != "") {
      const atl::string stateValueID =
          "EnumClassTypeDeclState" + atl::to_string(++nodeCount);
      declare(stateValueID, name_val.second.c_str());
      join(stateID, stateValueID);
    }
  }
  return enumClassTypeDeclID;
}
atl::string DotGraph::visit(For &f) {
  const atl::string forID = "For" + atl::to_string(++nodeCount);
  declare(forID, "for(;;)");
  join(forID, f.initialVarDecl->accept(*this));
  join(forID, f.condition->accept(*this));
  join(forID, f.endBodyExpr->accept(*this));
  join(forID, f.body->accept(*this));
  return forID;
}
atl::string DotGraph::visit(FunCall &fc) {
  const atl::string funCallID = "FunCall" + atl::to_string(++nodeCount);
  declare(funCallID, fc.funIdentifier->toString() + "()");

  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    join(funCallID, fc.funArgs[idx]->accept(*this));

  return funCallID;
}
atl::string DotGraph::visit(FunDecl &fd) {
  const atl::string funcID = "FunDecl" + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (unsigned int i = 0; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier()->toString();
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ");";
  declare(funcID, fd.funIdentifier->toString() + funParams);
  return funcID;
}
atl::string DotGraph::visit(FunDef &fd) {
  const atl::string funcID = "FunDef" + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (unsigned int i = 0; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier()->toString();
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ")";
  declare(funcID, fd.funIdentifier->toString() + funParams);
  join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
atl::string DotGraph::visit(If &i) {
  const atl::string ifID = "If" + atl::to_string(++nodeCount);
  declare(ifID, "if then else");

  join(ifID, i.ifCondition->accept(*this));
  join(ifID, i.ifBody->accept(*this));
  if (i.elseBody != nullptr) {
    join(ifID, i.elseBody->accept(*this));
  }

  return ifID;
}
atl::string DotGraph::visit(IntLiteral &il) {
  const atl::string intID = "IntLiteral" + atl::to_string(++nodeCount);
  declare(intID, il.getLiteral());
  return intID;
}
atl::string DotGraph::visit(Namespace &n) {
  const atl::string namespaceID = "Namespace" + atl::to_string(++nodeCount);
  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);
  return namespaceID;
}
atl::string DotGraph::visit(MemberAccess &ma) {
  const atl::string memberAccessID =
      "MemberAccess" + atl::to_string(++nodeCount);
  declare(memberAccessID, " . / -> ");
  const atl::string objID = ma.object->accept(*this);
  const atl::string fieldID =
      "MemberAccess_Field" + atl::to_string(++nodeCount);
  declare(fieldID, ma.fieldIdentifier->toString());

  join(memberAccessID, fieldID);
  join(memberAccessID, objID);
  return memberAccessID;
}
atl::string DotGraph::visit(MemberCall &mc) {
  const atl::string memberCallID = "MemberCall" + atl::to_string(++nodeCount);
  declare(memberCallID, " . / -> ");
  const atl::string objID = mc.object->accept(*this);
  const atl::string funcID = mc.funCall->accept(*this);
  join(memberCallID, objID);
  join(memberCallID, funcID);
  return memberCallID;
}
atl::string DotGraph::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::string DotGraph::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
atl::string DotGraph::visit(PrefixOp &po) {
  const atl::string preficIncID = "PrefixOp" + atl::to_string(++nodeCount);
  atl::string opStr = "PrefixOp(";
  if (po.operation == PrefixOp::Op::DEC)
    opStr += "--";
  if (po.operation == PrefixOp::Op::INC)
    opStr += "++";
  declare(preficIncID, opStr + ")");
  join(preficIncID, po.variable->accept(*this));
  return preficIncID;
}
atl::string DotGraph::visit(Program &p) {
  printf("digraph prog {\n");
  for (unsigned int idx = 0; idx < p.decls.size(); ++idx) {
    p.decls[idx]->accept(*this);
  }
  printf("}\n");
  return "Node0";
}
atl::string DotGraph::visit(ReferenceType &rt) {
  atl::string output = rt.referencedType->accept(*this) + " &";
  if (rt.typeModifiers.find(Type::Modifiers::CONST)) {
    output += " const ";
  }
  return output;
}
atl::string DotGraph::visit(Return &r) {
  const atl::string returnID = "Return" + atl::to_string(++nodeCount);
  declare(returnID, "return");

  if (r.returnExpr != nullptr) {
    join(returnID, r.returnExpr->accept(*this));
  }

  return returnID;
}
atl::string DotGraph::visit(SizeOf &so) {
  const atl::string returnID = "SizeOf" + atl::to_string(++nodeCount);
  declare(returnID, "sizeof(" + so.type->accept(*this) + ")");

  return returnID;
}
atl::string DotGraph::visit(StringLiteral &sl) {
  const atl::string strID = "StringLiteral" + atl::to_string(++nodeCount);
  declare(strID, "\\\"" + sl.getLiteral() + "\\\"");
  return strID;
}
atl::string DotGraph::visit(StructType &st) {
  return "struct " + st.identifier->toString();
}
atl::string DotGraph::visit(StructTypeDecl &std) {
  const atl::string structTypeDeclID =
      "StructTypeDecl" + atl::to_string(++nodeCount);
  declare(structTypeDeclID, std.structType->accept(*this) + " = {}");
  for (unsigned int idx = 0; idx < std.varDecls.size(); ++idx)
    join(structTypeDeclID, std.varDecls[idx]->accept(*this));
  return structTypeDeclID;
}
atl::string DotGraph::visit(TertiaryExpr &t) {
  const atl::string tertiaryID = "TertiaryExpr" + atl::to_string(++nodeCount);
  const atl::string conditionID = t.tertiaryCondition->accept(*this);
  const atl::string ifBodyID = t.tertiaryIfBody->accept(*this);
  const atl::string elseBodyID = t.tertiaryElseBody->accept(*this);
  declare(tertiaryID, "TertiaryExpr");
  join(tertiaryID, conditionID);
  join(tertiaryID, ifBodyID);
  join(tertiaryID, elseBodyID);
  return tertiaryID;
}
atl::string DotGraph::visit(Throw &t) {
  const atl::string throwID = "Throw" + atl::to_string(++nodeCount);
  declare(throwID, "throw");
  join(throwID, t.exceptionText->accept(*this));
  return throwID;
}
atl::string DotGraph::visit(TypeCast &tc) {
  const atl::string typecastID = "TypeCast" + atl::to_string(++nodeCount);
  declare(typecastID, "(" + tc.type->accept(*this) + ")");
  join(typecastID, tc.expr->accept(*this));
  return typecastID;
}
atl::string DotGraph::visit(TypeDefDecl &td) {
  const atl::string typeDefDeclID = "TypeDefDecl" + atl::to_string(++nodeCount);
  declare(typeDefDeclID, "typedef (" + td.type->accept(*this) + ") => (" +
                             td.identifier->toString() + ")");
  return typeDefDeclID;
}
atl::string DotGraph::visit(ValueAt &va) {
  const atl::string derefID = "Deref" + atl::to_string(++nodeCount);
  declare(derefID, "Deref");
  join(derefID, va.derefExpr->accept(*this));
  return derefID;
}
atl::string DotGraph::visit(VarDecl &vd) {
  const atl::string varDeclID = "VarDecl" + atl::to_string(++nodeCount);
  const atl::string outputStr =
      vd.type->accept(*this) + " " + vd.identifier->toString() + ";";
  declare(varDeclID, outputStr);
  return varDeclID;
}
atl::string DotGraph::visit(VarDef &vd) {
  const atl::string varDefID = "VarDef" + atl::to_string(++nodeCount);
  const atl::string valueID = vd.varValue->accept(*this);
  declare(varDefID,
          vd.type->accept(*this) + " " + vd.identifier->toString() + " = ");
  join(varDefID, valueID);
  return varDefID;
}
atl::string DotGraph::visit(VarExpr &ve) {
  const atl::string varID = "VarExpr" + atl::to_string(++nodeCount);
  declare(varID, ve.varIdentifier->toString().c_str());
  return varID;
}
atl::string DotGraph::visit(While &w) {
  const atl::string whileID = "While" + atl::to_string(++nodeCount);
  declare(whileID, "while()");
  join(whileID, w.condition->accept(*this));
  join(whileID, w.body->accept(*this));
  return whileID;
}