class Foo {
public:
  int m_val;

  Foo(const int val) : m_val(val) {}

  ~Foo() { this->m_val += 1; }
};
