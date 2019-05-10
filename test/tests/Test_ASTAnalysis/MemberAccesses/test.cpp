class Base {
public:
  int m_base;
};

class Derived {
public:
  Base m_derived;
};

int main() {
  Derived myDerived;
  myDerived.m_derived.m_base = 1;
}
