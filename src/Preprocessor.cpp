#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &newScanner) : scanner(newScanner) {}

void Preprocessor::preprocessDefinition(const std::string &definition) {
  definitions.insert(definition);
}

void Preprocessor::preprocessIfNDef(const std::string &definition) {
  if (definitions.find(definition) != definitions.end()) {
    char curr;
    while (true) {
      curr = scanner.next();
      if (curr == '#' && scanner.peek() == 'e')
        break;
      if (curr == '\0')
        throw std::runtime_error(
            "Pre-Processing: Expected #endif Directive but found EOF.");
    }
    std::string expected = "endif";
    for (int i = 0; i < expected.length(); i++) {
      curr = scanner.next();
      if (curr != expected[i]) {
        throw std::runtime_error("Pre-Processing: Expected #endif Directive. " +
                                 scanner.getPosition().toString());
      }
    }
  }
}

void Preprocessor::preprocessInclude(const bool localFile,
                                     const std::string &filename) {
  if (localFile) {
    std::shared_ptr<Scanner> includeScanner(
        new Scanner(scanner.getFilepath() + filename));
    scanner.startIncluding(includeScanner);
  } else {
    const std::string includePath = "/usr/include/c++/4.2.1/";
    std::shared_ptr<Scanner> includeScanner(
        new Scanner(includePath + filename));
    scanner.startIncluding(includeScanner);
  }
}

void Preprocessor::preprocessPragmaOnce(const std::string &filename) {
  if (files.find(filename) != files.end()) {
    const std::string abspath = scanner.getFilepath() + scanner.getFilename();
    while ((scanner.getFilepath() + scanner.getFilename()) == abspath) {
      scanner.next();
    }
  } else
    files.insert(filename);
}

void Preprocessor::preprocessUndef(const std::string &definition) {
  const std::set<std::string> definitonsCopy = definitions;
  definitions.clear();
  for (const std::string &definitonCopy : definitonsCopy) {
    if (definitonCopy != definition)
      preprocessDefinition(definitonCopy);
  }
}