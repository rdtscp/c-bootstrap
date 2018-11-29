#include <iostream>


#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &newScanner) : scanner(newScanner) {}

void Preprocessor::preprocessDefinition(const std::string &definition) {
  definitions.insert(definition);
}

void Preprocessor::preprocessIfNDef(const std::string &definition) {
  if (definitions.find(definition) != definitions.end()) {
    char curr, peek;
    do {
      curr = this->scanner.next();
      peek = this->scanner.peek();
    } while (curr != '#' && peek != 'e');
    std::string expected = "endif";
    for (int i = 0; i < expected.length(); i++) {
      curr = this->scanner.next();
      if (curr != expected[i]) {
        throw std::runtime_error(
            "Pre-Processing: Expected #endif Directive at Line " +
            std::to_string(this->scanner.line) + ", Column " +
            std::to_string(this->scanner.column));
      }
    }
  }
}

void Preprocessor::preprocessInclude(const bool localFile,
                                     const std::string &filename) {
  if (localFile) {
    std::cout << "Include: " << filename << std::endl;
    std::shared_ptr<Scanner> includeScanner(new Scanner(filename, scanner.getFilepath()));
    this->scanner.startIncluding(includeScanner);
  } else {
    throw std::runtime_error("Pre-Processing: Include Directive only Supports "
                             "Local Includes Temporarily");
  }
  // bar
}