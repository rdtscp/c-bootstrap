class Pair {
public:
  int first;
  char second;

  Pair(const int f, const char s) : first(f), second(s) {}

  Pair(const Pair &rhs) : first(rhs.first), second(rhs.second) {}

  ~Pair() {}
};

int main(int argc, char **argv) {
  const Pair p1(5, 'A');

  const Pair p2(p1);

  return 5 - p2.first + static_cast<int>('A') - static_cast<int>(p2.second);
}
