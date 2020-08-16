#include "targets/X64.h"

#include "Error.h"

using namespace ACC::X64;

/* ---- X64::AddrOffset --- */

AddrOffset::AddrOffset(const atl::shared_ptr<X64::Operand> p_addrOperand,
                       const int p_offset)
    : addrOperand(p_addrOperand), offset(p_offset) {}

atl::string AddrOffset::opType() const { return "AddrOffset"; }

atl::string AddrOffset::toString() const {
  // Accumulate the offsets and get the address to offset.
  int byteOffset = offset;
  atl::shared_ptr<X64::Operand> addressOp = addrOperand;
  while (addressOp->opType() == "AddrOffset") {
    atl::shared_ptr<X64::AddrOffset> addrOffset =
        atl::static_pointer_cast<AddrOffset>(addressOp);
    byteOffset += addrOffset->offset;
    addressOp = addrOffset->addrOperand;
  }

  if (byteOffset < 0)
    return "[" + addressOp->toString() + atl::to_string(byteOffset) + "]";
  else if (byteOffset > 0)
    return "[" + addressOp->toString() + "+" + atl::to_string(byteOffset) + "]";
  else
    return "[" + addressOp->toString() + "]";
}

/* ---- X64::Register --- */

Register::Register(const int bits, const atl::string &name)
    : bits(bits), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (bits == rhs.bits && name == rhs.name);
}

atl::string Register::opType() const { return "Register"; }

atl::string Register::toString() const { return name; }

/* ---- X64::GlobalVariable --- */

GlobalVariable::GlobalVariable(const atl::string &name, int size)
    : name(name), size(size) {}

bool GlobalVariable::operator==(const GlobalVariable &rhs) {
  return (name == rhs.name);
}

atl::string GlobalVariable::opType() const { return "GlobalVariable"; }

atl::string GlobalVariable::toString() const { return name; }

/* ---- X64::IntValue --- */

IntValue::IntValue(const int val) : val(atl::to_string(val)) {}

IntValue::IntValue(const atl::string &val) : val(val) {}

bool IntValue::operator==(const IntValue &rhs) { return (val == rhs.val); }

atl::string IntValue::opType() const { return "IntValue"; }

atl::string IntValue::toString() const { return val; }

/* ---- X64::None --- */

atl::string None::opType() const { return "None"; }

atl::string None::toString() const { return "None::INTERNAL_ERROR"; }

/* ---- X64::StringLiteral --- */

StringLiteral::StringLiteral(const atl::string &p_strName,
                             const atl::string &p_strVal)
    : strName(p_strName), strVal(p_strVal) {}

atl::string StringLiteral::opType() const { return "StringLiteral"; }

atl::string StringLiteral::toString() const { return strName; }

/* ---- X64::Writer ---- */

Writer::Writer(const atl::shared_ptr<SourceHandler> &output)
    : rax(new Register(64, "rax")), eax(new Register(32, "eax")),
      ax(new Register(16, "ax")), al(new Register(8, "al")),

      rcx(new Register(64, "rcx")), ecx(new Register(32, "ecx")),
      cx(new Register(16, "cx")), cl(new Register(8, "cl")),

      rbx(new Register(64, "rbx")), rdx(new Register(64, "rdx")),
      rsi(new Register(64, "rsi")), rdi(new Register(64, "rdi")),
      rsp(new Register(64, "rsp")), rbp(new Register(64, "rbp")),
      r12(new Register(64, "r12")), x64Output(output) {}

void Writer::add(const atl::shared_ptr<X64::Operand> &dst,
                 const atl::shared_ptr<X64::Operand> &src,
                 const atl::string &comment) {
  atl::string assembly = "add " + dst->toString() + ", " + src->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::block(const atl::string &blockName, const atl::string &comment) {
  atl::string assembly = blockName + ":";
  if (comment != "")
    assembly += "\t: " + comment;

  write("");
  write(assembly);
}

void Writer::call(const atl::string &ident, const atl::string &comment) {
  atl::string assembly = "call FunDecl_" + ident;
  if (comment != "")
    assembly += "\t: " + comment;

  write(assembly);
}

void Writer::cmp(const atl::shared_ptr<X64::Operand> &op1,
                 const atl::shared_ptr<X64::Operand> &op2,
                 const atl::string &comment) {
  atl::string assembly = "cmp " + op1->toString() + ", " + op2->toString();
  if (comment != "")
    assembly += "\t: " + comment;

  write(assembly);
}

void Writer::comment(const atl::string &comment) { write(";" + comment); }

void Writer::idiv(const atl::shared_ptr<X64::Register> &op,
                  const atl::string &comment) {
  atl::string assembly = "idiv " + op->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::imul(const atl::shared_ptr<X64::Operand> &op1,
                  const atl::shared_ptr<X64::Operand> &op2,
                  const atl::string &comment) {
  atl::string assembly = "imul " + op1->toString() + ", " + op2->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::je(const atl::string &label, const atl::string &comment) {
  atl::string assembly = "je " + label;
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::jmp(const atl::string &label, const atl::string &comment) {
  atl::string assembly = "jmp " + label;
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::lea(const atl::shared_ptr<X64::Operand> &dst,
                 const atl::shared_ptr<X64::Operand> &src,
                 const atl::string &comment) {
  atl::string assembly = "lea " + dst->toString() + ", " + src->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::mov(const atl::shared_ptr<X64::Operand> &dst,
                 const atl::shared_ptr<X64::Operand> &src,
                 const atl::string &comment) {
  if (dst == src)
    return;

  atl::string dstStr = dst->toString();
  atl::string srcStr = src->toString();
  if (dst->opType() == "GlobalVariable")
    dstStr = "[" + dstStr + "]";
  if (src->opType() == "GlobalVariable")
    srcStr = "[" + srcStr + "]";

  atl::string assembly = "mov " + dstStr + ", " + srcStr;
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::pop(const atl::shared_ptr<X64::Operand> &op,
                 const atl::string &comment) {
  atl::string assembly = "pop " + op->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::push(const atl::shared_ptr<X64::Operand> &op,
                  const atl::string &comment) {
  atl::string assembly = "push " + op->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::ret(const atl::string &comment) {
  atl::string assembly = "ret";
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::string_literal(const atl::string &strName,
                            const atl::string &strValue) {
  atl::string writtenValue;
  for (unsigned int idx = 0u; idx < strValue.size(); ++idx) {
    const char curr = strValue[idx];
    if (curr == '\\') {
      const char next = strValue[idx + 1];
      switch (next) {
      case '\'':
        writtenValue += "\", 0x27, \"";
        break;
      case '"':
        writtenValue += "\", 0x22, \"";
        break;
      case '?':
        writtenValue += "\", 0x3f, \"";
        break;
      case '\\':
        writtenValue += "\", 0x5c, \"";
        break;
      case 'a':
        writtenValue += "\", 0x07, \"";
        break;
      case 'b':
        writtenValue += "\", 0x08, \"";
        break;
      case 'f':
        writtenValue += "\", 0x0c, \"";
        break;
      case 'n':
        writtenValue += "\", 0x0a, \"";
        break;
      case 'r':
        writtenValue += "\", 0x0d, \"";
        break;
      case 't':
        writtenValue += "\", 0x09, \"";
        break;
      case 'v':
        writtenValue += "\", 0x0b, \"";
        break;
      }
      ++idx;
    } else {
      writtenValue += curr;
    }
  }
  // TODO: Replace '\n' and other special characters with [", ASCII(c), "]
  write(strName + ":\tdb \"" + writtenValue + "\", 0");
}

void Writer::sub(const atl::shared_ptr<X64::Operand> &dst,
                 const atl::shared_ptr<X64::Operand> &src,
                 const atl::string &comment) {
  atl::string assembly = "sub " + dst->toString() + ", " + src->toString();
  if (comment != "")
    assembly += "\t; " + comment;

  write(assembly);
}

void Writer::write(const atl::string &str) {
  // printf("%s\n", str.c_str());
  x64Output->write(indentation + str + "\n");
}

/* Helpers */
void Writer::calleePrologue() {
  comment(" ---- Callee Prologue ----");
  indent();
  // sub(rsp, 8);
  push(rbp);
  push(rbx);
  push(rdi);
  push(rsi);
  mov(rbp, rsp);
  unindent();
  comment(" -------------------------");
}

void Writer::calleeEpilogue() {
  comment(" ---- Callee Epilogue ----");
  indent();
  mov(rsp, rbp);
  pop(rsi);
  pop(rdi);
  pop(rbx);
  pop(rbp);
  unindent();
  // add(rsp, atl::shared_ptr<X64::IntValue>(new X64::IntValue(8)));
  comment(" -------------------------");
}

void Writer::callerPrologue() {
  comment(" ---- Caller Prologue ----");
  indent();
  push(rdi);
  push(rsi);
  push(rdx);
  push(rcx); // R8, R9
  unindent();
  comment(" -------------------------");
}

void Writer::callerEpilogue() {
  comment(" ---- Caller Epilogue ----");
  indent();
  pop(rcx);
  pop(rdx);
  pop(rsi);
  pop(rdi);
  unindent();
  comment(" -------------------------");
}

void Writer::indent() { indentation += "  "; }
void Writer::unindent() {
  if (indentation.size() < 2) {
    printf("INTERNAL_ERROR: Unindent Failure");
    throw;
  }

  indentation = atl::string(indentation.size() - 2, ' ');
}

atl::shared_ptr<Register>
Writer::getTempReg(const unsigned int num_bytes) const {
  switch (num_bytes) {
  case 1:
    return al;
  case 2:
    return ax;
  case 4:
    return eax;
  case 8:
    return rax;
  }
  printf("INTERNAL_ERROR: Invalid number of bytes requested for a register: %d",
         num_bytes);
  throw;
}

atl::stack<atl::shared_ptr<Register>> Writer::paramRegs() const {
  atl::stack<atl::shared_ptr<Register>> output;
  output.push_back(rcx);
  output.push_back(rdx);
  output.push_back(rsi);
  output.push_back(rdi);
  return output;
}

void Writer::writeStringLiterals() {
  write("SECTION .data");
  write(stringLiterals);
}