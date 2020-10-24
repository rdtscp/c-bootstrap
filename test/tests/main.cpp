#include "test_string.h"

void print(const test::string_view str) { printf("%s\n", str.c_str()); }

int main(int argc, char **argv) {
  const test::string_view hello_world("Hello World!");
  print(hello_world);

  {
    const test::string_view copy = hello_world;
    print(copy);
  }

  return hello_world.size() - 12u;
}
