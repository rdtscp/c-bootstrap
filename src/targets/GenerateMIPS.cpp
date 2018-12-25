
#include "../../include/targets/GenerateMIPS.h"

using namespace ACC;

GenerateMIPS::GenerateMIPS(std::shared_ptr<Program> progAST, std::string outputFile)
    : progAST(progAST) {
  MIPS::mipsOutput.open(outputFile);
}

void GenerateMIPS::error(std::string error) {
  errorCount++;
  errors.push_back(error);
}

void GenerateMIPS::printErrors() const {
  std::cerr << "FATAL MIPS Generation Errors:" << std::endl;
  for (const auto &error : errors)
    std::cerr << "\t" << error << std::endl;
}

void GenerateMIPS::run() {
  freeAllRegs();
  GenerateMIPS::visit(*progAST);
  MIPS::mipsOutput.close();
}
void GenerateMIPS::freeAllRegs() {
  freeAllArgsRegs();
  freeAllSaveRegs();
  freeAllTempRegs();
}

void GenerateMIPS::freeAllArgsRegs() {
  for (auto rit = MIPS::argsRegs.rbegin(); rit < MIPS::argsRegs.rend(); rit++)
    freeArgsRegs.push(*rit);
}

void GenerateMIPS::freeAllSaveRegs() {
  for (auto rit = MIPS::saveRegs.rbegin(); rit < MIPS::saveRegs.rend(); rit++)
    freeSaveRegs.push(*rit);
}

void GenerateMIPS::freeAllTempRegs() {
  for (auto rit = MIPS::tempRegs.rbegin(); rit < MIPS::tempRegs.rend(); rit++)
    freeTempRegs.push(*rit);
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
  MIPS::comment(" !--- Cannot Allocate on Heap yet ----! ");
}

void GenerateMIPS::stackPush(const MIPS::Register &src) {
  MIPS::ADDI(MIPS::sp, MIPS::sp, -4);
  MIPS::SW(src, MIPS::sp);
}

void GenerateMIPS::stackPop(const MIPS::Register &dest) {
  MIPS::LW(dest, MIPS::sp);
  MIPS::ADDI(MIPS::sp, MIPS::sp, 4);
}

/* ---- Visit AST ---- */

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
  currScope = std::make_shared<Block>(b);
  for (const auto &stmt : b.blockStmts)
    stmt->accept(*this);

  /* Clean up the stack. */
  for (const auto &ident_decl : b.blockDecls) {
    std::shared_ptr<Decl> currDecl = ident_decl.second;
    if (currDecl->astClass() == "VarDecl") {
      /* ---- Deconstruct VarDecl ---- */
      MIPS::ADDI(MIPS::sp, MIPS::sp, 4);
      MIPS::SW(MIPS::zero, MIPS::sp);
    }
  }
  currScope = b.outerBlock;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(CharLiteral &cl) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(FieldAccess &fa) {
  fa.object->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(FunCall &fc) {

  for (const auto &arg : fc.funArgs)
    arg->accept(*this);
  MIPS::JAL(fc.funName + "FunDecl");
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(FunDecl &fd) {
  std::vector<MIPS::Register> saveRegs = MIPS::saveRegs;
  currScope = fd.funBlock;

  MIPS::BLOCK(fd.getIdentifier() + "FunDecl");

  /* ---- Save Caller's $fp ---- */
  stackPush(MIPS::fp);

  /* ---- Save Registers [ $s0-$s7 ] && $ra ---- */
  for (const MIPS::Register &saveReg : saveRegs)
    stackPush(saveReg);
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
  std::reverse(std::begin(saveRegs), std::end(saveRegs));
  for (const MIPS::Register &saveReg : saveRegs)
    stackPop(saveReg);

  /* ---- Load Caller's $fp ---- */
  stackPop(MIPS::fp);

  /* ---- Return to $ra ---- */
  MIPS::write("JR $ra");

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
MIPS::Register GenerateMIPS::visit(ParenthExpr &pe) {
  pe.innerExpr->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(PointerType &pt) { return MIPS::Register(); }
MIPS::Register GenerateMIPS::visit(Program &p) {
  currScope = p.globalScope;

  freeAllRegs();

  MIPS::write(".data");
  for (std::shared_ptr<VarDecl> globalVar : p.globalVars)
    MIPS::alloc(globalVar->getIdentifier(), globalVar->getBytes());

  MIPS::write(".text");
  MIPS::JAL("mainFunDecl");
  for (std::shared_ptr<FunDecl> func : p.funDecls)
    func->accept(*this);

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
MIPS::Register GenerateMIPS::visit(ValueAt &va) {
  va.derefExpr->accept(*this);
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(VarDecl &vd) {
  MIPS::comment("Allocating for VarDecl: " + vd.getIdentifier());
  int bytesRequired = vd.getBytes();
  MIPS::ADDI(MIPS::sp, MIPS::sp, -bytesRequired);
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;
  return MIPS::Register();
}
MIPS::Register GenerateMIPS::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  int fpOffset = ve.variableDecl->fpOffset;
  MIPS::Register valReg = getTempRegister();
  MIPS::LW(valReg, MIPS::fp, fpOffset);
  return valReg;
}
MIPS::Register GenerateMIPS::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return MIPS::Register();
}