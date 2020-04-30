class Foo {
public:

  Foo(const int x) {
    const int z = bar(x);
  }

  static int bar(const int y) {
    return y;
  }

};
