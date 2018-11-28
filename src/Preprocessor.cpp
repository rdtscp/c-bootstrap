#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &scanner) : scanner(scanner) {}

void Preprocessor::preprocessDefinition(const std::string &definition) {}

void Preprocessor::preprocessIfNDef(const std::string &definition) {}

void Preprocessor::preprocessInclude(const bool localFile,
                                     const std::string &filepath) {}