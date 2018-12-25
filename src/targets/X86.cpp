
#include "../../include/targets/X86.h"

using namespace ACC::X86;

/* ---- X86::Register --- */

Register::Register() : bits(-1), name("UNIMPLEMENTED") {}

Register::Register(int bits, std::string name) : bits(bits), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (bits == rhs.bits && name == rhs.name);
}

std::string Register::toString() const { return name; }

std::ostream &operator<<(std::ostream &stream, const Register &reg) {
  return stream << reg.toString();
}

/* ---- X86::Writer ---- */

Writer::Writer(std::string filename) { x86Output.open(filename); }

void Writer::write(const std::string &str) { x86Output << str << std::endl; }

void Writer::comment(const std::string &comment) {
  x86Output << ";" << comment << std::endl;
}

void Writer::block(std::string blockName, const std::string &comment) {
  if (macOS)
    blockName = "_" + blockName;
  x86Output << "\n" << blockName << ":" << std::endl;
}

void Writer::push(const Register &reg, const std::string &comment) {
  x86Output << "push " << reg << std::endl;
}

void Writer::pop(const Register &reg, const std::string &comment) {
  x86Output << "pop " << reg << std::endl;
}

void Writer::call(const std::string &ident, const std::string &comment) {
  if (macOS)
    x86Output << "call _" << ident << "FunDecl" << std::endl;
  else
    x86Output << "call " << ident << "FunDecl" << std::endl;
}

void Writer::ret(const std::string &comment) {
  x86Output << "ret" << std::endl;
}

void Writer::mov(const Register &dst, const Register &src,
                 const std::string &comment) {
  x86Output << "mov " << dst << ", " << src << std::endl;
}

void Writer::sub(const Register &reg, const int value,
                 const std::string &comment) {
  x86Output << "sub " << reg << ", " << value << "\t;" << comment << std::endl;
}

void Writer::cmp(const Register &reg, const int value,
                 const std::string &comment) {
  x86Output << "cmp " << reg << ", " << value << std::endl;
}

void Writer::jeq(const std::string &label, const std::string &comment) {
  x86Output << "jeq " << label << std::endl;
}

void Writer::jmp(const std::string &label, const std::string &comment) {
  x86Output << "jmp " << label << std::endl;
}