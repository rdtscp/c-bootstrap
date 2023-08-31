void printf(const char *, int);

int add(int a, int b) {
  return a + b;
}

int two() {
  return add(1, 1);
}

int three() {
  return add(1, two());
}

int five() {
  return add(two(), three());
}

int x2(int x) {
  return x * 2;
}

int ten() {
  return x2(five());
}


int main(int argc, char **argv) {
  printf("five() == %d\n", five());
  printf("ten() == %d\n", ten());
  printf("five() + ten() == %d\n", add(five(), ten()));
  printf("add(5, 10) == %d\n", add(5, 10));
  printf("add(five(), ten()) == %d\n", add(five(), ten()));

  return 0;
}
