template <typename T> void swap(T &lhs, T &rhs) {
  T temp = lhs;
  lhs = rhs;
  rhs = temp;
}

int main(int argc, char **argv) {
  int a = 10;
  int b = 0;

  swap(a, b);

  return a;
}
