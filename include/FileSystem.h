#include "atl/include/string.h"
#include "atl/include/vector.h"

namespace ACC {

namespace FileSystem {

/* Returns the parentDirector of any path. */
static atl::string parentDir(const atl::string &filepath) {
  atl::string output;

  unsigned int start_idx = filepath.size() - 1u;
  if (filepath[start_idx] == '/')
    --start_idx;

  bool foundSlash = false;
  for (unsigned int idx = start_idx; idx >= 0u; --idx) {
    char currChar = filepath[idx];
    if (foundSlash) {
      output = atl::string(1u, currChar) + output;
    } else if (currChar == '/') {
      foundSlash = true;
      output = atl::string(1u, '/') + output;
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
  if (relativePath[0u] == '/')
    return relativePath;

  // Strip the file from the path.
  if (currFilepath[currFilepath.size() - 1u] != '/')
    currFilepath = parentDir(currFilepath);

  unsigned int endIdx = 0u;
  for (unsigned int idx = 0u; idx < relativePath.size(); ++idx) {
    if (relativePath[idx] == '.' && relativePath[++idx] == '.' &&
        relativePath[++idx] == '/') {
      currFilepath = parentDir(currFilepath);
    } else {
      endIdx = idx;
      break;
    }
  }
  for (unsigned int idx = endIdx; idx < relativePath.size(); ++idx)
    currFilepath += relativePath[idx];
  return currFilepath;
}

} // namespace FileSystem

} // namespace ACC
