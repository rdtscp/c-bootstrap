#include "ast/Deletion.h"

using namespace ACC;

Deletion::Deletion(const DelType p_deletionType,
                   const atl::shared_ptr<VarExpr> &p_deletionVar)
    : deletionType(p_deletionType), deletionVar(p_deletionVar) {}

bool Deletion::operator==(const Deletion &rhs) const {
  if (deletionType != rhs.deletionType)
    return false;

  if (*deletionVar != *rhs.deletionVar)
    return false;

  return true;
};

bool Deletion::operator!=(const Deletion &rhs) const {
  return !(*this == rhs);
};

bool Deletion::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Deletion *>(&rhs);
  return false;
}

bool Deletion::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
