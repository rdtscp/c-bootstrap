void printf(const char *, int);

class Base {
public:
  int m_size;
  char *m_str;
};

class Derived {
public:
  Base m_base;

  ~Derived() {}
};

int main(int argc, char **argv) {
  Derived myDerived;
  myDerived.m_base.m_size = 1337;
  myDerived.m_base.m_str = "Expecting 1337, got: %d\n";
  // printf(myDerived.m_base.m_str, myDerived.m_base.m_size);
  if (myDerived.m_base.m_size == 1337) {
    return 0;
  }
  return 1;
}
