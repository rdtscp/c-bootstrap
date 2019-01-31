#pragma once

#ifndef ACC_X86_H
#define ACC_X86_H

#include "atl/include/string.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace ACC {

namespace X86 {

class Operand {
public:
  virtual ~Operand() {}

  virtual atl::string opType() const = 0;
  virtual atl::string toString() const = 0;
};

class Register : public Operand {
public:
  const int bits;
  const atl::string name;

  Register(int bits, const atl::string &name);
  bool operator==(const Register &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class GlobalVariable : public Operand {
public:
  const atl::string name;
  const int size;

  GlobalVariable(const atl::string &name, int size);
  bool operator==(const GlobalVariable &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class IntValue : public Operand {
public:
  const atl::string val;

  IntValue(const atl::string &val);
  bool operator==(const IntValue &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class None : public Operand {
public:
  atl::string opType() const override;
  atl::string toString() const override;
};

class Writer final {
public:
  Writer(const atl::string &filename);

  void add(const std::shared_ptr<X86::Operand> &op1,
           const std::shared_ptr<X86::Operand> &op2,
           const atl::string &comment = "");
  void block(atl::string blockName, const atl::string &comment = "");
  void call(const atl::string &ident, const atl::string &comment = "");
  void cmp(const std::shared_ptr<X86::Operand> &reg, const int value,
           const atl::string &comment = "");
  void comment(const atl::string &comment);
  void imul(const std::shared_ptr<X86::Operand> &dst,
            const std::shared_ptr<X86::Operand> &src,
            const atl::string &comment = "");
  void jeq(const atl::string &label, const atl::string &comment = "");
  void jmp(const atl::string &label, const atl::string &comment = "");
  void mov(const std::shared_ptr<X86::Operand> &dst,
           const std::shared_ptr<X86::Operand> &src,
           const atl::string &comment = "");
  void pop(const std::shared_ptr<X86::Operand> &reg,
           const atl::string &comment = "");
  void push(const std::shared_ptr<X86::Operand> &op,
            const atl::string &comment = "");
  void ret(const atl::string &comment = "");
  void sub(const std::shared_ptr<X86::Operand> &reg, const int value,
           const atl::string &comment = "");
  void write(const atl::string &str);

private:
  std::ofstream x86Output;
};

static std::shared_ptr<Register> eax(new Register(32, "eax"));
static std::shared_ptr<Register> ebx(new Register(32, "ebx"));
static std::shared_ptr<Register> ecx(new Register(32, "ecx"));
static std::shared_ptr<Register> edx(new Register(32, "edx"));
static std::shared_ptr<Register> esi(new Register(32, "esi"));
static std::shared_ptr<Register> edi(new Register(32, "edi"));
static std::shared_ptr<Register> esp(new Register(32, "esp"));
static std::shared_ptr<Register> ebp(new Register(32, "ebp"));

}; // namespace X86

}; // namespace ACC

#endif