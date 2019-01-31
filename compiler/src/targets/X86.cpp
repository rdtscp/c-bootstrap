
#include "../../include/targets/X86.h"

using namespace ACC::X86;

/* ---- X86::Register --- */

Register::Register(const int bits, const atl::string &name)
    : bits(bits), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (bits == rhs.bits && name == rhs.name);
}

atl::string Register::opType() const { return "Register"; }

atl::string Register::toString() const { return name; }

/* ---- X86::GlobalVariable --- */

GlobalVariable::GlobalVariable(const atl::string &name, int size)
    : name(name), size(size) {}

bool GlobalVariable::operator==(const GlobalVariable &rhs) {
  return (name == rhs.name);
}

atl::string GlobalVariable::opType() const { return "GlobalVariable"; }

atl::string GlobalVariable::toString() const { return name; }

/* ---- X86::IntValue --- */

IntValue::IntValue(const atl::string &val) : val(val) {}

bool IntValue::operator==(const IntValue &rhs) { return (val == rhs.val); }

atl::string IntValue::opType() const { return "IntValue"; }

atl::string IntValue::toString() const { return val; }

/* ---- X86::None --- */

atl::string None::opType() const { return "None"; }

atl::string None::toString() const { return "None::INTERNAL_ERROR"; }

/* ---- X86::Writer ---- */

Writer::Writer(const atl::string &filename) {
  std::string std_filename(filename.c_str());
  x86Output.open(std_filename);
}

void Writer::add(const std::shared_ptr<X86::Operand> &op1,
                 const std::shared_ptr<X86::Operand> &op2,
                 const atl::string &comment) {
  atl::string output = atl::string("add ") + op1->toString() + ", " +
                       op2->toString() + "\t; eax = " + op1->toString() +
                       " + " + op2->toString();
  if (comment != "")
    output += atl::string("\t; ") + comment;
  write(output);
}

void Writer::block(atl::string blockName, const atl::string &comment) {
  write(atl::string("\n") + blockName + ":");
}

void Writer::call(const atl::string &ident, const atl::string &comment) {
  write(atl::string("call ") + ident + "FunDecl");
}

void Writer::cmp(const std::shared_ptr<X86::Operand> &op, const int value,
                 const atl::string &comment) {
  write(atl::string("cmp ") + op->toString() + ", " +
        std::to_string(value).c_str());
}

void Writer::comment(const atl::string &comment) {
  write(atl::string(";") + comment);
}

void Writer::imul(const std::shared_ptr<X86::Operand> &op1,
                  const std::shared_ptr<X86::Operand> &op2,
                  const atl::string &comment) {
  atl::string output = atl::string("imul ") + op1->toString() + ", " +
                       op2->toString() + "\t; eax = " + op1->toString() +
                       " * " + op2->toString();
  if (comment != "")
    output += atl::string("\t; ") + comment;
  write(output);
}

void Writer::jeq(const atl::string &label, const atl::string &comment) {
  write(atl::string("jeq ") + label);
}

void Writer::jmp(const atl::string &label, const atl::string &comment) {
  write(atl::string("jmp ") + label);
}

void Writer::mov(const std::shared_ptr<X86::Operand> &dst,
                 const std::shared_ptr<X86::Operand> &src,
                 const atl::string &comment) {
  if (dst == src)
    return;

  atl::string dstStr = dst->toString();
  atl::string srcStr = src->toString();
  if (dst->opType() == "GlobalVariable")
    dstStr = atl::string("[") + dstStr + atl::string("]");
  if (src->opType() == "GlobalVariable")
    srcStr = atl::string("[") + srcStr + "]";

  atl::string output = atl::string("mov ") + dstStr + ", " + srcStr;
  if (comment != "")
    output += atl::string("\t; ") + comment;
  write(output);
}

void Writer::pop(const std::shared_ptr<X86::Operand> &op,
                 const atl::string &comment) {
  atl::string output = atl::string("pop dword ") + op->toString();
  if (comment != "")
    output += atl::string("\t; ") + comment;
  write(output);
}

void Writer::push(const std::shared_ptr<X86::Operand> &op,
                  const atl::string &comment) {
  atl::string output = atl::string("push dword ") + op->toString();
  if (comment != "")
    output += atl::string("\t; ") + comment;
  write(output);
}

void Writer::ret(const atl::string &comment) {
  x86Output << "ret" << std::endl;
}

void Writer::sub(const std::shared_ptr<X86::Operand> &op, const int value,
                 const atl::string &comment) {
  write(atl::string("sub ") + op->toString() + ", " +
        std::to_string(value).c_str() + "\t; " + comment);
}

void Writer::write(const atl::string &str) {
  x86Output << str.c_str() << std::endl;
}