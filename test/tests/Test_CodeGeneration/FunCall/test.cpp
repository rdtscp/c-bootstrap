void printf(const char *, int);

int five() {
  return 5;
}

int ten() {
  return 10;
}

int add(int a, int b) {
  return a + b;
}

int main(int argc, char **argv) {
  printf("five() == %d\n", five());
  printf("ten() == %d\n", ten());
  printf("five() + ten() == %d\n", add(five(), ten()));
  printf("add(5, 10) == %d\n", add(5, 10));
  printf("add(five(), ten()) == %d\n", add(five(), ten()));

  return 0;
}
