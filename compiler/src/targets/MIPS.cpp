
#include "../../include/targets/MIPS.h"

using namespace ACC::MIPS;

/* ---- MIPS::Register --- */

Register::Register() : num(-1), name("") {}

Register::Register(const int num, const atl::string &name)
    : num(num), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (num == rhs.num && name == rhs.name);
}

atl::string Register::toString() const { return atl::string("$") + name; }

std::ostream &operator<<(std::ostream &stream, const Register &reg) {
  return stream << reg.toString().c_str();
}

/* ---- MIPS::Writer --- */

Writer::Writer(const atl::string &filename) {
  std::string std_filename = filename.c_str();
  mipsOutput.open(std_filename);
}

void Writer::ADDI(const MIPS::Register &dest, const MIPS::Register &src,
                  const int val) {
  atl::string output = atl::string("ADDI ") + dest.toString() + ", " +
                       src.toString() + ", " + std::to_string(val).c_str();
  output += atl::string("\t# ") + dest.toString() + " = " + src.toString() +
            " + " + std::to_string(val).c_str();
  write(output);
}

void Writer::alloc(const atl::string &varName, const int size) {
  atl::string output = varName + ": .space" + std::to_string(size).c_str();
  write(output);
}

void Writer::BLOCK(const atl::string &blockName) { write(blockName + ":"); }

void Writer::comment(const atl::string &comment) {
  write(atl::string("#") + comment);
}

void Writer::JAL(const atl::string &blockName) {
  write(atl::string("JAL ") + blockName);
}

void Writer::LW(const MIPS::Register &destReg, const int addr,
                const int offset) {
  atl::string output = atl::string("LW ") + destReg.toString() + ", " +
                       std::to_string(offset).c_str() + "(" +
                       std::to_string(addr).c_str() + ")";
  output += atl::string("\t# ") + destReg.toString() + " = " +
            std::to_string(offset).c_str() + "(" +
            std::to_string(addr).c_str() + ")";
  write(output);
}

void Writer::LW(const MIPS::Register &destReg, const MIPS::Register &addr,
                const int offset) {
  atl::string output = atl::string("LW ") + destReg.toString() + ", " +
                       std::to_string(offset).c_str() + "(" + addr.toString() +
                       ")";
  output += atl::string("\t# ") + destReg.toString() + " = " +
            std::to_string(offset).c_str() + "(" + addr.toString() + ")";
  write(output);
}

void Writer::SW(const MIPS::Register &regCtnt, const int addr,
                const int offset) {
  atl::string output = atl::string("SW ") + regCtnt.toString() + ", " +
                       std::to_string(offset).c_str() + "(" +
                       std::to_string(addr).c_str() + ")";
  output += atl::string("\t# ") + std::to_string(offset).c_str() + "(" +
            std::to_string(addr).c_str() + ") = " + regCtnt.toString();
  write(output);
}

void Writer::SW(const MIPS::Register &regCtnt, const MIPS::Register &addr,
                const int offset) {
  atl::string output = atl::string("SW ") + regCtnt.toString() + ", " +
                       std::to_string(offset).c_str() + "(" + addr.toString() +
                       ")";
  output += atl::string("\t# ") + std::to_string(offset).c_str() + "(" +
            addr.toString() + ") = " + regCtnt.toString();
  write(output);
}

void Writer::write(const atl::string &str) {
  mipsOutput << str.c_str() << std::endl;
}