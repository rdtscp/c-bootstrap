int myGlobalInt;

int foo(int x) { return 1; }

int foo(int y) { return 1; }

int main(int argc, char *argv) {
  myGlobalInt = foo(5);
  return 1;
}
