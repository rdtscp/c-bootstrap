int myGlobalInt;

int foo() { return 1; }

int foo() { return 1; }

int main(int argc, char *argv) {
  myGlobalInt = foo();
  return 1;
}
