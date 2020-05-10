void printf(const char *, const char *);
void printf(const char *, const int);
void printf(const char *);

int main(int argc, const char **argv) {
  int * y = new int[1];

  *y = 1234;
  printf("expected '1234'.\n");
  printf("actual:  '%d'\n", *y);

  *y = 5678;
  printf("expected '5678'.\n");
  printf("actual:  '%d'\n", *y);

  char * x = new char[4];
  x[0] = 'a';
  x[1] = 'b';
  x[2] = 'c';
  x[3] = '\0';
  printf("expected 'abc'.\n");
  printf("actual:  '%s'\n", x);

  return 0;
}
