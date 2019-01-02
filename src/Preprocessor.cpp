#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &newScanner) : scanner(newScanner) {}

void Preprocessor::preprocessDefinition(const std::string &definition,
                                        const std::string &value) {
  definitions[definition] = value;
}

void Preprocessor::preprocessElse() {
  std::pair<std::string, bool> latestIf = ifs[ifs.size() - 1];
  if (latestIf.second) {
    while (true) {
      scanner.next();
      if (scanner.peek() == '#')
        break;
      if (scanner.peek() == '\0')
        throw std::runtime_error("Pre-Processing: Expected #else or #endif "
                                 "Directive but found EOF.");
    }
  }
}

void Preprocessor::preprocessEndif() { ifs.pop_back(); }

void Preprocessor::preprocessIf(const std::string &condition) {
  bool conditionResult = (definitions.find(condition) == definitions.end() ||
                          definitions[condition] != "0");
  ifs.push_back(std::pair<std::string, bool>(condition, conditionResult));
  if (conditionResult) {
    while (true) {
      scanner.next();
      if (scanner.peek() == '#')
        break;
      if (scanner.peek() == '\0')
        throw std::runtime_error("Pre-Processing: Expected #else or #endif "
                                 "Directive but found EOF.");
    }
  }
}

void Preprocessor::preprocessIfDef(const std::string &definition) {
  bool definitionExists = (definitions.find(definition) != definitions.end());
  ifs.push_back(std::pair<std::string, bool>(definition, !definitionExists));
  if (!definitionExists) {
    while (true) {
      scanner.next();
      if (scanner.peek() == '#')
        break;
      if (scanner.peek() == '\0')
        throw std::runtime_error(
            "Pre-Processing: Expected #endif Directive but found EOF.");
    }
  }
}

void Preprocessor::preprocessIfNDef(const std::string &definition) {
  bool definitionExists = (definitions.find(definition) != definitions.end());
  ifs.push_back(std::pair<std::string, bool>(definition, !definitionExists));
  if (definitionExists) {
    while (true) {
      scanner.next();
      if (scanner.peek() == '#')
        break;
      if (scanner.peek() == '\0')
        throw std::runtime_error(
            "Pre-Processing: Expected #endif Directive but found EOF.");
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
  const std::map<std::string, std::string> definitonsCopy = definitions;
  definitions.clear();
  for (const auto &definitonCopy : definitonsCopy) {
    if (definitonCopy.first != definition)
      preprocessDefinition(definitonCopy.first, definitonCopy.second);
  }
}