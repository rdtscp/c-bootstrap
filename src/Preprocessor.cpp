#include <cassert>

#include "../include/Preprocessor.h"

using namespace ACC;

Preprocessor::Preprocessor(Scanner &newScanner) : scanner(newScanner) {}

void Preprocessor::preprocessDirective() {
  char c = scanner.next();
  if (c == 'd') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("define");
    if (!lexResult.first)
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
    c = scanner.next();
    if (!std::isspace(c))
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
    define();
  }
  if (c == 'e' && scanner.peek() == 'l') {
    c = scanner.next();
    if (scanner.peek() == 'i') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("lif");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      preprocessElif();
    } else if (scanner.peek() == 's') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("lse");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      preprocessElse();
    }
  }
  if (c == 'e' && scanner.peek() == 'n') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("endif");
    if (!lexResult.first)
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive at Line " +
          std::to_string(scanner.getPosition().line) + ", Column " +
          std::to_string(scanner.getPosition().column));
    c = scanner.next();
  }
  if (c == 'i' && scanner.peek() == 'f') {
    c = scanner.next();
    if (scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("fndef");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      c = scanner.next();
      if (!std::isspace(c))
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      std::string definition;
      c = scanner.next();
      while (isalpha(c) || c == '_') {
        definition += c;
        c = scanner.next();
      }
      preprocessIfNDef(definition);
    } else if (scanner.peek() == 'd') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("fdef");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      c = scanner.next();
      if (!std::isspace(c))
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      std::string definition;
      c = scanner.next();
      while (isalpha(c) || c == '_') {
        definition += c;
        c = scanner.next();
      }
      preprocessIfDef(definition);
    } else if (std::isspace(scanner.peek())) {
      std::string condition;
      scanner.next();
      c = scanner.next();
      int currLine = scanner.getPosition().line;
      while (currLine == scanner.getPosition().line) {
        condition += c;
        c = scanner.next();
      }
      preprocessIf(condition);
    } else {
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive. " +
          scanner.getPosition().toString());
    }
  }
  if (c == 'i' && scanner.peek() == 'n') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("include");
    if (!lexResult.first)
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
    c = scanner.next();
    if (c != ' ')
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
    c = scanner.next();
    bool localFile = true;
    std::string filename;
    if (c == '<') {
      c = scanner.next();
      while (c != '>') {
        filename += c;
        c = scanner.next();
        if (c == '\0')
          throw std::runtime_error("Lexer: Unexpected EOF. " +
                                   scanner.getPosition().toString());
      }
      localFile = false;
    }
    if (c == '"') {
      c = scanner.next();
      while (c != '"') {
        filename += c;
        c = scanner.next();
        if (c == '\0')
          throw std::runtime_error("Lexer: Unexpected EOF. " +
                                   scanner.getPosition().toString());
      }
      localFile = true;
    }
    preprocessInclude(localFile, filename);
  }
  if (c == 'p' && scanner.peek() == 'r') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("pragma");
    if (!lexResult.first)
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
    c = scanner.next();
    c = scanner.next();
    if (c == 'o') {
      lexResult = tryLexKeyword("once");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      preprocessPragmaOnce(scanner.getFilepath() + scanner.getFilename());
    } else if (c == 'G') {
      lexResult = tryLexKeyword("GCC system_header");
      if (!lexResult.first)
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            lexResult.second + ". " + scanner.getPosition().toString());
      // Do Nothing
    } else {
      throw std::runtime_error(
          "Pre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + scanner.getPosition().toString());
      preprocessPragmaOnce(scanner.getFilepath() + scanner.getFilename());
    }
  }
}

void Preprocessor::define() {
  char c;
  /* Parse #define key */
  int currLine = scanner.getPosition().line;
  std::string definition;
  c = scanner.next();
  while (!std::isspace(c)) {
    definition += c;
    c = scanner.next();
  }

  /* Parse #define value */
  std::string value;
  /* Check this even has a value. */
  if (currLine == scanner.getPosition().line) {
    c = scanner.next();
    while (currLine == scanner.getPosition().line) {
      value += c;
      if (c == '\\' && scanner.peek() == '\n')
        currLine++;
      c = scanner.next();
    }
  }

  preprocessDefinition(definition, value);
}

void Preprocessor::preprocessDefinition(const std::string &definition,
                                        const std::string &value) {
  if (ifs.size() == 0 || ifs[ifs.size() - 1].second)
    definitions[definition] = value;
}

void Preprocessor::preprocessElif() {
  assert(ifs.size() > 0);
  std::pair<std::string, bool> &latestIf = ifs[ifs.size() - 1];
  bool ifExecuting = latestIf.second;
  latestIf.second = false;
  if (!ifExecuting) {
    ifs.pop_back();
    char curr;
    while (true) {
      curr = scanner.next();
      checkChar(curr);
      if (curr == '#' && scanner.peek() == 'e') {
        scanner.next();
        if (scanner.next() == 'n' && scanner.next() == 'd' &&
            scanner.next() == 'i' && scanner.next() == 'f')
          break;
      }
    }
    if (!std::isspace(scanner.next()))
      throw std::runtime_error(
          "Pre-Processing: #if with #elif expected #endif Directive. " +
          scanner.getPosition().toString());
  }
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

void Preprocessor::preprocessEndif() {
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
    bool foundElif = false;
    while (!foundElif) {
      std::pair<bool, std::string> nextIfDirective = getNextIfDirective();
      std::string directive = nextIfDirective.second;
      if (directive == "elif") {
        /* Pluck the condition */
        std::string condition;
        curr = scanner.next();
        curr = scanner.next();
        int currLine = scanner.getPosition().line;
        while (currLine == scanner.getPosition().line) {
          condition += curr;
          curr = scanner.next();
        }
        if (evalCondition(condition)) {
          foundElif = true;
        }
      } else if (directive == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (directive == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
      // curr = scanner.next();
    }
    if (foundElif) {
      ifs.push_back(std::pair<std::string, bool>(condition, false));
    }
  }
}

void Preprocessor::preprocessIfDef(const std::string &definition) {
  Position ifdefPos = scanner.getPosition();
  bool definitionExists = definitions.find(definition) != definitions.end();
  if (!definitionExists) {
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
          "Pre-Processing: #ifdef expected #endif Directive. " +
          ifdefPos.toString());
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
  if (definitions.find(condition) == definitions.end())
    return false;

  if (definitions[condition] == "0")
    return false;

  return true;
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