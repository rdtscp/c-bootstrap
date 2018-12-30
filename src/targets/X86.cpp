
#include "../../include/targets/X86.h"

using namespace ACC::X86;

/* ---- X86::Register --- */

Register::Register(int bits, std::string name) : bits(bits), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (bits == rhs.bits && name == rhs.name);
}

std::string Register::opType() const { return "Register"; }

std::string Register::toString() const { return name; }

/* ---- X86::GlobalVariable --- */

GlobalVariable::GlobalVariable(std::string name) : name(name) {}

bool GlobalVariable::operator==(const GlobalVariable &rhs) {
  return (name == rhs.name);
}

std::string GlobalVariable::opType() const { return "GlobalVariable"; }

std::string GlobalVariable::toString() const { return name; }

/* ---- X86::IntValue --- */

IntValue::IntValue(std::string val) : val(val) {}

bool IntValue::operator==(const IntValue &rhs) { return (val == rhs.val); }

std::string IntValue::opType() const { return "IntValue"; }

std::string IntValue::toString() const { return val; }

/* ---- X86::None --- */

std::string None::opType() const { return "None"; }

std::string None::toString() const { return "None::INTERNAL_ERROR"; }

/* ---- X86::Writer ---- */

Writer::Writer(std::string filename) { x86Output.open(filename); }

void Writer::add(const std::shared_ptr<X86::Operand> &op1,
                 const std::shared_ptr<X86::Operand> &op2,
                 const std::string &comment) {
  std::string output = "add " + op1->toString() + ", " + op2->toString() +
                       "\t; eax = " + op1->toString() + " + " + op2->toString();
  if (comment != "")
    output += "\t" + comment;
  write(output);
}

void Writer::block(std::string blockName, const std::string &comment) {
  x86Output << "\n" << blockName << ":" << std::endl;
}

void Writer::call(const std::string &ident, const std::string &comment) {
  x86Output << "call " << ident << "FunDecl" << std::endl;
}

void Writer::cmp(const std::shared_ptr<X86::Operand> &op, const int value,
                 const std::string &comment) {
  x86Output << "cmp " << op->toString() << ", " << value << std::endl;
}

void Writer::comment(const std::string &comment) {
  x86Output << ";" << comment << std::endl;
}

void Writer::imul(const std::shared_ptr<X86::Operand> &op1,
                  const std::shared_ptr<X86::Operand> &op2,
                  const std::string &comment) {
  std::string output = "imul " + op1->toString() + ", " + op2->toString() +
                       "\t; eax = " + op1->toString() + " * " + op2->toString();
  if (comment != "")
    output += "\t" + comment;
  write(output);
}

void Writer::jeq(const std::string &label, const std::string &comment) {
  x86Output << "jeq " << label << std::endl;
}

void Writer::jmp(const std::string &label, const std::string &comment) {
  x86Output << "jmp " << label << std::endl;
}

void Writer::mov(const std::shared_ptr<X86::Operand> &dst,
                 const std::shared_ptr<X86::Operand> &src,
                 const std::string &comment) {
  std::string output = "mov " + dst->toString() + ", " + src->toString();
  if (dst->opType() == "GlobalVariable")
    output = "mov [" + dst->toString() + "], " + src->toString();

  if (comment != "")
    output += "\t; " + comment;

  write(output);
}

void Writer::pop(const std::shared_ptr<X86::Operand> &op,
                 const std::string &comment) {
  x86Output << "pop " << op->toString() << std::endl;
}

void Writer::push(const std::shared_ptr<X86::Operand> &op,
                  const std::string &comment) {
  x86Output << "push " << op->toString() << std::endl;
}

void Writer::ret(const std::string &comment) {
  x86Output << "ret" << std::endl;
}

void Writer::sub(const std::shared_ptr<X86::Operand> &op, const int value,
                 const std::string &comment) {
  x86Output << "sub " << op->toString() << ", " << value << "\t; " << comment
            << std::endl;
}

void Writer::write(const std::string &str) { x86Output << str << std::endl; }