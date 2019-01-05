#include <cassert>
#include <fstream>
#include <vector>

#include "../include/SourceCode.h"

using namespace ACC;

/* ---- Public ---- */

SourceCode::SourceCode(const std::string &src, const std::string &abspath)
    : currColumn(1), currLine(1), src(src) {
  abspaths.push(abspath);
  currIdx = 0;
}

char SourceCode::next() {
  char nextChar = src[currIdx];

  if (currIdx == src.size() || nextChar == '\0')
    return '\0';

  if (nextChar == '#') {
    preprocessorNote();
    return next();
  }

  if (nextChar == '\n' || nextChar == '\r') {
    ++currLine;
    currColumn = 1;
  } else {
    ++currColumn;
  }

  currIdx++;
  return nextChar;
}

char SourceCode::peek() {
  if (currIdx == src.size())
    return '\0';
  return src[currIdx];
}

Position SourceCode::getPosition() const {
  return Position(currLine, currColumn, getFilepath() + getFilename());
}

/* ---- Private ---- */

std::string SourceCode::getFilename() const {
  const std::string abspath = abspaths.top();
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  return directories[directories.size() - 1];
}

std::string SourceCode::getFilepath() const {
  const std::string abspath = abspaths.top();
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  std::string filepath;
  for (int i = 0; i < directories.size() - 1; i++)
    filepath += directories[i] + '/';

  return filepath;
}

void SourceCode::preprocessorNote() {
  char nextChar = src[currIdx];
  std::string note;
  while (nextChar != '\n' && nextChar != '\r') {
    note += nextChar;
    nextChar = src[++currIdx];
  }
  const int endNote = note.size() - 1;
  const int startFileLoc = note.find("startFile=");
  const int endFileLoc = note.find("endFile");
  assert(startFileLoc != endFileLoc);

  if (startFileLoc != endNote) {
    const std::string abspath = note.substr(startFileLoc + 9, endNote);
    abspaths.push(abspath);
    currLine = 1;
    currColumn = 1;
  } else if (endFileLoc != endNote)
    abspaths.pop();
}