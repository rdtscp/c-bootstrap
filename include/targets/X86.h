#pragma once

#ifndef X86_H
#define X86_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ACC {

namespace X86 {

/* ---- X86 Config ---- */
static bool macOS = true;

/* ---- X86 Output ---- */
static std::ofstream x86Output;

/* --- X86 Registers --- */

class Register {

public:
  int bits;
  std::string name;

  Register() : bits(-1), name("UNIMPLEMENTED") {}
  Register(int bits, std::string name) : bits(bits), name(name) {}
  bool operator==(const Register &rhs) {
    return (bits == rhs.bits && name == rhs.name);
  }
  std::string toString() const { return name; }
};
std::ostream &operator<<(std::ostream &stream, const Register &reg) {
  return stream << reg.toString();
}

static Register eax(32, "eax");
static Register ebx(32, "ebx");
static Register ecx(32, "ecx");
static Register edx(32, "edx");
static Register esi(32, "esi");
static Register edi(32, "edi");
static Register esp(32, "esp");
static Register ebp(32, "ebp");

/* ---- X86 Operations ---- */

static void write(const std::string &str) { x86Output << str << std::endl; }

static void comment(const std::string &comment) {
  x86Output << ";" << comment << std::endl;
}

static void block(std::string blockName, const std::string &comment = "") {
  if (macOS)
    blockName = "_" + blockName;
  x86Output << "\n" << blockName << ":" << std::endl;
}

static void push(const Register &reg, const std::string &comment = "") {
  x86Output << "push " << reg << std::endl;
}

static void pop(const Register &reg, const std::string &comment = "") {
  x86Output << "pop " << reg << std::endl;
}

static void call(const std::string &ident, const std::string &comment = "") {
  if (macOS)
    x86Output << "call _" << ident << "FunDecl" << std::endl;
  else
    x86Output << "call " << ident << "FunDecl" << std::endl;
}

static void ret(const std::string &comment = "") {
  x86Output << "ret" << std::endl;
}

static void mov(const Register &dst, const Register &src,
                const std::string &comment = "") {
  x86Output << "mov " << dst << ", " << src << std::endl;
}

static void sub(const Register &reg, const int value,
                const std::string &comment = "") {
  x86Output << "sub " << reg << ", " << value << "\t;" << comment << std::endl;
}

static void cmp(const Register &reg, const int value,
                const std::string &comment = "") {
  x86Output << "cmp " << reg << ", " << value << std::endl;
}

static void jeq(const std::string &label, const std::string &comment = "") {
  x86Output << "jeq " << label << std::endl;
}

static void jmp(const std::string &label, const std::string &comment = "") {
  x86Output << "jmp " << label << std::endl;
}

}; // namespace X86

}; // namespace ACC

#endif