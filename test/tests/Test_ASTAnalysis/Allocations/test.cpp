void printf(const char *, const char *);
void printf(const char *, const int);

int main(int argc, const char **argv) {
  int * y = new int[1];

  *y = 1234;
  printf("y has value: %d\n", *y);

  *y = 5678;
  printf("y has value: %d\n", *y);


  char * x = new char[4];
  x[0] = 'a';
  x[1] = 'b';
  x[2] = 'c';
  x[3] = '\0';
  printf("x has value: %s\n", x);

  return 0;
}
