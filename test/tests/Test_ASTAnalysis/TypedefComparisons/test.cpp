
class Foo {
  typedef unsigned int uint;
  uint m_size;

  Foo() {
    for (uint idx = 0u; idx < m_size; ++idx) {}
  }
};

typedef unsigned int uint;

int main() {
  uint size = 1u;
  
  for (uint i = 0u; i < size; ++i) {}

  return 1;
}
