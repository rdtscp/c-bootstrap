class Example1 {
public:
  Example1(int x) : m_val(x) {}

  int operator[](const int y) { return this->m_val + y; }

private:
  int m_val;
};

namespace Foo {
class Example2 {
public:
  int operator[](const int input) { return input + 1; }

  int m_func(Example2 foo) { return foo[0]; }
};
} // namespace Foo

int main(int argc, char **argv) {
  Example1 e1(5);
  int foo = e1[1];

  Foo::Example2 e2;
  int bar = e2[1];

  return 1;
}
