void printf(const char *, int);


int main(int argc, const char **argv) {
  int x = 1234;
  int *y = &x;

  printf("x has value: %d\n", *y);
}
