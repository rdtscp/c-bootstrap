#include "ast/StructType.h"

using namespace ACC;

StructType::StructType(const atl::shared_ptr<Identifier> &p_identifier)
    : identifier(p_identifier) {}

int StructType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  atl::vector<atl::shared_ptr<VarDecl>> typeVarDecls = typeDefinition->varDecls;
  for (int idx = 0; idx < typeVarDecls.size(); ++idx) {
    const atl::shared_ptr<VarDecl> &structField = typeVarDecls[idx];
    aggregateBytes += structField->type->getBytes();
  }

  return aggregateBytes;
}

bool StructType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StructType *>(&rhs);
  return false;
}

bool StructType::operator!=(Type &rhs) const { return !(*this == rhs); }

bool StructType::operator==(const StructType &rhs) const {
  return *identifier == *rhs.identifier;
}

bool StructType::operator!=(const StructType &rhs) const {
  return !(*this == rhs);
}