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

class Operand {
public:
  virtual std::string opType() const = 0;
  virtual std::string toString() const = 0;
};

class Register : public Operand {
public:
  int bits;
  std::string name;

  Register(int bits, std::string name);
  bool operator==(const Register &rhs);
  std::string opType() const override;
  std::string toString() const override;
};

class GlobalVariable : public Operand {
public:
  std::string name;

  GlobalVariable(std::string name);
  bool operator==(const GlobalVariable &rhs);
  std::string opType() const override;
  std::string toString() const override;
};

class IntValue : public Operand {
public:
  std::string val;

  IntValue(std::string val);
  bool operator==(const IntValue &rhs);
  std::string opType() const override;
  std::string toString() const override;
};

class None : public Operand {
public:
  std::string opType() const override;
  std::string toString() const override;
};

class Writer {
public:
  Writer(std::string filename);

  void add(const std::shared_ptr<X86::Operand> &op1,
           const std::shared_ptr<X86::Operand> &op2,
           const std::string &comment = "");
  void block(std::string blockName, const std::string &comment = "");
  void call(const std::string &ident, const std::string &comment = "");
  void comment(const std::string &comment);
  void push(const std::shared_ptr<X86::Operand> &op,
            const std::string &comment = "");
  void pop(const std::shared_ptr<X86::Operand> &reg,
           const std::string &comment = "");
  void ret(const std::string &comment = "");
  void mov(const std::shared_ptr<X86::Operand> &dst,
           const std::shared_ptr<X86::Operand> &src,
           const std::string &comment = "");
  void sub(const std::shared_ptr<X86::Operand> &reg, const int value,
           const std::string &comment = "");
  void cmp(const std::shared_ptr<X86::Operand> &reg, const int value,
           const std::string &comment = "");
  void jeq(const std::string &label, const std::string &comment = "");
  void jmp(const std::string &label, const std::string &comment = "");
  void write(const std::string &str);

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