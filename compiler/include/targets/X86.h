#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"
#include <fstream>

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

  void add(const atl::shared_ptr<X86::Operand> &op1,
           const atl::shared_ptr<X86::Operand> &op2,
           const atl::string &comment = "");
  void block(atl::string blockName, const atl::string &comment = "");
  void call(const atl::string &ident, const atl::string &comment = "");
  void cmp(const atl::shared_ptr<X86::Operand> &reg, const int value,
           const atl::string &comment = "");
  void comment(const atl::string &comment);
  void imul(const atl::shared_ptr<X86::Operand> &dst,
            const atl::shared_ptr<X86::Operand> &src,
            const atl::string &comment = "");
  void jeq(const atl::string &label, const atl::string &comment = "");
  void jmp(const atl::string &label, const atl::string &comment = "");
  void mov(const atl::shared_ptr<X86::Operand> &dst,
           const atl::shared_ptr<X86::Operand> &src,
           const atl::string &comment = "");
  void pop(const atl::shared_ptr<X86::Operand> &reg,
           const atl::string &comment = "");
  void push(const atl::shared_ptr<X86::Operand> &op,
            const atl::string &comment = "");
  void ret(const atl::string &comment = "");
  void sub(const atl::shared_ptr<X86::Operand> &reg, const int value,
           const atl::string &comment = "");
  void write(const atl::string &str);

private:
  std::ofstream x86Output;
};

static atl::shared_ptr<Register> eax(new Register(32, "eax"));
static atl::shared_ptr<Register> ebx(new Register(32, "ebx"));
static atl::shared_ptr<Register> ecx(new Register(32, "ecx"));
static atl::shared_ptr<Register> edx(new Register(32, "edx"));
static atl::shared_ptr<Register> esi(new Register(32, "esi"));
static atl::shared_ptr<Register> edi(new Register(32, "edi"));
static atl::shared_ptr<Register> esp(new Register(32, "esp"));
static atl::shared_ptr<Register> ebp(new Register(32, "ebp"));

} // namespace X86

} // namespace ACC
