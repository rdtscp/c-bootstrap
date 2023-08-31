void printf(const char *, int);
void printf(const char *);

int main(int argc, const char **argv) {
  int x = 1234;
  int *y = &x;

  printf("expected '1234'.\n");
  printf("actual:  '%d'\n", *y);
  x = 5678;

  printf("expected '5678'.\n");
  printf("actual:  '%d'\n", *y);
  return 0;
}
