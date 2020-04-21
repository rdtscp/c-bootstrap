namespace atl {
  typedef unsigned int uint;

  void foo(uint &param) {

  }

  void user() {
    uint x = 0u;
    foo(x);
  }
}