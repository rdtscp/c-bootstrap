
#include "targets/X86.h"

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

void Writer::add(const atl::shared_ptr<X86::Operand> &op1,
                 const atl::shared_ptr<X86::Operand> &op2,
                 const atl::string &comment) {
  atl::string output = "add " + op1->toString() + ", " + op2->toString() +
                       "\t; eax = " + op1->toString() + " + " + op2->toString();
  if (comment != "")
    output += "\t; " + comment;
  write(output);
}

void Writer::block(atl::string blockName, const atl::string &comment) {
  write("\n" + blockName + ":" + "\t" + comment);
}

void Writer::call(const atl::string &ident, const atl::string &comment) {
  write("call " + ident + "FunDecl");
}

void Writer::cmp(const atl::shared_ptr<X86::Operand> &op, const int value,
                 const atl::string &comment) {
  write("cmp " + op->toString() + ", " + atl::to_string(value));
}

void Writer::comment(const atl::string &comment) { write(";" + comment); }

void Writer::imul(const atl::shared_ptr<X86::Operand> &op1,
                  const atl::shared_ptr<X86::Operand> &op2,
                  const atl::string &comment) {
  atl::string output = "imul " + op1->toString() + ", " + op2->toString() +
                       "\t; eax = " + op1->toString() + " * " + op2->toString();
  if (comment != "")
    output += "\t; " + comment;
  write(output);
}

void Writer::jeq(const atl::string &label, const atl::string &comment) {
  write("jeq " + label);
}

void Writer::jmp(const atl::string &label, const atl::string &comment) {
  write("jmp " + label);
}

void Writer::mov(const atl::shared_ptr<X86::Operand> &dst,
                 const atl::shared_ptr<X86::Operand> &src,
                 const atl::string &comment) {
  if (dst == src)
    return;

  atl::string dstStr = dst->toString();
  atl::string srcStr = src->toString();
  if (dst->opType() == "GlobalVariable")
    dstStr = "[" + dstStr + "]";
  if (src->opType() == "GlobalVariable")
    srcStr = "[" + srcStr + "]";

  atl::string output = "mov " + dstStr + ", " + srcStr;
  if (comment != "")
    output += "\t; " + comment;
  write(output);
}

void Writer::pop(const atl::shared_ptr<X86::Operand> &op,
                 const atl::string &comment) {
  atl::string output = "pop dword " + op->toString();
  if (comment != "")
    output += "\t; " + comment;
  write(output);
}

void Writer::push(const atl::shared_ptr<X86::Operand> &op,
                  const atl::string &comment) {
  atl::string output = "push dword " + op->toString();
  if (comment != "")
    output += "\t; " + comment;
  write(output);
}

void Writer::ret(const atl::string &comment) {
  x86Output << "ret" << std::endl;
}

void Writer::sub(const atl::shared_ptr<X86::Operand> &op, const int value,
                 const atl::string &comment) {
  write("sub " + op->toString() + ", " + atl::to_string(value) + "\t; " +
        comment);
}

void Writer::write(const atl::string &str) {
  x86Output << str.c_str() << std::endl;
}