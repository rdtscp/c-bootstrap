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

Preprocessor::Preprocessor(const atl::vector<atl::string> &includePaths)
    : includePaths(includePaths) {}

SourceHandler Preprocessor::getSource(const SourceHandler &src) {
  scanner = atl::shared_ptr<PPScanner>(new PPScanner(src));
  atl::string sourceFilepath = "RAW";
  if (src.type == SourceHandler::Type::FILEPATH)
    sourceFilepath = src.value;

  atl::string output = formatIncludeDirective(sourceFilepath) + "\n";
  char c;
  do {
    c = scanner->next();
    if (c == '#' && scanner->peek() == 'i') {
      const atl::string relativeIncludePath = lexInclude();
      const atl::string absoluteIncludePath =
          FileSystem::resolveRelativePath(sourceFilepath, relativeIncludePath);
      const SourceHandler includeFilepath(SourceHandler::Type::FILEPATH,
                                          absoluteIncludePath);

      // Preprocess the included file.
      Preprocessor includePreprocessor(includePaths);
      const SourceHandler includeSource =
          includePreprocessor.getSource(includeFilepath);

      // Append the preprocessed source.
      output += includeSource.value;

      // Mark we are returning to the original file.
      output += Preprocessor::formatIncludeDirective(
          sourceFilepath, scanner->getPosition().line);
    } else {
      output += c;
    }
  } while (c != '\0');
  return SourceHandler(SourceHandler::Type::RAW, output);
}

/* Have found `#i`, lex the rest. */
atl::string Preprocessor::lexInclude() {
  lexKeyword("#include");
  scanner->next(); // Skip space.
  char c = scanner->next();
  if (c != '"')
    throw "Preprocessor: #include directives must be followed by a string "
          "filepath";

  // scanner->next();
  const atl::string relativeFilepath = lexStringLiteral();
  c = scanner->next();
  while (c != '\n') {
    if (!atl::isspace(c))
      throw "Preprocessor: #include directive must end with whitespace and a "
            "newline.";
    c = scanner->next();
  }

  return relativeFilepath;
}

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
