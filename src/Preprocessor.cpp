#include <cassert>

#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &newScanner) : scanner(newScanner) {}

void Preprocessor::preprocessDefinition(const std::string &definition,
                                        const std::string &value) {
  if (ifs.size() == 0 || ifs[ifs.size() - 1].second)
    definitions[definition] = value;
}

void Preprocessor::preprocessElse() {
  assert(ifs.size() > 0);
  std::pair<std::string, bool> &latestIf = ifs[ifs.size() - 1];
  bool ifExecuted = latestIf.second;
  latestIf.second = false;
  if (ifExecuted) {
    char curr;
    while (true) {
      curr = scanner.next();
      checkChar(curr);
      if (curr == '#' && scanner.peek() == 'e')
        break;
    }
    scanner.next();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #if #else expected #endif Directive. " +
          scanner.getPosition().toString());
    ifs.pop_back();
  }
}

void Preprocessor::preprocessIf(const std::string &condition) {
  bool conditionResult = evalCondition(condition);
  if (conditionResult) {
    /* Do the IF body */
    ifs.push_back(std::pair<std::string, bool>(condition, true));
    /* Lexer will call preprocessor when it finds an #elif, #else, or #endif. */
  } else {
    /* Scan for #elif, #else, or #endif */
    char curr;
    bool withinIf = true;
    while (true) {
      std::pair<bool, std::string> nextIfDirective = getNextIfDirective();
      std::string directive = nextIfDirective.second;
      if (directive == "elif") {
        /* Pluck the condition */
      } else if (directive == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (directive == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
      // curr = scanner.next();
    }
  }
}

void Preprocessor::preprocessIfDef(const std::string &definition) {
  bool definitionExists = definitions.find(definition) != definitions.end();
  if (!definitionExists) {
    char curr;
    while (true) {
      curr = scanner.next();
      checkChar(curr);
      if (curr == '#' && scanner.peek() == 'e')
        break;
    }
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #ifdef expected #endif Directive. " +
          scanner.getPosition().toString());
  }
}

void Preprocessor::preprocessIfNDef(const std::string &definition) {
  bool definitionExists = definitions.find(definition) != definitions.end();
  if (definitionExists) {
    char curr;
    while (true) {
      curr = scanner.next();
      checkChar(curr);
      if (curr == '#' && scanner.peek() == 'e')
        break;
    }
    scanner.next();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #ifndef expected #endif Directive. " +
          scanner.getPosition().toString());
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

/* ---- Helpers ---- */

void Preprocessor::checkChar(char c) {
  if (c == '\0')
    throw std::runtime_error(
        "Pre-Processing: Expected #endif Directive but found EOF.");
}

bool Preprocessor::evalCondition(const std::string &condition) {
  return (definitions.find(condition) == definitions.end() ||
          definitions[condition] != "0");
}

std::pair<bool, std::string> Preprocessor::getNextIfDirective() {
  char curr;
  while (true) {
    curr = scanner.next();
    checkChar(curr);
    if (curr == '#' && scanner.peek() == 'e') {
      curr = scanner.next();
      checkChar(curr);
      if (scanner.peek() == 'l') {
        checkChar(curr);
        curr = scanner.next();
        if (scanner.peek() == 'i') {
          if (tryLexKeyword("lif").first)
            return std::pair<bool, std::string>(true, "elif");
        } else if (scanner.peek() == 's') {
          if (tryLexKeyword("lse").first)
            return std::pair<bool, std::string>(true, "else");
        }
      } else if (scanner.peek() == 'n') {
        if (tryLexKeyword("endif").first)
          return std::pair<bool, std::string>(true, "endif");
      }
    }
  }
}

std::pair<bool, std::string>
Preprocessor::tryLexKeyword(const std::string &keyword) {
  bool keywordMatch = false;
  std::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); i++) {
    char peek = scanner.peek();
    if (scanner.peek() != keyword[i])
      return std::pair<bool, std::string>(false, literal);

    literal += scanner.next();
  }

  char peek = scanner.peek();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return std::pair<bool, std::string>(keywordMatch, literal);
}