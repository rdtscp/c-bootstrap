class Foo {
  int *val;

public:
  Foo(int *p) : val(p) {}
  ~Foo() { *this->val = 0; }
};

int main(int argc, char **argv) {
  int x = 5;

  { Foo f_x(&x); }

  return x;
}
