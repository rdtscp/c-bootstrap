
void printf(const char *, const int);

class bigint {
public:
  int value;

  bigint(const int v) : value(v) {}

  ~bigint() {}
};

void use(bigint p) {
  printf("p[%d]\n", p.value);

  p.value = 10;
  printf("p[%d]\n", p.value);
}

int main(int argc, char **argv) {
  bigint foo(3);

  printf("foo[%d]\n", foo.value);

  use(foo);

  printf("foo[%d]\n", foo.value);

  return foo.value - 3;
}
