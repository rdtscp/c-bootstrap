void printf(const char *, int);

int myGlobalInt;

int ten() {
  int x;
  x = 2 * (3 + 15);
  return x;
}

int one() {
  int tenVar;
  tenVar = 4 + 2 * 3;
  myGlobalInt = tenVar + ten();
  return 1;
}

int main(int argc, char **argv) {
  myGlobalInt = 10;
  int x = myGlobalInt + one();
  printf("%d\n", x);
  return 0;
}
