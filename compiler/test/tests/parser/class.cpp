class className {

private:
  int privateInt;
  const int constInt;

public:
  className() {}
  className(int a, char b) {}
};

int main(int argc, char *argv) {
  className foo;
  className bar;
  className *baz = new className(1, 'b');
  int *intarr = new int[5];
  return 1;
}