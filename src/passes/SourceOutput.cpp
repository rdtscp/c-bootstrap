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
atl::string SourceOutput::visit(ArrayType &at) {
  atl::string output = at.pointedType->accept(*this);
  output += "[";
  output += at.size->accept(*this);
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
  default:
    output = "";
    break;
  }
  if (bt.typeModifiers.find(Type::Modifiers::CONST))
    output += " const";

  return output;
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
  if (bo.operation == Op::ASSIGNADD)
    output += ";";
  return output;
}
atl::string SourceOutput::visit(Block &b) {
  atl::string output = "{\n";
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx) {
    output += b.stmts[idx]->accept(*this);
  }
  output += "\n}";
  return output;
}
atl::string SourceOutput::visit(BoolLiteral &bl) { return bl.getLiteral(); }
atl::string SourceOutput::visit(CharLiteral &cl) {
  atl::string output = "'";
  return output + cl.getLiteral() + "'";
}
atl::string SourceOutput::visit(ClassType &ct) {
  atl::string output = ct.identifier->toString();
  if (ct.typeModifiers.find(Type::Modifiers::CONST))
    output += " const";
  return output;
}
atl::string SourceOutput::visit(ClassTypeDecl &ctd) {
  atl::string output = "class " + ctd.getIdentifier()->accept(*this) + ";";
  return output;
}
atl::string SourceOutput::visit(ClassTypeDef &ctd) {
  atl::string output = "class " + ctd.getIdentifier()->accept(*this) + " {";
  // Print Public:
  output += "public:\n";
  for (unsigned int i = 0; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PUBLIC) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  // Print Private:
  output += "private:\n";
  for (unsigned int i = 0; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PRIVATE) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  // Print Protected
  output += "protected:\n";
  for (unsigned int i = 0; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    if (ctd.classDecls[i]->visibility == ClassTypeDecl::Visibility::PROTECTED) {
      output += ctd.classDecls[i]->accept(*this);
    }
  }
  output += "\n};";
  return output;
}
atl::string SourceOutput::visit(ConstructorCall &cc) {
  atl::string output = cc.constructorIdentifier->toString() + "(";
  for (unsigned int i = 0; i < cc.constructorArgs.size(); ++i) {
    atl::string currParam = cc.constructorArgs[i]->accept(*this);
    if (i != (cc.constructorArgs.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  return output;
}
atl::string SourceOutput::visit(ConstructorDecl &cd) {
  atl::string output = cd.classType->accept(*this);
  output += "(";
  for (unsigned int i = 0; i < cd.constructorParams.size(); ++i) {
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
  for (unsigned int i = 1u; i < cd.constructorParams.size(); ++i) {
    atl::string currParam = cd.constructorParams[i]->type->accept(*this) + " ";

    currParam += cd.constructorParams[i]->getIdentifier()->accept(*this);
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  const unsigned int initialiserListSize = cd.initialiserList.size();
  if (initialiserListSize > 0) {
    output += " : ";
    for (unsigned int idx = 0; idx < initialiserListSize; ++idx) {
      output += cd.initialiserList[idx]->lhs->accept(*this);
      output += "(";
      output += cd.initialiserList[idx]->rhs->accept(*this);
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
atl::string SourceOutput::visit(For &f) {
  atl::string output = "for (";
  output += f.initialVarDecl->accept(*this);
  output += " " + f.condition->accept(*this) + "; ";
  atl::string endBodyExpr = f.endBodyExpr->accept(*this);
  for (unsigned int i = 0; i < endBodyExpr.size() - 1u; ++i)
    output += endBodyExpr[i];
  output += ") " + f.body->accept(*this);
  return output;
}
atl::string SourceOutput::visit(FunCall &fc) {
  atl::string output = fc.funIdentifier->toString() + "(";
  for (unsigned int i = 0; i < fc.funArgs.size(); ++i) {
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
  for (unsigned int i = 0; i < fd.funParams.size(); ++i) {
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
  atl::string output = "\n";
  if (fd.funModifiers.find(FunDecl::FunModifiers::STATIC))
    output += "static ";
  output += fd.funType->accept(*this) + " ";
  output += fd.getIdentifier()->accept(*this) + "(";
  for (unsigned int i = 0; i < fd.funParams.size(); ++i) {
    if (fd.funParams[i]->getIdentifier()->accept(*this) == "this")
      continue;

    atl::string currParam = fd.funParams[i]->type->accept(*this) + " ";
    if (fd.funParams[i]->type->typeModifiers.find(Type::Modifiers::CONST))
      currParam += "const ";
    currParam += fd.funParams[i]->getIdentifier()->accept(*this);
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";

  if (fd.funModifiers.find(FunDecl::FunModifiers::CONST))
    output += " const ";

  output += fd.funBlock->accept(*this);

  return output + "\n";
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
atl::string SourceOutput::visit(MemberAccess &ma) {
  atl::string output = ma.object->accept(*this);
  if (ma.accessType == SourceToken::Class::PTRDOT)
    output += "->";
  else
    output += ".";

  return output + ma.fieldVariable->accept(*this);
}
atl::string SourceOutput::visit(MemberCall &mc) {
  atl::string output = mc.object->accept(*this);
  if (mc.accessType == SourceToken::Class::PTRDOT)
    output += "->";
  else
    output += ".";

  return output + mc.funCall->accept(*this);
}
atl::string SourceOutput::visit(Namespace &n) {
  atl::string output = "namespace ";
  output += n.getIdentifier()->accept(*this);
  output += "{\n";
  const unsigned int numDecls = n.namespaceDecls.size();
  for (unsigned int idx = 0; idx < numDecls; ++idx) {
    output += n.namespaceDecls[idx]->accept(*this) + "\n";
  }
  output += "}";

  return output;
}
atl::string SourceOutput::visit(Not &n) {
  atl::string output = "!";
  output += n.expr->accept(*this);
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
  atl::string output = pt.pointedType->accept(*this);
  if (pt.typeModifiers.find(Type::Modifiers::CONST))
    output += " const";
  return output + " *";
}
atl::string SourceOutput::visit(PrefixOp &po) {
  atl::string output;
  if (po.operation == PrefixOp::Op::INC)
    output += "++";
  if (po.operation == PrefixOp::Op::DEC)
    output += "--";
  output += po.variable->accept(*this) + ";";
  return output;
}
atl::string SourceOutput::visit(Program &p) {
  atl::string output;
  for (unsigned int idx = 0; idx < p.decls.size(); ++idx) {
    output += p.decls[idx]->accept(*this) + "\n";
  }
  return output;
}
atl::string SourceOutput::visit(ReferenceType &rt) {
  atl::string output = rt.referencedType->accept(*this);
  if (rt.typeModifiers.find(Type::Modifiers::CONST))
    output += " const";
  return output + " &";
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
atl::string SourceOutput::visit(StaticCast &sc) {
  atl::string output = "static_cast<";
  output += sc.type->accept(*this);
  output += ">(";
  output += sc.expr->accept(*this);
  return output + ")";
}
atl::string SourceOutput::visit(StringLiteral &sl) { return sl.getLiteral(); }
atl::string SourceOutput::visit(SubscriptOp &so) {
  return so.variable->accept(*this) + "[" + so.index->accept(*this) + "]";
}
atl::string SourceOutput::visit(TemplateDef &td) { throw; }
atl::string SourceOutput::visit(TemplatedFunCall &tfc) { throw; }
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
  output += "\";";
  return output;
}
atl::string SourceOutput::visit(TypeDefDecl &td) {
  return "typedef " + td.type->accept(*this) + " " +
         td.identifier->accept(*this) + ";";
}
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