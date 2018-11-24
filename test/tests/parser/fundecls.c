int myGlobalInt;

int foo() {
  int x;
  x = 2 * (3 + 4);
  return x;
}

int bar() {
  int x;
  x = 2 * 3 + 4;
  return x + foo();
}

int main(int argc, char *argv) {
  myGlobalInt = bar();
  return 1;
}