#include "Preprocessor.h"

#include "FileSystem.h"

using namespace ACC;

/* ---- PPScanner ---- */

PPScanner::PPScanner(const SourceHandler &src) : Scanner(src) {}

char PPScanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '\n' || nextChar == '\r') {
    ++line;
    column = 1;
  } else {
    ++column;
  }
  ++currChar;
  return nextChar;
}

/* ---- Preprocessor ---- */

Preprocessor::Preprocessor(const SourceHandler &src,
                           const atl::vector<atl::string> &includePaths,
                           atl::shared_ptr<Preprocessor> parentPreprocessor)
    : includePaths(includePaths), src(src),
      parentPreprocessor(parentPreprocessor), scanner(new PPScanner(src)) {}

SourceHandler Preprocessor::getSource() {

  atl::string output = formatIncludeDirective(src.getFilepath()) + "\n";
  char c;
  do {
    c = scanner->next();
    // Skip through Comments.
    if (c == '/' && (scanner->peek() == '*' || scanner->peek() == '/')) {
      passComment();
    } else if (c == '#' && scanner->peek() == 'i') {
      const SourceHandler includeFilepath = lexInclude();

      // Preprocess the included file.
      Preprocessor includePreprocessor(includeFilepath, includePaths, this);
      const SourceHandler includeSource = includePreprocessor.getSource();

      // Append the preprocessed source.
      output += includeSource.value;

      // Mark we are returning to the original file.
      output += Preprocessor::formatIncludeDirective(
          src.getFilepath(), scanner->getPosition().line);
    } else if (c == '#' && scanner->peek() == 'p') {
      const bool filePreprocessed = lexPragmaOnce();
      if (filePreprocessed)
        return SourceHandler(SourceHandler::Type::RAW, output + "\n");
    } else {
      output += c;
    }
  } while (c != '\0');
  markVisited(src.getFilepath());
  return SourceHandler(SourceHandler::Type::RAW, output);
}

// bool Preprocessor::filePreprocessed(const atl::string &filename) { if }

/* Have found `#i`, lex the rest and return a SourceHandler. */
SourceHandler Preprocessor::lexInclude() {
  lexKeyword("#include");
  scanner->next(); // Skip space.
  char c = scanner->next();
  if (c != '"')
    throw "Preprocessor: #include directives must be followed by a string "
          "filepath";

  const atl::string relativeIncludePath = lexStringLiteral();
  c = scanner->next();
  while (c != '\n') {
    if (!atl::isspace(c))
      throw "Preprocessor: #include directive must end with whitespace and a "
            "newline.";
    c = scanner->next();
  }

  const atl::string absoluteIncludePath =
      FileSystem::resolveRelativePath(src.getFilepath(), relativeIncludePath);

  return SourceHandler(SourceHandler::Type::FILEPATH, absoluteIncludePath);
}

bool Preprocessor::lexPragmaOnce() {
  lexKeyword("#pragma once");
  char c = scanner->next();
  while (c != '\n') {
    if (!atl::isspace(c))
      throw "Preprocessor: #pragma once directive must end with whitespace "
            "and "
            "a newline.";
    c = scanner->next();
  }
  return checkVisited(src.getFilepath());
}

/* Helpers */

void Preprocessor::lexKeyword(const atl::string &keyword) {
  atl::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); ++i) {
    if (scanner->peek() != keyword[i])
      throw "Preprocessor: Could not Lex Keyword";

    literal += scanner->next();
  }

  char peekChar = scanner->peek();
  if ((literal == keyword) && (!isalpha(peekChar)) && (!isdigit(peekChar)) &&
      (peekChar != '_'))
    return;

  throw "Preprocessor: Could not Lex Keyword";
}
atl::string Preprocessor::lexStringLiteral() {
  atl::string literal;

  while (true) {
    char c = scanner->next();
    if (c == '\0')
      throw "Preprocessor: Unexpected EOF in String Literal.";
    if (c == '\n')
      throw "Preprocessor: Unexpected Newline Character in String Literal. ";

    // Check if we are about to see an escaped character.
    if (c == '\\') {
      literal += c;
      c = scanner->next();
      literal += c;
    } else if (c == '"') {
      break;
    } else {
      literal += c;
    }
  }
  return literal;
}
bool Preprocessor::checkVisited(const atl::string &filepath) const {
  if (parentPreprocessor != nullptr) {
    return parentPreprocessor->checkVisited(filepath);
  } else {
    for (int idx = 0; idx < filesPreprocessed.size(); ++idx)
      if (filesPreprocessed[idx] == filepath)
        return true;

    return false;
  }
}
void Preprocessor::markVisited(const atl::string &filepath) {
  if (parentPreprocessor != nullptr)
    parentPreprocessor->markVisited(filepath);
  else
    filesPreprocessed.push_back(filepath);
}
void Preprocessor::passComment() {
  // Consume the '/' or '*' character.
  char c = scanner->next();
  int currLine = scanner->getPosition().line;
  if (c == '/') {
    while (scanner->getPosition().line == currLine)
      scanner->next();
    return;
  } else if (c == '*') {
    c = scanner->next();
    while (true) {
      c = scanner->next();
      if (c == '*' && scanner->peek() == '/') {
        scanner->next(); // Consume the closing DIV.
        c = scanner->next();
        return;
      }
      if (c == '\0')
        break;
    }
  }
  throw std::runtime_error(
      std::string(
          "Preprocessor: Lexing Comment Returned Unexpected SourceToken(s). ") +
      scanner->getPosition().toString().c_str());
}