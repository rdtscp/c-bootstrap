#include "ast/ClassType.h"
#include "ast/ReferenceType.h"
#include "ast/VarDecl.h"

using namespace ACC;

ClassType::ClassType(const atl::shared_ptr<Identifier> &p_identifier)
    : identifier(p_identifier) {}

int ClassType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  // @TODO: Calculate SUM of VarDecls.
  const unsigned int classDeclsSize = typeDefinition->classDecls.size();
  for (unsigned int i = 0; i < classDeclsSize; ++i) {
    if (typeDefinition->classDecls[i]->astClass() == "VarDecl") {
      aggregateBytes +=
          atl::static_pointer_cast<VarDecl>(typeDefinition->classDecls[i])
              ->getBytes();
    }
  }

  return aggregateBytes;
}

atl::string ClassType::getSignature() const { return identifier->toString(); }

bool ClassType::operator==(Type &rhs) const {
  if (rhs.astClass() == "ReferenceType") {
    const atl::shared_ptr<ReferenceType> rhsRefType(
        static_cast<ReferenceType *>(&rhs));
    return *this == *rhsRefType->referencedType;
  } else {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<ClassType *>(&rhs);
    return false;
  }
}

bool ClassType::operator!=(Type &rhs) const { return !(*this == rhs); }

bool ClassType::operator==(const ClassType &rhs) const {
  return *identifier == *rhs.identifier;
}

bool ClassType::operator!=(const ClassType &rhs) const {
  return !(*this == rhs);
}
