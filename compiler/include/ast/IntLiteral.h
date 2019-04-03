#pragma once

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class IntLiteral : public Literal,
                   public atl::enable_shared_from_this<IntLiteral> {
public:
  IntLiteral(const atl::string &p_literal);


  atl::string getLiteral() const override {
    if (value.find('x') != atl::string::npos) {
      unsigned int intValue =
          std::stoul(std::string(value.c_str()), nullptr, 16);
      return atl::to_string(intValue);
    }
    return value;
  }

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const IntLiteral &rhs) const;
  bool operator!=(const IntLiteral &rhs) const;
  
  atl::shared_ptr<IntLiteral> getptr(){ return shared_from_this(); }

  atl::string astClass() const override { return "IntLiteral"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
