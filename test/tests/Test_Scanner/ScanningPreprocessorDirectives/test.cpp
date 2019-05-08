# 1 "path/to/a/file.h"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 399 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "compiler/test/tests/scanner/include.cpp" 2
# 1 "compiler/test/tests/scanner/header.h" 1

# 1 "compiler/test/tests/scanner/header2.h" 1
int bar() { return 0; }
# 3 "compiler/test/tests/scanner/header.h" 2

int foo() { return 1; }

# 2 "compiler/test/tests/scanner/include.cpp" 2

int main() { return foo(); }
