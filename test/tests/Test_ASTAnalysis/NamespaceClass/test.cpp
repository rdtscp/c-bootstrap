
namespace MyNamespace {
class MyClass {
public:
  MyClass(const int x) : m_x(x) {}

  int m_x;
};
} // namespace MyNamespace

static MyNamespace::MyClass operator+(const MyNamespace::MyClass &lhs, const int rhs) {
  MyNamespace::MyClass output(lhs.m_x + rhs);
  return output;
}

int main(int argc, char **argv) {
  MyNamespace::MyClass one(1);
  MyNamespace::MyClass two = one + 2; // + one;
  return two.m_x;
}
