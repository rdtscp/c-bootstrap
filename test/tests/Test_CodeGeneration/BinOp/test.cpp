void printf(const char *, const bool);

int main(int argc, const char **argv) {
  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;
  
  printf("one < two == %d\n", one < two);
  printf("two < three == %d\n", two < three);
  printf("three < one == %d\n", three < one);

  printf("three > two == %d\n", three > two);
  printf("two > one == %d\n", two > one);
  printf("one > three == %d\n", one > three);

  printf("two >= one == %d\n", two >= one);
  printf("two >= two == %d\n", two >= two);
  printf("two >= three == %d\n", two >= three);

  printf("two <= one == %d\n", two <= one);
  printf("two <= two == %d\n", two <= two);
  printf("two <= three == %d\n", two <= three);

  printf("three % two == %d\n", three % two);
  printf("four % two == %d\n", four % two);

  return 0;
}
