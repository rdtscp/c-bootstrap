
#include "../../include/targets/MIPS.h"

using namespace ACC::MIPS;

/* ---- MIPS::Register --- */

Register::Register() : num(-1), name("") {}

Register::Register(int num, std::string name) : num(num), name(name) {}

bool Register::operator==(const Register &rhs) {
  return (num == rhs.num && name == rhs.name);
}

std::string Register::toString() const { return "$" + name; }

std::ostream &operator<<(std::ostream &stream, const Register &reg) {
  return stream << reg.toString();
}

/* ---- MIPS::Writer --- */

Writer::Writer(std::string filename) { mipsOutput.open(filename); }

void Writer::ADDI(const MIPS::Register &dest, const MIPS::Register &src,
                  const int val) {
  mipsOutput << "ADDI " << dest.toString() << ", " << src.toString() << ", "
             << val;
  mipsOutput << "\t# " << dest.toString() << " = " << src.toString() << " + "
             << val;
  mipsOutput << std::endl;
}

void Writer::alloc(const std::string &varName, const int size) {
  mipsOutput << varName << ": .space " << size << std::endl;
}

void Writer::BLOCK(const std::string &blockName) {
  mipsOutput << blockName << ":" << std::endl;
}

void Writer::comment(const std::string &comment) {
  mipsOutput << "#" << comment << std::endl;
}

void Writer::JAL(const std::string &blockName) {
  mipsOutput << "JAL " << blockName << std::endl;
}

void Writer::LW(const MIPS::Register &destReg, const int addr,
                const int offset) {
  mipsOutput << "LW " << destReg.toString() << ", " << offset << "(" << addr
             << ")";
  mipsOutput << "\t# " << destReg.toString() << " = " << offset << "(" << addr
             << ")";
  mipsOutput << std::endl;
}

void Writer::LW(const MIPS::Register &destReg, const MIPS::Register &addr,
                const int offset) {
  mipsOutput << "LW " << destReg.toString() << ", " << offset << "("
             << addr.toString() << ")";
  mipsOutput << "\t# " << destReg.toString() << " = " << offset << "(" << addr
             << ")";
  mipsOutput << std::endl;
}

void Writer::SW(const MIPS::Register &regCtnt, const int addr,
                const int offset) {
  mipsOutput << "SW " << regCtnt.toString() << ", " << offset << "(" << addr
             << ")";
  mipsOutput << "\t# " << offset << "(" << addr << ") = " << regCtnt.toString();
  mipsOutput << std::endl;
}

void Writer::SW(const MIPS::Register &regCtnt, const MIPS::Register &addr,
                const int offset) {
  mipsOutput << "SW " << regCtnt.toString() << ", " << offset << "("
             << addr.toString() << ")";
  mipsOutput << "\t# " << offset << "(" << addr.toString()
             << ") = " << regCtnt.toString();
  mipsOutput << std::endl;
}

void Writer::write(const std::string &str) { mipsOutput << str << std::endl; }