#include "atl/include/string.h"

void printf(const char *, const char *);

int main(int argc, char **argv) {
  const char *raw = "World";
  // atl::string str(raw);
  printf("Hello %s!", raw);
  return 23;
}
