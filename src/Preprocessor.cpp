#include "Preprocessor.h"

#include "Error.h"
#include "FileSystem.h"

using namespace ACC;

/* ---- PPScanner ---- */

PPScanner::PPScanner(const SourceHandler &src) : Scanner(src) {
  const atl::string abspath = src.getFilepath();

  filepath = getFilepath(abspath);
  filename = getFilename(abspath);

  const char last_char = *(file.end() - 1);
  if (last_char != '\n') {
    throw ACC::Error("Preprocessor: Files must end with `\\n`: " + filepath +
                     filename +
                     ". But ended with: " + atl::string(1, last_char));
  }
}

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

Preprocessor::Preprocessor(const SourceHandler &p_src,
                           const atl::vector<atl::string> &p_includePaths,
                           atl::shared_ptr<Preprocessor> p_parentPreprocessor)
    : includePaths(p_includePaths), src(p_src),
      parentPreprocessor(p_parentPreprocessor), scanner(new PPScanner(src)) {}

SourceHandler Preprocessor::getSource() {
  atl::string output = formatIncludeDirective(src.getFilepath()) + "\n";
  char c;
  do {
    c = scanner->next();
    // Skip through Comments.
    if (c == '/' && (scanner->peek() == '*' || scanner->peek() == '/')) {
      const int linesInComment = passComment();
      const atl::string newlines(linesInComment, '\n');
      output += newlines;
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
      if (filePreprocessed) {
        return SourceHandler(SourceHandler::Type::RAW, output + "\n");
      } else {
        markVisited(src.getFilepath());
      }
    } else {
      output += c;
    }
  } while (c != '\0');
  return SourceHandler(SourceHandler::Type::RAW, output);
}

// bool Preprocessor::filePreprocessed(const atl::string &filename) { if }

/* Have found `#i`, lex the rest and return a SourceHandler. */
SourceHandler Preprocessor::lexInclude() {
  lexKeyword("#include");
  scanner->next(); // Skip space.
  char c = scanner->next();
  if (c == '<') {
    /* BIG HACK: Just accept we have to have:
     *   <initializer_list>
     *   <stdio.h>
     */
    if (scanner->peek() == 'i') {
      lexKeyword("<initializer_list>");
      return SourceHandler(SourceHandler::Type::RAW, "\n");
    } else if (scanner->peek() == 's') {
      lexKeyword("<stdio.h>");
      atl::string stdio_h_str;
      stdio_h_str += "struct FILE;\n";
      stdio_h_str += "extern \"C\" FILE *fopen(char *, char *);\n";
      stdio_h_str += "extern \"C\" void fclose(FILE *);\n";
      stdio_h_str += "extern \"C\" char *fgets(char *, int, FILE *);\n";
      return SourceHandler(SourceHandler::Type::RAW, stdio_h_str);
    } else {
      throw ACC::Error(
          "Preprocessor: #include directives must be followed by a "
          "string filepath.",
          scanner->getPosition());
    }
  } else if (c != '"') {
    throw ACC::Error("Preprocessor: #include directives must be followed by a "
                     "string filepath.",
                     scanner->getPosition());
  }

  const atl::string relativeIncludePath = lexStringLiteral();
  c = scanner->next();
  while (c != '\n') {
    if (!atl::isspace(c))
      throw ACC::Error(
          "Preprocessor: #include directive must end with whitespace and a "
          "newline.",
          scanner->getPosition());
    c = scanner->next();
  }

  const atl::string absoluteIncludePath =
      FileSystem::resolveRelativePath(src.getFilepath(), relativeIncludePath);

  // Check the file exists.
  if (fileExists(absoluteIncludePath))
    return SourceHandler(SourceHandler::Type::FILEPATH, absoluteIncludePath);

  for (unsigned int idx = 0; idx < includePaths.size(); ++idx) {
    const atl::string currIncludePath = includePaths[idx] + relativeIncludePath;
    if (fileExists(currIncludePath))
      return SourceHandler(SourceHandler::Type::FILEPATH, currIncludePath);
  }
  throw ACC::Error(
      "Preprocessor: Could not include file that does not exist: " +
          relativeIncludePath,
      scanner->getPosition());
}

bool Preprocessor::lexPragmaOnce() {
  lexKeyword("#pragma once");
  char c = scanner->next();
  while (c != '\n') {
    if (!atl::isspace(c))
      throw ACC::Error("Preprocessor: #pragma once directive must end with "
                       "whitespace and a newline.",
                       scanner->getPosition());
    c = scanner->next();
  }
  return checkVisited(src.getFilepath());
}

/* Helpers */

void Preprocessor::lexKeyword(const atl::string &keyword) {
  atl::string literal(1, keyword[0]);

  for (unsigned int i = 1; i < keyword.length(); ++i) {
    if (scanner->peek() != keyword[i])
      throw ACC::Error("Preprocessor: Could not Lex Keyword",
                       scanner->getPosition());

    literal += scanner->next();
  }

  char peekChar = scanner->peek();
  if ((literal == keyword) && (!atl::isalpha(peekChar)) &&
      (!atl::isdigit(peekChar)) && (peekChar != '_'))
    return;

  throw ACC::Error("Preprocessor: Could not Lex Keyword",
                   scanner->getPosition());
}
atl::string Preprocessor::lexStringLiteral() {
  atl::string literal;

  while (true) {
    char c = scanner->next();
    if (c == '\0')
      throw ACC::Error("Preprocessor: Unexpected EOF in String Literal.",
                       scanner->getPosition());
    if (c == '\n')
      throw ACC::Error(
          "Preprocessor: Unexpected Newline Character in String Literal. ",
          scanner->getPosition());

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
    for (unsigned int idx = 0; idx < filesPreprocessed.size(); ++idx)
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
int Preprocessor::passComment() {
  // Consume the '/' or '*' character.
  char c = scanner->next();
  int currLine = scanner->getPosition().line;
  if (c == '/') {
    while (scanner->getPosition().line == currLine)
      scanner->next();
    return 1;
  } else if (c == '*') {
    int numLines = 0;
    c = scanner->next();
    while (true) {
      c = scanner->next();
      if (c == '\n')
        ++numLines;
      if (c == '*' && scanner->peek() == '/') {
        scanner->next(); // Consume the closing DIV.
        // c = scanner->next();
        return numLines;
      }
      if (c == '\0')
        break;
    }
  }
  throw ACC::Error(
      "Preprocessor: Lexing Comment Returned Unexpected SourceToken(s). ",
      scanner->getPosition());
}