#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

#include "../include/Preprocessor.h"

using namespace ACC;

/* ---- Public ---- */

Preprocessor::Preprocessor(const std::string &abspath) : abspath(abspath) {
  init();
  try {
    std::shared_ptr<Scanner> scanner(new Scanner(abspath));
    scanners.push_back(scanner);
  } catch (std::runtime_error const &err) {
    throw std::runtime_error("\nPreprocessing: Couldn't resolve filepath:\n  " +
                             abspath + getStackPosition());
  }
}

SourceCode Preprocessor::getSource() {
  char c;
  while (true) {
    c = nextChar();

    if (c == '\0') {
      if (scanners.size() > 1) {
        scanners.pop_back();
        src += "#endFile\n";
        continue;
      }
      return SourceCode(src, abspath);
    } else if (c == '/' && (peekChar() == '*' || peekChar() == '/')) {
      passComment();
    } else if (c == '#') {
      while (std::isspace(peekChar()))
        nextChar();
      c = nextChar();
      if (c == 'd') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("define");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        preprocessDefine();
      } else if (c == 'e' && peekChar() == 'l') {
        c = nextChar();
        if (peekChar() == 'i') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("lif");
          if (!lexResult.first)
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          preprocessElif();
        } else if (peekChar() == 's') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("lse");
          if (!lexResult.first)
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          preprocessElse();
        }
      } else if (c == 'e' && peekChar() == 'n') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("endif");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        preprocessEndif();
      } else if (c == 'e' && peekChar() == 'r') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("error");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        std::string errorString = parseCondition();
        throw std::runtime_error("\nPre-Processing Error: " + errorString +
                                 getStackPosition());
      } else if (c == 'G') {
        std::pair<bool, std::string> lexResult =
            tryLexKeyword("GCC system_header");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        /* Do Nothing */
      } else if (c == 'i' && peekChar() == 'f') {
        c = nextChar();
        if (peekChar() == 'n') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("fndef");
          if (!lexResult.first)
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          c = nextChar();
          if (!std::isspace(c))
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          preprocessIfndef();
        } else if (peekChar() == 'd') {
          std::pair<bool, std::string> lexResult = tryLexKeyword("fdef");
          if (!lexResult.first)
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          c = nextChar();
          if (!std::isspace(c))
            throw std::runtime_error(
                "\nPre-Processing: Unexpected Preprocessing Directive: " +
                lexResult.second + ". " + getStackPosition());
          preprocessIfdef();
        } else if (std::isspace(peekChar())) {
          nextChar();
          preprocessIf();
        } else {
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive. " +
              getStackPosition());
        }
      } else if (c == 'i' && peekChar() == 'n') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("include");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        preprocessInclude();
      } else if (c == 'p' && peekChar() == 'r') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("pragma");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        preprocessPragma();
      } else if (c == 'u' && peekChar() == 'n') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("undef");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        c = nextChar();
        if (!std::isspace(c))
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        preprocessUndef();
      } else if (c == 'w' && peekChar() == 'a') {
        std::pair<bool, std::string> lexResult = tryLexKeyword("warning");
        if (!lexResult.first)
          throw std::runtime_error(
              "\nPre-Processing: Unexpected Preprocessing Directive: " +
              lexResult.second + ". " + getStackPosition());
        std::string errorString = parseCondition();
        std::cout << "  *  Pre-Processor Warning: " << errorString << std::endl;
      } else {
        throw std::runtime_error(
            "\nPre-Processing: Unexpected Preprocessing Directive: " +
            getStackPosition());
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
      /* @TODO: Replace definitions */
      src += c;
  }
}

/* ---- Private ---- */

std::string Preprocessor::currFilename() const {
  return scanners[scanners.size() - 1]->getFilename();
}

std::string Preprocessor::currFilepath() const {
  return scanners[scanners.size() - 1]->getFilepath();
}

int Preprocessor::currLine() const {
  return scanners[scanners.size() - 1]->getPosition().line;
}

char Preprocessor::nextChar() { return scanners[scanners.size() - 1]->next(); }

char Preprocessor::peekChar() { return scanners[scanners.size() - 1]->peek(); }

/* ---- Setup Environment ---- */
void Preprocessor::init() {
  varDefinitions["__x86_64__"] = "";
  varDefinitions["_LIBCPP_DEPRECATED_ABI_DISABLE_PAIR_TRIVIAL_COPY_CTOR"] = "";
  varDefinitions["__cplusplus"] = "";
  varDefinitions["__MACH__"] = "";
}

/* ---- Functionality ---- */

bool Preprocessor::evalIfCondition(const std::string &condition) {
  // std::string::size_type orPos = condition.find("||");
  // if (orPos != std::string::npos) {
  //   // We have an or statement.
  //   const std::string lhs = condition.substr(0, );
  //   const std::string rhs;
  // }

  // std::string::size_type andPos = condition.find("&&");
  // if (andPos != std::string::npos) {
  //   // We have an and statement.
  // }

  std::string::size_type defStartPos = condition.find("defined(");
  std::string::size_type defEndPos = condition.find(")");

  /* If this condition is checking for definition status. */
  if (defStartPos != std::string::npos && defEndPos != std::string::npos) {
    const int defStart = defStartPos + 8;
    const int defLength = condition.find(")") - condition.find("(") - 1;

    const std::string definition = condition.substr(defStart, defLength);
    const bool defined =
        (varDefinitions.find(definition) != varDefinitions.end());
    bool result = defined;
    if (condition.find("!defined(") == (defStartPos - 1) && (defStartPos != 0))
      result = !result;
    return result;
  }

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
    int currScannerLine = currLine();
    while (currScannerLine == currLine()) {
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
    int currScannerLine = currLine();
    std::string token_string;
    c = nextChar();
    while (currScannerLine == currLine()) {
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
        "\nPre-Processing: Unexpected Preprocessing Directive: " +
        getStackPosition());
}
void Preprocessor::preprocessElif() {
  assert(ifs.size() > 0);
  /* Skip this section */
  if (ifs.top()) {
    char c;
    while (true) {
      c = nextChar();
      checkChar(c);
      if (c == '#' && peekChar() == 'e') {
        c = nextChar();
        if (peekChar() == 'n')
          break;
      }
    }
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "\nPre-Processing: #if/#elif/#else expected #endif Directive. " +
          getStackPosition());
    ifs.pop();
  }
}
void Preprocessor::preprocessElse() {
  assert(ifs.size() > 0);
  /* Skip this section */
  if (ifs.top()) {
    char c;
    while (true) {
      c = nextChar();
      checkChar(c);
      if (c == '#' && peekChar() == 'e') {
        c = nextChar();
        if (peekChar() == 'n')
          break;
      }
    }
    std::pair<bool, std::string> endifRes = tryLexKeyword("endif");
    if (!endifRes.first)
      throw std::runtime_error(
          "\nPre-Processing: #if/#elif/#else expected #endif Directive. " +
          getStackPosition());
    ifs.pop();
  }
}
void Preprocessor::preprocessEndif() { ifs.pop(); }
void Preprocessor::preprocessIf() {
  std::string condition = parseCondition();

  if (evalIfCondition(condition)) {
    /* Notify preprocessElif(), preprocessElse() to skip the section. */
    ifs.push(true);
  } else {
    /* Scan for #elif, #else, or #endif */
    ifs.push(false);
    while (true) {
      std::string nextDirective = getNextIfDirective();
      if (nextDirective == "elif") {
        /* Check if this section should be used. */
        nextChar();
        condition = parseCondition();
        if (evalIfCondition(condition)) {
          ifs.top() = true;
          break;
        }
      } else if (nextDirective == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (nextDirective == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
    }
  }
}
void Preprocessor::preprocessIfdef() {
  std::string condition = "defined(" + parseCondition() + ")";

  if (evalIfCondition(condition)) {
    /* Notify preprocessElif(), preprocessElse() to skip the section. */
    ifs.push(true);
  } else {
    /* Scan for #elif, #else, or #endif */
    while (true) {
      std::string nextDirective = getNextIfDirective();
      if (nextDirective == "elif") {
        /* Check if this section should be used. */
        condition = parseCondition();
        if (evalIfCondition(condition)) {
          ifs.top() = true;
          break;
        }
      } else if (nextDirective == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (nextDirective == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
    }
  }
}
void Preprocessor::preprocessIfndef() {
  std::string condition = "!defined(" + parseCondition() + ")";

  if (evalIfCondition(condition)) {
    /* Notify preprocessElif(), preprocessElse() to skip the section. */
    ifs.push(true);
  } else {
    /* Scan for #elif, #else, or #endif */
    while (true) {
      std::string nextDirective = getNextIfDirective();
      if (nextDirective == "elif") {
        /* Check if this section should be used. */
        condition = parseCondition();
        if (evalIfCondition(condition)) {
          ifs.top() = true;
          break;
        }
      } else if (nextDirective == "else") {
        /* Allow the Lexer to use this section */
        break;
      } else if (nextDirective == "endif") {
        /* Allow the Lexer to continue */
        break;
      }
    }
  }
}
void Preprocessor::preprocessInclude() {
  bool localFile = false;
  std::string filename;

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

  /* Resolve the absolute filepath. */
  std::string abspath;
  if (localFile) {
    abspath = currFilepath() + filename;
  } else {
    const std::string cIncludePath = "/usr/include/";
    const std::string cppIncludePath =
        "/Applications/Xcode.app/Contents/Developer/Toolchains/"
        "XcodeDefault.xctoolchain/usr/include/c++/v1/";

    if (isCppHeader(filename))
      abspath = cppIncludePath + filename;
    else
      abspath = cIncludePath + filename;
  }

  /* Include this file if we haven't seen it. */
  if (files.find(abspath) == files.end()) {
    src += "#startFile=" + abspath;
    try {
      std::shared_ptr<Scanner> includeScanner(new Scanner(abspath));
      scanners.push_back(includeScanner);
    } catch (std::runtime_error const &err) {
      std::string includeStr = filename;
      if (localFile)
        includeStr = "\"" + includeStr + "\"";
      else
        includeStr = "<" + includeStr + ">";
      throw std::runtime_error(
          "\nPreprocessing: Couldn't resolve include directive:\n #include " +
          includeStr + getStackPosition());
    }
  }
}
void Preprocessor::preprocessPragma() {
  char c = nextChar();
  if (c == 'G') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("GCC system_header");
    if (!lexResult.first)
      throw std::runtime_error(
          "\nPre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + getStackPosition());
  } else if (c == 'o') {
    std::pair<bool, std::string> lexResult = tryLexKeyword("once");
    if (!lexResult.first)
      throw std::runtime_error(
          "\nPre-Processing: Unexpected Preprocessing Directive: " +
          lexResult.second + ". " + getStackPosition());
    const std::string currAbsPath = currFilepath() + currFilename();

    /* Mark that we are visiting this file. */
    if (files.find(currAbsPath) == files.end())
      files.insert(currAbsPath);
  } else {
    throw std::runtime_error("\nPre-Processing: Unknown #pragma Directive: " +
                             getStackPosition());
  }
}
void Preprocessor::preprocessUndef() {
  char c = nextChar();

  /* Lex a valid identifier. */
  std::string definition(1, c);
  while (isalpha(peekChar()) || isdigit(peekChar()) || peekChar() == '_') {
    c = nextChar();
    definition += c;
  }

  const std::map<std::string, std::string> definitonsCopy = varDefinitions;
  varDefinitions.clear();
  for (const auto &definitonCopy : definitonsCopy) {
    if (definitonCopy.first != definition)
      varDefinitions[definitonCopy.first] = definitonCopy.second;
  }
}

void Preprocessor::addDefinition(const std::string &definition,
                                 const std::string &value) {
  varDefinitions[definition] = value;
}

void Preprocessor::checkChar(char c) const {
  if (c == '\0')
    throw std::runtime_error(
        "\nPre-Processing: Expected #endif Directive but found EOF.");
}

std::string Preprocessor::getNextIfDirective() {
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
            return "elif";
        } else if (peekChar() == 's') {
          if (tryLexKeyword("lse").first)
            return "else";
        }
      } else if (peekChar() == 'n') {
        if (tryLexKeyword("endif").first)
          return "endif";
      }
    }
  }
}

std::string Preprocessor::getStackPosition() const {
  std::string output = "\n";

  const int numFiles = scanners.size() - 1;
  for (int currFile = numFiles; currFile >= 0; --currFile)
    output += " -> " + scanners[currFile]->getPosition().toString() + "\n";

  return output;
}

bool Preprocessor::isCppHeader(const std::string &filename) const {
  return (filename.find(".h") == std::string::npos);
}

std::string Preprocessor::parseCondition() {
  std::string condition;
  char c = nextChar();
  int line = currLine();
  while (line == currLine()) {
    if (c == '\\' && peekChar() == '\n') {
      nextChar();
      c = nextChar();
      line++;
    }
    condition += c;
    c = nextChar();
  }
  return condition;
}

void Preprocessor::passComment() {
  // Consume the '/' or '*' character.
  char c = nextChar();
  int line = currLine();
  if (c == '/') {
    while (currLine() == line)
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
                           getStackPosition());
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