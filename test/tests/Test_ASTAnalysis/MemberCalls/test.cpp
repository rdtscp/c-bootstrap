class Base {
public:
  int b_func() { return 1; }
};

class Derived {
public:
  char m_int;

  Base d_func() {
    Base b;
    const int unused = this->m_int;
    return b;
  }
};

int main() {
  Derived myDerived;
  int m_int = myDerived.d_func().b_func();
}
