void printf(const char *, int);

int main(int argc, const char **argv) {
  int x = 1234;
  int *y = &x;

  x = 5678;

  printf("x has value: %d\n", *y);
  return 0;
}
