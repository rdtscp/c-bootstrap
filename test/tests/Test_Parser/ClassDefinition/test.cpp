class className {

private:
  int privateInt;
  int constInt;

public:
  className() {}
  className(int a, char b) {}

  className operator+(const int c) {
    return className(privateInt + c, 'b');
  }
};

namespace outer {
  class Inner {
  public:
    Inner(int foo) {}

    outer::Inner operator+(const int c) {
      Inner output(c);
      return output;
    }

  };
}

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
  return 1;
}
