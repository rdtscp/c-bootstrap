#include "atl/include/string.h"
#include "atl/include/vector.h"

namespace ACC {

namespace FileSystem {

/* Returns the parentDirector of any path. */
static atl::string parentDir(const atl::string &filepath) {
  atl::string output;

  int start_idx = filepath.size() - 1;
  if (filepath[start_idx] == '/')
    --start_idx;

  bool foundSlash = false;
  for (int idx = start_idx; idx >= 0; --idx) {
    char currChar = filepath[idx];
    if (foundSlash) {
      output = atl::string(1, currChar) + output;
    } else if (currChar == '/') {
      foundSlash = true;
      output = atl::string(1, '/') + output;
    }
  }
  return output;
}

/* Takes in the current absolute filepath, and a relative
 * path, and resolved the relative path to an absolute path.
 */
static atl::string resolveRelativePath(atl::string currFilepath,
                                       const atl::string &relativePath) {
  // Already an absolute path, return it.
  if (relativePath[0] == '/')
    return relativePath;

  // Strip the file from the path.
  if (currFilepath[currFilepath.size() - 1] != '/')
    currFilepath = parentDir(currFilepath);

  int endIdx = 0;
  for (int idx = 0; idx < relativePath.size(); ++idx) {
    if (relativePath[idx] == '.' && relativePath[++idx] == '.' &&
        relativePath[++idx] == '/') {
      currFilepath = parentDir(currFilepath);
    } else {
      endIdx = idx;
      break;
    }
  }
  for (int idx = endIdx; idx < relativePath.size(); ++idx)
    currFilepath += relativePath[idx];
  return currFilepath;
}

} // namespace FileSystem

} // namespace ACC