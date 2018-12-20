#pragma once

#ifndef MIPS_H
#define MIPS_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ACC {

namespace MIPS {

/* ---- MIPS Output ---- */
static std::ofstream mipsOutput;

/* --- MIPS Registers --- */

class Register {

public:
  int num;
  std::string name;

  Register() : num(-1), name("") {}
  Register(int num, std::string name) : num(num), name(name) {}
  bool operator==(const Register &rhs) {
    return (num == rhs.num && name == rhs.name);
  }
  std::string toString() const { return "$" + name; }
};
std::ostream &operator<<(std::ostream &stream, const Register &reg) {
  return stream << reg.toString();
}

const static Register zero(0, "zero");
const static Register v0(2, "v0");
const static Register v1(3, "v0");
const static std::vector<Register> argsRegs = {
    Register(4, "a0"), Register(5, "a1"), Register(6, "a2"), Register(7, "a3")};

const static std::vector<Register> tempRegs = {
    Register(8, "t0"),  Register(9, "t1"),  Register(10, "t2"),
    Register(11, "t3"), Register(12, "t4"), Register(13, "t5"),
    Register(14, "t6"), Register(15, "t7"), Register(24, "t8"),
    Register(25, "t9")};

const static std::vector<Register> saveRegs = {
    Register(16, "s0"), Register(17, "s1"), Register(18, "s2"),
    Register(19, "s3"), Register(20, "s4"), Register(21, "s5"),
    Register(22, "s6"), Register(23, "s7")};

const static Register gp(28, "gp");
const static Register sp(29, "sp");
const static Register fp(30, "fp");
const static Register ra(31, "ra");

/* ---- MIPS Operations ---- */

static void comment(const std::string &comment) {
  mipsOutput << "#" << comment << std::endl;
}
static void ADDI(const MIPS::Register &dest, const MIPS::Register &src,
                 const int val) {
  mipsOutput << "ADDI " << dest.toString() << ", " << src.toString() << ", "
             << val;
  mipsOutput << "\t# " << dest.toString() << " = " << src.toString() << " + "
             << val;
  mipsOutput << std::endl;
}
static void JAL(const std::string &blockName) {
  mipsOutput << "JAL " << blockName << std::endl;
}
static void LW(const MIPS::Register &destReg, const int addr,
               const int offset = 0) {
  mipsOutput << "LW " << destReg.toString() << ", " << offset << "(" << addr
             << ")";
  mipsOutput << "\t# " << destReg.toString() << " = " << offset << "(" << addr
             << ")";
  mipsOutput << std::endl;
}
static void LW(const MIPS::Register &destReg, const MIPS::Register &addr,
               const int offset = 0) {
  mipsOutput << "LW " << destReg.toString() << ", " << offset << "("
             << addr.toString() << ")";
  mipsOutput << "\t# " << destReg.toString() << " = " << offset << "(" << addr
             << ")";
  mipsOutput << std::endl;
}
static void SW(const MIPS::Register &regCtnt, const int addr,
               const int offset = 0) {
  mipsOutput << "SW " << regCtnt.toString() << ", " << offset << "(" << addr
             << ")";
  mipsOutput << "\t# " << offset << "(" << addr << ") = " << regCtnt.toString();
  mipsOutput << std::endl;
}
static void SW(const MIPS::Register &regCtnt, const MIPS::Register &addr,
               const int offset = 0) {
  mipsOutput << "SW " << regCtnt.toString() << ", " << offset << "("
             << addr.toString() << ")";
  mipsOutput << "\t# " << offset << "(" << addr.toString()
             << ") = " << regCtnt.toString();
  mipsOutput << std::endl;
}

static void BLOCK(const std::string &blockName) {
  mipsOutput << blockName << ":" << std::endl;
}

}; // namespace MIPS

}; // namespace ACC

#endif