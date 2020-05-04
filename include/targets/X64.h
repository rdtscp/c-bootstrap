#pragma once

#include "atl/include/ofstream.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/stack.h"
#include "atl/include/string.h"
#include "atl/include/vector.h"

namespace ACC {

namespace X64 {

class Operand {
public:
  virtual ~Operand() {}

  virtual atl::string opType() const = 0;
  virtual atl::string toString() const = 0;
};

class AddrOffset : public Operand {
public:
  const atl::shared_ptr<X64::Operand> addrOperand;
  const int offset;

  AddrOffset(const atl::shared_ptr<X64::Operand> p_addrOperand,
             const int p_offset);

  atl::string opType() const override;
  atl::string toString() const override;
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

class StringLiteral : public Operand {
public:
  const atl::string strName;
  const atl::string strVal;

  StringLiteral(const atl::string &p_strName, const atl::string &p_strVal);
  atl::string opType() const override;
  atl::string toString() const override;
};

class Registers {
private:
  atl::stack<atl::shared_ptr<Register>> m_paramRegs;

public:
  atl::shared_ptr<Register> rax;
  atl::shared_ptr<Register> rbx;
  atl::shared_ptr<Register> rcx;
  atl::shared_ptr<Register> rdx;
  atl::shared_ptr<Register> rsi;
  atl::shared_ptr<Register> rdi;
  atl::shared_ptr<Register> rsp;
  atl::shared_ptr<Register> rbp;


  atl::shared_ptr<Register> tempReg() const {
    return rax;
  }

  atl::stack<atl::shared_ptr<Register>> paramRegs() const {
    return m_paramRegs;
  }

};

class Writer final {
public:
  atl::shared_ptr<Register> rax;
  atl::shared_ptr<Register> rbx;
  atl::shared_ptr<Register> rcx;
  atl::shared_ptr<Register> rdx;
  atl::shared_ptr<Register> rsi;
  atl::shared_ptr<Register> rdi;
  atl::shared_ptr<Register> rsp;
  atl::shared_ptr<Register> rbp;

  Writer(const atl::string &filename);
  Writer(const Writer &) = delete;
  ~Writer();

  /* Instructions */
  void add(const atl::shared_ptr<X64::Operand> &op1,
           const atl::shared_ptr<X64::Operand> &op2,
           const atl::string &comment = "");
  void block(atl::string blockName, const atl::string &comment = "");
  void call(const atl::string &ident, const atl::string &comment = "");
  void cmp(const atl::shared_ptr<X64::Operand> &reg, const int value,
           const atl::string &comment = "");
  void comment(const atl::string &comment);
  void imul(const atl::shared_ptr<X64::Operand> &dst,
            const atl::shared_ptr<X64::Operand> &src,
            const atl::string &comment = "");
  void jeq(const atl::string &label, const atl::string &comment = "");
  void jmp(const atl::string &label, const atl::string &comment = "");
  void lea(const atl::shared_ptr<X64::Operand> &dst,
           const atl::shared_ptr<X64::Operand> &src);
  void mov(const atl::shared_ptr<X64::Operand> &dst,
           const atl::shared_ptr<X64::Operand> &src,
           const atl::string &comment = "");
  void pop(const atl::shared_ptr<X64::Operand> &reg,
           const atl::string &comment = "");
  void push(const atl::shared_ptr<X64::Operand> &op,
            const atl::string &comment = "");
  void ret(const atl::string &comment = "");
  void string_literal(const atl::string &strName, const atl::string &strValue);
  void sub(const atl::shared_ptr<X64::Operand> &reg, const int value,
           const atl::string &comment = "");
  void write(const atl::string &str);

  /* Helpers */
  void calleePrologue();
  void calleeEpilogue();
  void callerPrologue();
  void callerEpilogue();

  atl::shared_ptr<Register> getTempReg() const;
  atl::stack<atl::shared_ptr<Register>> paramRegs() const;

private:
  atl::ofstream x64Output;
  atl::string stringLiterals;
};

} // namespace X64

} // namespace ACC
