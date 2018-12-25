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

class Register {
public:
  int bits;
  std::string name;

  Register();
  Register(int bits, std::string name);
  bool operator==(const Register &rhs);
  std::string toString() const;
};

class Writer {
public:
  bool macOS = true;

  Writer(std::string filename);

  void write(const std::string &str);

  void comment(const std::string &comment);

  void block(std::string blockName, const std::string &comment = "");

  void push(const X86::Register &reg, const std::string &comment = "");

  void pop(const Register &reg, const std::string &comment = "");

  void call(const std::string &ident, const std::string &comment = "");

  void ret(const std::string &comment = "");
  void mov(const Register &dst, const Register &src,
           const std::string &comment = "");

  void sub(const Register &reg, const int value,
           const std::string &comment = "");

  void cmp(const Register &reg, const int value,
           const std::string &comment = "");

  void jeq(const std::string &label, const std::string &comment = "");

  void jmp(const std::string &label, const std::string &comment = "");

private:
  std::ofstream x86Output;
};

static Register eax(32, "eax");
static Register ebx(32, "ebx");
static Register ecx(32, "ecx");
static Register edx(32, "edx");
static Register esi(32, "esi");
static Register edi(32, "edi");
static Register esp(32, "esp");
static Register ebp(32, "ebp");

/* ---- X86 Operations ---- */

}; // namespace X86

}; // namespace ACC

#endif