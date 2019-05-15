
namespace MyNamespace {
namespace Nested {
char namespacedFunc(char c) { return 2; }
} // namespace Nested
} // namespace MyNamespace

namespace MyNamespace {
int namespacedFunc(int x) { return 1; }
} // namespace MyNamespace

int main(int argc, char **argv) {
  int foo;
  foo = MyNamespace::namespacedFunc(1);
  char c;
  c = MyNamespace::Nested::namespacedFunc('a');
  return 1;
}
