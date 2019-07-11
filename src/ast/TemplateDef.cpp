#include "ast/TemplateDef.h"

using namespace ACC;

TemplateDef::TemplateDef() {}

atl::shared_ptr<Identifier> TemplateDef::getIdentifier() const {
  return nullptr;
}

bool TemplateDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TemplateDef *>(&rhs);
  return false;
}

bool TemplateDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool TemplateDef::operator==(const TemplateDef &rhs) const { return true; }

bool TemplateDef::operator!=(const TemplateDef &rhs) const {
  return !(*this == rhs);
}
