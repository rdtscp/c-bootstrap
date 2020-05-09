void printf(const char *, const char *);
void printf(const char *, const char);


int main(int argc, const char **argv) {
  char str[13] = "Hello World\n";

  printf("str has value: %d\n", str);

  const int idx = 10;

  printf("str[10] has value: %c\n", str[idx]);
  return 0;
}
