class className {

private:
  int privateInt;
  int constInt;

public:
  className() {}
  className(int a, char b) {}
};

int randInt() {
  const int foo = 10;
  return 5 + foo;
}

int main(int argc, char *argv) {
  className foo;
  className bar;
  className far(1, 15);
  int myint = 4;
  className *baz = new className(1, 'b');
  int *intarr = new int[randInt()];
  // int *intarr = new int[intarr[1] + 3];
  return 1;
}
