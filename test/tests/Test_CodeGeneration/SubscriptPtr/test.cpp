void printf(const char *, const char *);
void printf(const char *, const char);

class Foo {
public:
  char *m_val;

  ~Foo() {}
};

int main(int argc, const char **argv) {
  char str[13] = "Hello World\n";

  printf("str has value: %s\n", str);

  const int idx = 10;

  printf("str[10] has value: %c\n", str[idx]);

  Foo f;
  f.m_val = "Hello World\n";
  printf("f.m_val[10] has value: %c\n", f.m_val[idx]);

  return 0;
}
