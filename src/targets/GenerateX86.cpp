
#include "../../include/targets/GenerateX86.h"

using namespace ACC;

GenerateX86::GenerateX86(std::shared_ptr<Program> progAST,
                         std::string outputFile)
    : x86(outputFile), progAST(progAST) {}

void GenerateX86::error(std::string error) {
  errorCount++;
  errors.push_back(error);
}

void GenerateX86::printErrors() const {
  std::cerr << "FATAL x86 Generation Errors:" << std::endl;
  for (const auto &error : errors)
    std::cerr << "\t" << error << std::endl;
}

void GenerateX86::run() { visit(*progAST); }

void GenerateX86::alloc(const VarDecl &vd) {
  const int size = vd.getBytes();

  std::string allocSize = "DB ?";
  if (size > 1)
    allocSize = "DW ?";
  if (size > 2)
    allocSize = "DD ?";
  if (size > 4)
    allocSize = "DB " + std::to_string(size) + "DUP(?)";

  x86.write(vd.getIdentifier() + " " + allocSize);
}

/* ---- Visit AST ---- */

std::shared_ptr<X86::Operand> GenerateX86::visit(ArrayAccess &aa) {
  aa.array->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(ArrayType &at) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(Assign &as) {
  std::shared_ptr<X86::Operand> lhsRegRes = as.lhs->accept(*this);
  std::shared_ptr<X86::Operand> rhsResReg = as.rhs->accept(*this);
  // x86.mov(lhsRegRes, rhsResReg);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(BaseType &bt) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(Block &b) {
  currScope = b.getptr();
  for (const auto &stmt : b.blockStmts)
    stmt->accept(*this);

  // /* Clean up the stack. */
  // for (const auto &ident_decl : b.blockDecls) {
  //   std::shared_ptr<Decl> currDecl = ident_decl.second;
  //   if (currDecl->astClass() == "VarDecl") {
  //     /* ---- Deconstruct VarDecl ---- */
  //   }
  // }
  currScope = b.outerBlock;
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(CharLiteral &cl) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(FieldAccess &fa) {
  fa.object->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(FunCall &fc) {
  std::vector<std::shared_ptr<Expr>> revArgs = fc.funArgs;
  std::reverse(std::begin(revArgs), std::end(revArgs));
  for (const auto &arg : revArgs) {
    std::shared_ptr<X86::Operand> argReg = arg->accept(*this);
    // x86.push(argReg);
  }
  x86.call(fc.funName);
  return std::make_shared<X86::Register>(X86::eax);
}
std::shared_ptr<X86::Operand> GenerateX86::visit(FunDecl &fd) {
  currScope = fd.funBlock;

  x86.block(fd.getIdentifier() + "FunDecl");

  /* ---- Callee Prologue ---- */
  x86.push(X86::ebp);
  x86.mov(X86::ebp, X86::esp);
  x86.push(X86::ebx);
  x86.push(X86::edi);
  x86.push(X86::esi);

  /* ---- Execute Function ---- */
  x86.comment(" ---- Function Body ----");
  fd.funBlock->accept(*this);
  x86.comment(" -----------------------");

  /* -------------------------- */

  /* ---- Callee Epilogue ---- */
  x86.pop(X86::esi);
  x86.pop(X86::edi);
  x86.pop(X86::ebx);
  x86.mov(X86::esp, X86::ebp);
  x86.pop(X86::ebp);
  x86.ret();
  x86.write("");

  currScope = fd.funBlock->outerBlock;
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(If &i) {
  /* Calculate Names for Blocks */
  const std::string trueBlockName =
      "ifTrueBlock" + std::to_string(blockCount++);
  const std::string falseBlockName =
      "ifFalseBlock" + std::to_string(blockCount++);
  const std::string endBlockName = "ifEndBlock" + std::to_string(blockCount++);

  /* Calculate the result of the if condition. */
  std::shared_ptr<X86::Operand> condResReg = i.ifCondition->accept(*this);

  /* Branch to False block if False, else branch to True block. */
  // x86.cmp(condResReg, 0);
  x86.jeq(falseBlockName);
  x86.jmp(trueBlockName);

  /* Handle when the Case is True. */
  x86.block(trueBlockName);
  i.ifBody->accept(*this);
  x86.jmp(endBlockName);

  /* Handle when the Case is False. */
  x86.block(falseBlockName);
  if (i.elseBody)
    i.elseBody->accept(*this);
  x86.jmp(endBlockName);

  /* Handle after the If statement. */
  x86.block(endBlockName);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(IntLiteral &il) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(ParenthExpr &pe) {
  pe.innerExpr->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(PointerType &pt) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(Program &p) {
  currScope = p.globalScope;

  x86.write(".DATA");
  for (std::shared_ptr<VarDecl> globalVar : p.globalVars)
    alloc(*globalVar);

  if (x86.macOS)
    x86.write(".globl _mainFunDecl");
  else
    x86.write(".globl mainFunDecl");

  for (std::shared_ptr<FunDecl> func : p.funDecls)
    func->accept(*this);

  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(SizeOf &so) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(StringLiteral &sl) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(StructType &st) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(StructTypeDecl &std) {
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(TypeCast &tc) {
  return tc.expr->accept(*this);
}
std::shared_ptr<X86::Operand> GenerateX86::visit(ValueAt &va) {
  va.derefExpr->accept(*this);
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(VarDecl &vd) {
  int bytesRequired = vd.getBytes();
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  x86.sub(X86::esp, bytesRequired,
          "Allocated for VarDecl: " + vd.getIdentifier() +
              " with fpOffset: " + std::to_string(currFpOffset));
  return std::shared_ptr<X86::Operand>();
}
std::shared_ptr<X86::Operand> GenerateX86::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  int fpOffset = ve.variableDecl->fpOffset;
  x86.comment("visit(VarExpr) with offset: " + std::to_string(fpOffset));
  if (fpOffset == 0)
    return std::make_shared<X86::Register>(X86::Register(0, ve.variableDecl->getIdentifier()));

  if (fpOffset > 0)
    return std::make_shared<X86::Register>(X86::Register(0, "[ebp+" + std::to_string(fpOffset) +
                                                "]"));
  else
    return std::make_shared<X86::Register>(X86::Register(0, "[ebp-" + std::to_string(fpOffset) +
                                                "]"));
}
std::shared_ptr<X86::Operand> GenerateX86::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return std::shared_ptr<X86::Operand>();
}