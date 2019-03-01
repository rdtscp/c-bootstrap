
#include "../../include/targets/GenerateMIPS.h"

using namespace ACC;

GenerateMIPS::GenerateMIPS(atl::shared_ptr<Program> progAST,
                           const atl::string &outputFile)
    : MIPS(outputFile), progAST(progAST) {}

void GenerateMIPS::error(const atl::string &error) {
  errorCount++;
  errors.push_back(error);
}

void GenerateMIPS::printErrors() const {
  printf("FATAL MIPS Generation Errors:\n");
  for (int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void GenerateMIPS::run() {
  freeAllRegs();
  visit(*progAST);
}

void GenerateMIPS::freeAllRegs() {
  freeAllArgsRegs();
  freeAllSaveRegs();
  freeAllTempRegs();
}

void GenerateMIPS::freeAllArgsRegs() {
  for (int idx = MIPS::argsRegs.size() - 1; idx >= 0; --idx)
    freeArgsRegs.push(MIPS::argsRegs[idx]);
}

void GenerateMIPS::freeAllSaveRegs() {
  for (int idx = MIPS::saveRegs.size() - 1; idx >= 0; --idx)
    freeSaveRegs.push(MIPS::saveRegs[idx]);
}

void GenerateMIPS::freeAllTempRegs() {
  for (int idx = MIPS::tempRegs.size() - 1; idx >= 0; --idx)
    freeTempRegs.push(MIPS::tempRegs[idx]);
}

void GenerateMIPS::freeRegister(MIPS::Register reg) {
  switch (reg.name[0]) {
  case 'a':
    freeArgsRegs.push(reg);
    break;
  case 's':
    freeSaveRegs.push(reg);
    break;
  case 't':
    freeTempRegs.push(reg);
    break;
  default:
    break;
  }
}

MIPS::Register GenerateMIPS::getArgsRegister() {
  if (freeArgsRegs.size() > 0) {
    MIPS::Register reg = freeArgsRegs.top();
    freeArgsRegs.pop();
    return reg;
  }
  throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                           "allocated Args Register.");
}

MIPS::Register GenerateMIPS::getSaveRegister() {
  if (freeSaveRegs.size() > 0) {
    MIPS::Register reg = freeSaveRegs.top();
    freeSaveRegs.pop();
    return reg;
  }
  throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                           "allocated Save Register.");
}

MIPS::Register GenerateMIPS::getTempRegister() {
  if (freeTempRegs.size() > 0) {
    MIPS::Register reg = freeTempRegs.top();
    freeTempRegs.pop();
    return reg;
  }
  throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                           "allocated Temp Register.");
}

/* ---- MIPS Memory ---- */

void GenerateMIPS::alloc(const int bytes) {
  MIPS.comment(" !--- Cannot Allocate on Heap yet ----! ");
}

void GenerateMIPS::stackPush(const MIPS::Register &src) {
  MIPS.ADDI(MIPS::sp, MIPS::sp, -4);
  MIPS.SW(src, MIPS::sp);
}

void GenerateMIPS::stackPop(const MIPS::Register &dest) {
  MIPS.LW(dest, MIPS::sp);
  MIPS.ADDI(MIPS::sp, MIPS::sp, 4);
}

/* ---- Visit AST ---- */

MIPS::Register GenerateMIPS::visit(Allocation &a) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(ArrayAccess &aa) {
  aa.array->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(ArrayType &at) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(Assign &as) {
  as.lhs->accept(*this);
  as.rhs->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(BaseType &bt) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(Block &b) {
  currScope = b.getptr();
  for (int idx = 0; idx < b.blockStmts.size(); ++idx)
    b.blockStmts[idx]->accept(*this);

  /* Clean up the stack. */
  for (const auto &ident_decl : b.blockDecls) {
    atl::shared_ptr<Decl> currDecl = ident_decl.second;
    if (currDecl->astClass() == "VarDecl") {
      /* ---- Deconstruct VarDecl ---- */
      MIPS.ADDI(MIPS::sp, MIPS::sp, 4);
      MIPS.SW(MIPS::zero, MIPS::sp);
    }
  }
  currScope = b.outerBlock;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(CharLiteral &cl) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(ClassType &ct) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(ClassTypeDecl &ctd) {
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(EnumTypeDecl &etd) {
  return MIPS::Register();
}

MIPS::Register GenerateMIPS::visit(FieldAccess &fa) {
  fa.object->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(FunCall &fc) {
  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);
  MIPS.JAL(fc.funName + "FunDecl");
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(FunDecl &fd) {
  // atl::vector<MIPS::Register> saveRegs = MIPS::saveRegs;
  // currScope = fd.funBlock;

  // MIPS.BLOCK(fd.getIdentifier() + "FunDecl");

  // /* ---- Save Caller's $fp ---- */
  // stackPush(MIPS::fp);

  // /* ---- Save Registers [ $s0-$s7 ] && $ra ---- */
  // for (const MIPS::Register &saveReg : saveRegs)
  //   stackPush(saveReg);
  // stackPush(MIPS::ra);

  // /* ---- Construct Arguments ---- */
  // // @TODO

  // /* ---- Execute Function ---- */

  // fd.funBlock->accept(*this);

  // /* -------------------------- */

  // /* ---- Deconstruct Arguments ---- */
  // // @TODO

  // /* ---- Load Registers [ $s0-$s7 ] && $ra ---- */
  // stackPop(MIPS::ra);
  // for (const MIPS::Register &saveReg : saveRegs)
  //   stackPop(saveReg);

  // /* ---- Load Caller's $fp ---- */
  // stackPop(MIPS::fp);

  // /* ---- Return to $ra ---- */
  // MIPS.write("JR $ra");

  // currScope = fd.funBlock->outerBlock;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(FunDef &fd) {
  atl::vector<MIPS::Register> saveRegs = MIPS::saveRegs;
  currScope = fd.funBlock;

  MIPS.BLOCK(fd.getIdentifier() + "FunDecl");

  /* ---- Save Caller's $fp ---- */
  stackPush(MIPS::fp);

  /* ---- Save Registers [ $s0-$s7 ] && $ra ---- */
  for (int idx = 0; idx < saveRegs.size(); ++idx)
    stackPush(saveRegs[idx]);
  stackPush(MIPS::ra);

  /* ---- Construct Arguments ---- */
  // @TODO

  /* ---- Execute Function ---- */

  fd.funBlock->accept(*this);

  /* -------------------------- */

  /* ---- Deconstruct Arguments ---- */
  // @TODO

  /* ---- Load Registers [ $s0-$s7 ] && $ra ---- */
  stackPop(MIPS::ra);
  for (int idx = saveRegs.size() - 1; idx >= 0; --idx)
    stackPop(saveRegs[idx]);

  /* ---- Load Caller's $fp ---- */
  stackPop(MIPS::fp);

  /* ---- Return to $ra ---- */
  MIPS.write("JR $ra");

  currScope = fd.funBlock->outerBlock;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(IntLiteral &il) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(Namespace &n) {
  // n.namespaceBlock->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(ParenthExpr &pe) {
  pe.innerExpr->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(PointerType &pt) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(Program &p) {
  currScope = p.globalScope;

  freeAllRegs();

  MIPS.write(".data");
  for (int idx = 0; idx < p.globalVars.size(); ++idx) {
    MIPS.alloc(p.globalVars[idx]->getIdentifier(),
               p.globalVars[idx]->getBytes());
  }

  MIPS.write(".text");
  MIPS.JAL("mainFunDecl");
  // for (atl::shared_ptr<FunDef> func : p.funDefs)
  //   func->accept(*this);

  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(SizeOf &so) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(StringLiteral &sl) {
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(StructType &st) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(StructTypeDecl &std) {
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(TypeCast &tc) {
  return tc.expr->accept(*this);
}
MIPS::Register GenerateMIPS::visit(TypeDefDecl &td) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(ValueAt &va) {
  va.derefExpr->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(VarDecl &vd) {
  MIPS.comment(atl::string("Allocating for VarDecl: ") + vd.getIdentifier());
  int bytesRequired = vd.getBytes();
  MIPS.ADDI(MIPS::sp, MIPS::sp, -bytesRequired);
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(VarDef &vd) {
  MIPS.comment(atl::string("Allocating for VarDef: ") + vd.getIdentifier());
  int bytesRequired = vd.getBytes();
  MIPS.ADDI(MIPS::sp, MIPS::sp, -bytesRequired);
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  int fpOffset = ve.variableDecl->fpOffset;
  MIPS::Register valReg = getTempRegister();
  MIPS.LW(valReg, MIPS::fp, fpOffset);
  return valReg;
}
MIPS::Register GenerateMIPS::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return MIPS::Register();
}