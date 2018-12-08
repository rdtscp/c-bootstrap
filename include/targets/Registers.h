#pragma once

#ifndef MIPSREGISTER_H
#define MIPSREGISTER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ACC {

namespace MIPS {

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

const static Register v0(2, "v0");
const static std::vector<Register> paramRegs = {
    Register(4, "a0"), Register(5, "a1"), Register(6, "a2"), Register(7, "a3")};

const static std::vector<Register> tmpRegs = {
    Register(8, "t0"),  Register(9, "t1"),  Register(10, "t2"),
    Register(11, "t3"), Register(12, "t4"), Register(13, "t5"),
    Register(14, "t6"), Register(15, "t7"),

    Register(16, "s0"), Register(17, "s1"), Register(18, "s2"),
    Register(19, "s3"), Register(20, "s4"), Register(21, "s5"),
    Register(22, "s6"), Register(23, "s7"),

    Register(24, "t8"), Register(25, "t9")};

const static Register gp(28, "gp");
const static Register sp(29, "sp");
const static Register fp(30, "fp");
const static Register ra(31, "ra");

}; // namespace MIPS
}; // namespace ACC

#endif