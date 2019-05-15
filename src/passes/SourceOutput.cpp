#include "passes/SourceOutput.h"

using namespace ACC;

SourceOutput::SourceOutput(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}
void SourceOutput::print() { printf("%s", visit(*progAST).c_str()); }

void SourceOutput::declare(const atl::string &nodeID,
                           const atl::string &label) {
  put(nodeID + " [label=\"" + label + "\"];");
}

void SourceOutput::join(const atl::string &lhs, const atl::string &rhs) {
  atl::string output = lhs + " -> " + rhs;
  printf("%s\n", output.c_str());
}

void SourceOutput::put(const atl::string &str) { printf("%s\n", str.c_str()); }

/* ---- Visit AST ---- */

atl::string SourceOutput::visit(AddressOf &ao) {
  atl::string output = "&";
  return output + ao.addressOfExpr->accept(*this);
}
atl::string SourceOutput::visit(Allocation &a) {
  atl::string output = "new ";
  if (a.varType)
    output += a.varType->accept(*this);
  else
    output += a.varConstructorCall->accept(*this);

  return output;
}
atl::string SourceOutput::visit(ArrayAccess &aa) {
  atl::string output;
  return output;
}
atl::string SourceOutput::visit(ArrayType &at) {
  atl::string output = at.pointedType->accept(*this);
  output += "[";
  output += at.size;
  output += "]";
  return output;
}
atl::string SourceOutput::visit(Assign &as) {
  atl::string output = as.lhs->accept(*this);
  output += " = ";
  output += as.rhs->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(BaseType &bt) {
  switch (bt.primitiveType) {
  case PrimitiveType::CHAR:
    return "char";
  case PrimitiveType::INT:
    return "int";
  case PrimitiveType::SHORT:
    return "short";
  case PrimitiveType::VOID:
    return "void";
  case PrimitiveType::UINT:
    return "unsigned int";
  case PrimitiveType::BOOL:
    return "bool";
  default:
    return "";
  }
}
atl::string SourceOutput::visit(BinOp &bo) {
  atl::string output = bo.lhs->accept(*this);

  switch (bo.operation) {
  case Op::ADD:
    output += "+";
    break;
  case Op::AND:
    output += "&&";
    break;
  case Op::DIV:
    output += "/";
    break;
  case Op::EQ:
    output += "==";
    break;
  case Op::GE:
    output += ">=";
    break;
  case Op::GT:
    output += ">";
    break;
  case Op::LE:
    output += "<=";
    break;
  case Op::LT:
    output += "<";
    break;
  case Op::MOD:
    output += "%";
    break;
  case Op::MUL:
    output += "*";
    break;
  case Op::NE:
    output += "!=";
    break;
  case Op::OR:
    output += "||";
    break;
  case Op::SUB:
    output += "-";
    break;
  case Op::ASSIGNADD:
    output += "+=";
    break;
  }
  output += bo.rhs->accept(*this);
  return output;
}
atl::string SourceOutput::visit(Block &b) {
  atl::string output = "{\n";
  for (unsigned int idx = 0u; idx < b.stmts.size(); ++idx) {
    output += b.stmts[idx]->accept(*this);
  }
  output += "\n}";
  return output;
}

atl::string SourceOutput::visit(BoolLiteral &bl) {
  atl::string output = "'";
  return output + bl.getLiteral() + "'";
}
atl::string SourceOutput::visit(CharLiteral &cl) {
  atl::string output = "'";
  return output + cl.getLiteral() + "'";
}
atl::string SourceOutput::visit(ClassType &ct) {
  return ct.identifier->toString();
}
atl::string SourceOutput::visit(ClassTypeDecl &ctd) {
  atl::string output = "class " + ctd.getIdentifier()->accept(*this) + ";";
  return output;
}
atl::string SourceOutput::visit(ClassTypeDef &ctd) {
  atl::string output = "class " + ctd.getIdentifier()->accept(*this) + " {";
  // Print Public:
  output += "public:\n";
  for (unsigned int i = 0u; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PUBLIC) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  // Print Private:
  output += "private:\n";
  for (unsigned int i = 0u; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PRIVATE) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  // Print Protected
  output += "protected:\n";
  for (unsigned int i = 0u; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PROTECTED) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  output += "\n};";
  return output;
}
atl::string SourceOutput::visit(ConstructorDecl &cd) {
  atl::string output = cd.classType->accept(*this);
  output += "(";
  for (unsigned int i = 0u; i < cd.constructorParams.size(); ++i) {
    atl::string currParam =
        cd.constructorParams[i]->type->accept(*this) + " " +
        cd.constructorParams[i]->getIdentifier()->accept(*this);
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ");";

  return output;
}
atl::string SourceOutput::visit(ConstructorDef &cd) {
  atl::string output = cd.classType->accept(*this);
  output += "(";
  for (unsigned int i = 0u; i < cd.constructorParams.size(); ++i) {
    atl::string currParam =
        cd.constructorParams[i]->type->accept(*this) + " " +
        cd.constructorParams[i]->getIdentifier()->accept(*this);
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  const unsigned int initialiserListSize = cd.initialiserList.size();
  if (initialiserListSize > 0) {
    output += " : ";
    for (unsigned int idx = 0u; idx < initialiserListSize; ++idx) {
      output += cd.initialiserList[idx]->lhs->accept(*this);
      output += "(";
      output += cd.initialiserList[idx]->lhs->accept(*this);
      output += ")";
      if (idx < (initialiserListSize - 1))
        output += ", ";
    }
  }
  output += cd.constructorBlock->accept(*this);

  return output;
}
atl::string SourceOutput::visit(Deletion &d) {
  atl::string output = "delete";
  if (d.deletionType == Deletion::DelType::ARRAY) {
    output += "[]";
  }
  output += " ";
  output += d.deletionVar->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(DestructorDecl &dd) {
  atl::string output = dd.classType->accept(*this);
  output += "();";
  return output;
}
atl::string SourceOutput::visit(DestructorDef &dd) {
  atl::string output = dd.classType->accept(*this);
  output += "()";
  output += dd.destructorBlock->accept(*this);
  return output;
}
atl::string SourceOutput::visit(DoWhile &dw) {
  atl::string output = "do";
  output += dw.body->accept(*this);
  output += "while (";
  output += dw.condition->accept(*this);
  output += ");";
  return output;
}
atl::string SourceOutput::visit(EnumClassTypeDecl &ectd) {
  atl::string output = "enum class ";
  output += ectd.getIdentifier()->toString() + " {";
  output += "};";
  return output;
}
atl::string SourceOutput::visit(For &f) { return ""; }
atl::string SourceOutput::visit(FunCall &fc) {
  atl::string output = fc.funIdentifier->toString() + "(";
  for (unsigned int i = 0u; i < fc.funArgs.size(); ++i) {
    atl::string currParam = fc.funArgs[i]->accept(*this);
    if (i != (fc.funArgs.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  return output;
}
atl::string SourceOutput::visit(FunDecl &fd) {
  atl::string output = fd.funType->accept(*this) + " ";
  output += fd.getIdentifier()->toString() + "(";
  for (unsigned int i = 0u; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier()->accept(*this);
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ");";

  return output;
}
atl::string SourceOutput::visit(FunDef &fd) {
  atl::string output = fd.funType->accept(*this) + " ";
  output += fd.getIdentifier()->accept(*this) + "(";
  for (unsigned int i = 0u; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier()->accept(*this);
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  output += fd.funBlock->accept(*this);

  return output;
}
atl::string SourceOutput::visit(Identifier &i) { return i.toString(); }
atl::string SourceOutput::visit(If &i) {
  atl::string output = "if (";
  output += i.ifCondition->accept(*this);
  output += ")";
  output += i.ifBody->accept(*this);
  if (i.elseBody) {
    output += "else";
    output += i.elseBody->accept(*this);
  }
  return output;
}
atl::string SourceOutput::visit(IntLiteral &il) { return il.getLiteral(); }
atl::string SourceOutput::visit(MemberAccess &ma) { return ""; }
atl::string SourceOutput::visit(MemberCall &mc) { return ""; }
atl::string SourceOutput::visit(Namespace &n) {
  atl::string output = "namespace ";
  output += n.getIdentifier()->accept(*this);
  output += "{\n";
  const unsigned int numDecls = n.namespaceDecls.size();
  for (unsigned int idx = 0u; idx < numDecls; ++idx) {
    output += n.namespaceDecls[idx]->accept(*this) + "\n";
  }
  output += "}";

  return output;
}
atl::string SourceOutput::visit(Nullptr &n) { return "nullptr"; }
atl::string SourceOutput::visit(ParenthExpr &pe) {
  atl::string output = "(";
  output += pe.innerExpr->accept(*this);
  output += ")";
  return output;
}
atl::string SourceOutput::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
atl::string SourceOutput::visit(PrefixOp &po) {
  atl::string output;
  if (po.operation == PrefixOp::Op::INC)
    output += "++";
  if (po.operation == PrefixOp::Op::DEC)
    output += "--";
  output += po.variable->accept(*this);
  return output;
}
atl::string SourceOutput::visit(Program &p) {
  atl::string output;
  for (unsigned int idx = 0u; idx < p.decls.size(); ++idx) {
    output += p.decls[idx]->accept(*this) + "\n";
  }
  return output;
}
atl::string SourceOutput::visit(ReferenceType &rt) {
  return rt.referencedType->accept(*this) + " &";
}
atl::string SourceOutput::visit(Return &r) {
  atl::string output = "return";
  if (r.returnExpr) {
    output += " ";
    output += r.returnExpr->accept(*this);
  }
  output += ";";
  return output;
}
atl::string SourceOutput::visit(SizeOf &so) {
  atl::string output = "sizeof(";
  output += so.type->accept(*this);
  output += ")";
  return output;
}
atl::string SourceOutput::visit(StringLiteral &sl) { return sl.getLiteral(); }
atl::string SourceOutput::visit(TertiaryExpr &t) {
  atl::string output;
  output += t.tertiaryCondition->accept(*this);
  output += " ? ";
  output += t.tertiaryIfBody->accept(*this);
  output += " : ";
  output += t.tertiaryElseBody->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(Throw &t) {
  atl::string output = "throw \"";
  output += t.exceptionText->accept(*this);
  output += "\"";
  return output;
}
atl::string SourceOutput::visit(TypeCast &tc) {
  atl::string output = "(";
  output += tc.type->accept(*this);
  output += ")";
  output += tc.expr->accept(*this);
  return output;
}
atl::string SourceOutput::visit(TypeDefDecl &td) { return ""; }
atl::string SourceOutput::visit(ValueAt &va) {
  atl::string output = "*";
  output += va.derefExpr->accept(*this);
  return output;
}
atl::string SourceOutput::visit(VarDecl &vd) {
  atl::string output = vd.type->accept(*this) + " ";
  output += vd.getIdentifier()->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(VarDef &vd) {
  atl::string output = vd.type->accept(*this) + " ";
  output += vd.getIdentifier()->accept(*this) + " = ";
  output += vd.varValue->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(VarExpr &ve) {
  return ve.varIdentifier->toString();
}
atl::string SourceOutput::visit(While &w) {
  atl::string output = "while (";
  output += w.condition->accept(*this);
  output += ")";
  output += w.body->accept(*this);
  return output;
}