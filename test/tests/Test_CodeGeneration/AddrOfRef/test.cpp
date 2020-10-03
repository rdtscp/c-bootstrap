void printf(const char *, int *);
void printf(const char *, int);

int main(int argc, char **argv) {
  int value = 5;
  int &ref = value;

  printf("value:  %d\n", value);
  printf("ref:    %d\n", ref);
  bool correct = (value == ref);
  if (!correct) {
    return 1;
  }

  printf("&value:  %d\n", &value);
  printf("&ref:    %d\n", &ref);
  correct = (&value == &ref);
  if (!correct) {
    return 1;
  }

  ref = 10;
  printf("value:  %d\n", value);
  printf("ref:    %d\n", ref);
  const bool correct2 = (value == ref);
  if (!correct2) {
    return 1;
  }

  return 0;
}
