#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

#include "../include/Preprocessor.h"

using namespace ACC;

/* ---- Public ---- */

Preprocessor::Preprocessor(const std::string &abspath) : abspath(abspath) {
  scanners.push(std::shared_ptr<Scanner>(new Scanner(abspath)));
}

SourceCode Preprocessor::getSource() {
  char c;
  while (true) {
    c = nextChar();

    if (c == '\0') {
      if (scanners.size() > 1) {
        scanners.pop();
        src += "#endFile\n";
        continue;
      }
      return SourceCode(src, abspath);
    } else if (c == '/' && (peekChar() == '*' || peekChar() == '/')) {
      passComment();
    } else if (c == '#') {
      c = nextChar();
      if (c == 'd') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("define");
        if (!lexResult.first)
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        preprocessDefine();
      } else if (c == 'e' && peekChar() == 'l') {
        c = nextChar();
        if (peekChar() == 'i') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("lif");
          if (!lexResult.first)
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          preprocessElif();
        } else if (peekChar() == 's') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("lse");
          if (!lexResult.first)
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          preprocessElse();
        }
      } else if (c == 'e' && peekChar() == 'n') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("endif");
        if (!lexResult.first)
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive at Line " +
              std::to_string(scanners.top()->getPosition().line) + ", Column " +
              std::to_string(scanners.top()->getPosition().column));
        c = nextChar();
      } else if (c == 'G') {
        std::pair<bool, std::string> lexResult =
            tryLexKeyword("GCC system_header");
        if (!lexResult.first)
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        // Do Nothing
      } else if (c == 'i' && peekChar() == 'f') {
        c = nextChar();
        if (peekChar() == 'n') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("fndef");
          if (!lexResult.first)
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          c = nextChar();
          if (!std::isspace(c))
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          preprocessIfndef();
        } else if (peekChar() == 'd') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("fdef");
          if (!lexResult.first)
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          c = nextChar();
          if (!std::isspace(c))
            throw std::runtime_error(
                "Pre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " +
                scanners.top()->getPosition().toString());
          preprocessIfdef();
        } else if (std::isspace(peekChar())) {
          preprocessIf();
        } else {
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive. " +
              scanners.top()->getPosition().toString());
        }
      } else if (c == 'i' && peekChar() == 'n') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("include");
        if (!lexResult.first)
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        preprocessInclude();
      } else if (c == 'p' && peekChar() == 'r') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("pragma");
        if (!lexResult.first)
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "Pre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " +
              scanners.top()->getPosition().toString());
        preprocessPragma();
      } else {
        throw std::runtime_error(
            "Pre-Processing: Unexpected Preprocessing Directive: " +
            scanners.top()->getPosition().toString());
      }
      src += "\n";
    } else if (isalpha(c) || c == '_') {
      src += c;
      std::string identifier;
      /* Lex a valid identifier. */
      while (isalpha(peekChar()) || peekChar() == '_') {
        c = nextChar();
        identifier += c;
      }

      /* Check if this Identifier is in our Definitions */
      if (false)
        continue;
      else
        src += identifier;
    } else
      /* Replace definitions */
      src += c;
  }
}

/* ---- Private ---- */
char Preprocessor::nextChar() { return scanners.top()->next(); }

char Preprocessor::peekChar() { return scanners.top()->peek(); }

bool Preprocessor::evalIfCondition(const std::string &condition) {
  if (varDefinitions.find(condition) == varDefinitions.end())
    return false;

  if (varDefinitions[condition] == "0")
    return false;

  return true;
}

void Preprocessor::preprocessDefine() {
  char c = nextChar();

  /* Lex a valid identifier. */
  std::string identifier(1, c);
  /* Lex a valid identifier. */
  while (isalpha(peekChar()) || isdigit(peekChar()) || peekChar() == '_') {
    c = nextChar();
    identifier += c;
  }

  char peek = peekChar();
  if (peek == '(') {
    nextChar();
    std::vector<std::string> params;
    std::string currParam;
    /* Lex params of this function-like definition. */
    while (peekChar() != ')') {
      c = nextChar();
      currParam += c;
      if (peekChar() == ',') {
        params.push_back(currParam);
        currParam = "";
      }
    }
    c = nextChar();

    /* Lex the function body. */
    std::string token_string;
    int currScannerLine = scanners.top()->getPosition().line;
    while (currScannerLine == scanners.top()->getPosition().line) {
      if (c == '\\' && peekChar() == '\n') {
        nextChar();
        c = nextChar();
        currScannerLine++;
      }
      token_string += c;
      c = nextChar();
    }
    PPFuncSig funSignature = PPFuncSig(identifier, params);
    funcDefinitions[funSignature] = token_string;
  } else if (peek == '\n') {
    nextChar();
    addDefinition(identifier, "");
  } else if (std::isspace(peek)) {
    nextChar();
    int currScannerLine = scanners.top()->getPosition().line;
    std::string token_string;
    c = nextChar();
    while (currScannerLine == scanners.top()->getPosition().line) {
      if (c == '\\' && (peekChar() == '\n' || peekChar() == '\r')) {
        nextChar();
        c = nextChar();
        currScannerLine++;
      }
      token_string += c;
      c = nextChar();
    }
    addDefinition(identifier, token_string);
  } else if (std::isdigit(peek)) {
    c = nextChar();
  } else
    throw std::runtime_error(
        "Pre-Processing: Unexpected Preprocessing Directive: " +
        scanners.top()->getPosition().toString());
}
void Preprocessor::preprocessInclude() {

  bool localFile = false;
  std::string filename;
  const std::string includePath = "/usr/include/c++/4.2.1/";

  /* Lex the include filepath. */
  char c = nextChar();
  if (c == '<') {
    c = nextChar();
    while (c != '>') {
      filename += c;
      c = nextChar();
      checkChar(c);
    }
    localFile = false;
  }
  if (c == '"') {
    c = nextChar();
    while (c != '"') {
      filename += c;
      c = nextChar();
      checkChar(c);
    }
    localFile = true;
  }

  std::string filepath;
  if (localFile) {
    filepath = scanners.top()->getFilepath() + filename;
  } else {
    filepath = includePath + filename;
  }
  if (files.find(filepath) == files.end()) {
    src += "#startFile=" + filepath;
    scanners.push(std::shared_ptr<Scanner>(new Scanner(filepath)));

    if (peekChar() == '\0') {
      scanners.pop();
      throw std::runtime_error(
          "Preprocessing: Invalid #include, empty file:\n  " + filepath +
          "\nin " + scanners.top()->getPosition().toString());
    }
  }
}
void Preprocessor::preprocessPragma() {

  char c = nextChar();
  std::pair<bool, std::string> lexResult = tryLexKeyword("once");
  if (!lexResult.first)
    throw std::runtime_error(
        "Pre-Processing: Unexpected Preprocessing Directive: " +
        lexResult.second + ". " + scanners.top()->getPosition().toString());
  const std::string currAbsPath =
      scanners.top()->getFilepath() + scanners.top()->getFilename();

  /* Skip this File if its already been Preprocessed. */
  if (files.find(currAbsPath) != files.end()) {
    const std::string abspath =
        scanners.top()->getFilepath() + scanners.top()->getFilename();
    while ((scanners.top()->getFilepath() + scanners.top()->getFilename()) ==
           abspath) {
      nextChar();
    }
  } else
    files.insert(currAbsPath);
}
void Preprocessor::preprocessIf() {
  /* Lex the function body. */
  nextChar();
  char c = nextChar();
  std::string condition;
  int currScannerLine = scanners.top()->getPosition().line;
  while (currScannerLine == scanners.top()->getPosition().line) {
    if (c == '\\' && peekChar() == '\n') {
      nextChar();
      c = nextChar();
      currScannerLine++;
    }
    condition += c;
    c = nextChar();
  }

  bool conditionResult = evalIfCondition(condition);
  if (conditionResult) {
    /* Do the IF body */
    ifs.push_back(std::pair<std::string, bool>(condition, true));
    /* Lexer will call preprocessor when it finds an #elif, #else, or #endif.
     */
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
        curr = nextChar();
        curr = nextChar();
        int currLine = scanners.top()->getPosition().line;
        while (currLine == scanners.top()->getPosition().line) {
          condition += curr;
          curr = nextChar();
        }
        if (evalIfCondition(condition)) {
          foundElif = true;
        }
      } else if (directive == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (directive == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
      // curr = nextChar();
    }
    if (foundElif) {
      ifs.push_back(std::pair<std::string, bool>(condition, false));
    }
  }
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
      curr = nextChar();
      checkChar(curr);
      if (curr == '#' && peekChar() == 'e') {
        nextChar();
        if (nextChar() == 'n' && nextChar() == 'd' && nextChar() == 'i' &&
            nextChar() == 'f')
          break;
      }
    }
    if (!std::isspace(nextChar()))
      throw std::runtime_error(
          "Pre-Processing: #if with #elif expected #endif Directive. " +
          scanners.top()->getPosition().toString());
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
      curr = nextChar();
      checkChar(curr);
      if (curr == '#' && peekChar() == 'e')
        break;
    }
    nextChar();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #if #else expected #endif Directive. " +
          scanners.top()->getPosition().toString());
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
      curr = nextChar();
      checkChar(curr);
      if (curr == '#' && peekChar() == 'e')
        break;
    }
    nextChar();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #if #else expected #endif Directive. " +
          scanners.top()->getPosition().toString());
    ifs.pop_back();
  }
}
void Preprocessor::preprocessIfdef() {

  char c = nextChar();
  std::string definition;
  while (isalpha(c) || c == '_') {
    definition += c;
    c = nextChar();
  }

  Position ifdefPos = scanners.top()->getPosition();
  bool definitionExists =
      varDefinitions.find(definition) != varDefinitions.end();
  if (!definitionExists) {
    char curr;
    while (true) {
      curr = nextChar();
      checkChar(curr);
      if (curr == '#' && peekChar() == 'e')
        break;
    }
    nextChar();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #ifdef expected #endif Directive. " +
          ifdefPos.toString());
  }
}
void Preprocessor::preprocessIfndef() {

  char c = nextChar();
  std::string definition;
  while (isalpha(c) || c == '_') {
    definition += c;
    c = nextChar();
  }

  bool definitionExists =
      varDefinitions.find(definition) != varDefinitions.end();
  if (definitionExists) {
    char curr;
    while (true) {
      curr = nextChar();
      checkChar(curr);
      if (curr == '#' && peekChar() == 'e')
        break;
    }
    nextChar();
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "Pre-Processing: #ifndef expected #endif Directive. " +
          scanners.top()->getPosition().toString());
  }
}
void Preprocessor::preprocessUndef() {

  char c = nextChar();
  std::string definition;
  while (isalpha(c) || c == '_') {
    definition += c;
    c = nextChar();
  }

  const std::map<std::string, std::string> definitonsCopy = varDefinitions;
  varDefinitions.clear();
  for (const auto &definitonCopy : definitonsCopy) {
    if (definitonCopy.first != definition)
      addDefinition(definitonCopy.first, definitonCopy.second);
  }
}

void Preprocessor::addDefinition(const std::string &definition,
                                 const std::string &value) {
  if (ifs.size() == 0 || ifs[ifs.size() - 1].second)
    varDefinitions[definition] = value;
}

void Preprocessor::checkChar(char c) {
  if (c == '\0')
    throw std::runtime_error(
        "Pre-Processing: Expected #endif Directive but found EOF.");
}

std::pair<bool, std::string> Preprocessor::getNextIfDirective() {

  char curr;
  while (true) {
    curr = nextChar();
    checkChar(curr);
    if (curr == '#' && peekChar() == 'e') {
      curr = nextChar();
      checkChar(curr);
      if (peekChar() == 'l') {
        checkChar(curr);
        curr = nextChar();
        if (peekChar() == 'i') {
          if (tryLexKeyword("lif").first)
            return std::pair<bool, std::string>(true, "elif");
        } else if (peekChar() == 's') {
          if (tryLexKeyword("lse").first)
            return std::pair<bool, std::string>(true, "else");
        }
      } else if (peekChar() == 'n') {
        if (tryLexKeyword("endif").first)
          return std::pair<bool, std::string>(true, "endif");
      }
    }
  }
}

void Preprocessor::passComment() {
  // Consume the '/' or '*' character.
  char c = nextChar();
  int currLine = scanners.top()->getPosition().line;
  if (c == '/') {
    while (scanners.top()->getPosition().line == currLine)
      nextChar();
    return;
  } else if (c == '*') {
    c = nextChar();
    while (true) {
      c = nextChar();
      if (c == '*' && peekChar() == '/') {
        nextChar(); // Consume the closing DIV.
        c = nextChar();
        char temp = peekChar();
        return;
      }
      if (c == '\0')
        break;
    }
  }
  throw std::runtime_error("Preprocessor: Malformed Comment: " +
                           scanners.top()->getPosition().toString());
}

std::pair<bool, std::string>
Preprocessor::tryLexKeyword(const std::string &keyword) {

  bool keywordMatch = false;
  std::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); i++) {
    char peek = peekChar();
    if (peekChar() != keyword[i])
      return std::pair<bool, std::string>(false, literal);

    literal += nextChar();
  }

  char peek = peekChar();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return std::pair<bool, std::string>(keywordMatch, literal);
}