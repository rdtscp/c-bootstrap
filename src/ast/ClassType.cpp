#include "ast/ClassType.h"
#include "ast/ReferenceType.h"
#include "ast/VarDecl.h"

using namespace ACC;

ClassType::ClassType(const atl::shared_ptr<Identifier> &p_identifier)
    : identifier(p_identifier) {}

bool ClassType::canCastTo(Type &rhs) const {
  // We can cast a class to a reference of itself.
  if (rhs.astClass() == "ReferenceType") {
    const ReferenceType *rhsRefType = static_cast<ReferenceType *>(&rhs);
    if (rhsRefType->referencedType->astClass() == "ClassType") {
      const atl::shared_ptr<ClassType> rhsRefClassType =
          atl::static_pointer_cast<ClassType>(rhsRefType->referencedType);
      if (rhsRefClassType->typeDefinition.lock() == typeDefinition.lock()) {
        return true;
      }
    }
  }
  return false;
}

bool ClassType::equivalentTo(Type &rhs) const {
  Type &rhsType = ReferenceType::collapseReferenceTypes(&rhs);

  if (rhsType.astClass() != "ClassType") {
    return false;
  }

  const ClassType &ct = *static_cast<ClassType *>(&rhsType);
  return typeDefinition.lock() == ct.typeDefinition.lock();
}

unsigned int ClassType::getBytes() const {
  unsigned int aggregateBytes = 0;

  const unsigned int classDeclsSize = typeDefinition.lock()->classDecls.size();
  for (unsigned int i = 0; i < classDeclsSize; ++i) {
    if (typeDefinition.lock()->classDecls[i]->astClass() == "VarDecl" ||
        typeDefinition.lock()->classDecls[i]->astClass() == "VarDef") {
      aggregateBytes += atl::static_pointer_cast<VarDecl>(
                            typeDefinition.lock()->classDecls[i])
                            ->getBytes();
    }
  }

  return aggregateBytes;
}

atl::string ClassType::mangle() const {
  atl::string output = typeDefinition.lock()->classIdentifier->mangle();

  if (typeModifiers.find(Type::Modifiers::CONST))
    output += "_const";

  return output;
}

bool ClassType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassType *>(&rhs);
  return false;
}

bool ClassType::operator!=(Type &rhs) const { return !(*this == rhs); }

bool ClassType::operator==(const ClassType &rhs) const {
  return *identifier == *rhs.identifier;
}

bool ClassType::operator!=(const ClassType &rhs) const {
  return !(*this == rhs);
}
