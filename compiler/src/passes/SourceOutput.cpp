#include "../../include/passes/SourceOutput.h"

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

atl::string SourceOutput::visit(Allocation &a) {
  atl::string output = "new ";
  if (a.variableType)
    output += a.variableType->accept(*this);
  else
    output += a.variableConstructorCall->accept(*this);

  return output;
}
atl::string SourceOutput::visit(ArrayAccess &aa) {
  atl::string output;
  return output;
}
atl::string SourceOutput::visit(ArrayType &at) {
  atl::string output = at.arrayType->accept(*this);
  output += "[";
  output += at.arraySize;
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
  }
  output += bo.rhs->accept(*this);
  return output;
}
atl::string SourceOutput::visit(Block &b) {
  atl::string output = "{\n";
  for (int idx = 0; idx < b.blockStmts.size(); ++idx) {
    output += b.blockStmts[idx]->accept(*this);
  }
  output += "\n}";
  return output;
}
atl::string SourceOutput::visit(CharLiteral &cl) {
  atl::string output = "'";
  return output + cl.getLiteral() + "'";
}
atl::string SourceOutput::visit(ClassType &ct) { return ct.identifier; }
atl::string SourceOutput::visit(ClassTypeDecl &ctd) {
  atl::string output = ctd.getIdentifier() + " {";
  for (int i = 0; i < ctd.classDecls.size(); ++i) {
    output += "\n";
    switch (ctd.classDecls[i]->visibility) {
    case ClassTypeDecl::Visibility::PUBLIC:
      output += "public:\n";
      break;
    case ClassTypeDecl::Visibility::PRIVATE:
      output += "private:\n";
      break;
    case ClassTypeDecl::Visibility::PROTECTED:
      output += "protected:\n";
      break;
    case ClassTypeDecl::Visibility::NONE:
      break;
    }
    output += ctd.classDecls[i]->accept(*this);
  }
  output += "\n};";
  return output;
}
atl::string SourceOutput::visit(ConstructorDecl &cd) {
  atl::string output = cd.classType->accept(*this);
  output += "(";
  for (int i = 0; i < cd.constructorParams.size(); ++i) {
    atl::string currParam = cd.constructorParams[i]->type->accept(*this) + " " +
                            cd.constructorParams[i]->getIdentifier();
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
  for (int i = 0; i < cd.constructorParams.size(); ++i) {
    atl::string currParam = cd.constructorParams[i]->type->accept(*this) + " " +
                            cd.constructorParams[i]->getIdentifier();
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  output += cd.constructorBlock->accept(*this);

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
atl::string SourceOutput::visit(EnumTypeDecl &etd) { return ""; }
atl::string SourceOutput::visit(FieldAccess &fa) { return ""; }
atl::string SourceOutput::visit(For &f) { return ""; }
atl::string SourceOutput::visit(FunCall &fc) {
  atl::string output = fc.funName + "(";
  for (int i = 0; i < fc.funArgs.size(); ++i) {
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
  output += fd.getIdentifier() + "(";
  for (int i = 0; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier();
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ");";

  return output;
}
atl::string SourceOutput::visit(FunDef &fd) {
  atl::string output = fd.funType->accept(*this) + " ";
  output += fd.getIdentifier() + "(";
  for (int i = 0; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier();
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    output += currParam;
  }
  output += ")";
  output += fd.funBlock->accept(*this);

  return output;
}
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
atl::string SourceOutput::visit(Namespace &n) {
  atl::string output = "namespace ";
  output += n.getIdentifier();

  return output;
}
atl::string SourceOutput::visit(ParenthExpr &pe) {
  atl::string output = "(";
  output += pe.innerExpr->accept(*this);
  output += ")";
  return output;
}
atl::string SourceOutput::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
atl::string SourceOutput::visit(Program &p) {
  atl::string output;
  for (int idx = 0; idx < p.decls.size(); ++idx) {
    output += p.decls[idx]->accept(*this) + "\n";
  }
  return output;
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
atl::string SourceOutput::visit(StructType &st) {
  return atl::string("struct ") + st.identifier;
}
atl::string SourceOutput::visit(StructTypeDecl &std) {
  atl::string output = std.getIdentifier() + " {";
  for (int i = 0; i < std.varDecls.size(); ++i) {
    output += "\n";
    output += std.varDecls[i]->accept(*this);
  }
  output += "\n};";
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
  output += vd.getIdentifier();
  output += ";";
  return output;
}
atl::string SourceOutput::visit(VarDef &vd) {
  atl::string output = vd.type->accept(*this);
  output += vd.getIdentifier() + " = ";
  output += vd.value->accept(*this);
  output += ";";
  return output;
}
atl::string SourceOutput::visit(VarExpr &ve) { return ve.identifier; }
atl::string SourceOutput::visit(While &w) {
  atl::string output = "while (";
  output += w.condition->accept(*this);
  output += ")";
  output += w.body->accept(*this);
  return output;
}