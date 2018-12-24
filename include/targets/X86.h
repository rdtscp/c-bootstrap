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

/* ---- X86 Output ---- */
static std::ofstream x86Output;

/* --- X86 Registers --- */

class Register {

public:
  int bits;
  std::string name;

  Register() : bits(-1), name("") {}
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
static Register eax(32, "ebx");
static Register eax(32, "ecx");
static Register eax(32, "edx");
static Register eax(32, "esi");
static Register eax(32, "edi");
static Register eax(32, "esp");
static Register eax(32, "ebp");

/* ---- X86 Operations ---- */

static void comment(const std::string &comment) {
  x86Output << ";" << comment << std::endl;
}

}; // namespace X86

}; // namespace ACC

#endif