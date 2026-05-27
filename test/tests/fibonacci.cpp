void printf(char *, char *);
void printf(char *, int);

void print_s(char *s) { printf("%s", s); }

void print_i(int i) { printf("%d", i); }

int stoi(char *str) {
  int output;
  int idx;
  output = 0;
  idx = 0;
  while (str[idx] != '\0') {
    output = output * 10 + static_cast<int>(str[idx]) - static_cast<int>('0');
    idx = idx + 1;
  }
  return output;
}

int main(int argc, char **argv) {
  int n;
  int first;
  int second;
  int next;
  int c;

  n = stoi(argv[1]);

  first = 0;
  second = 1;

  print_s("First ");
  print_i(n);
  print_s(" terms of Fibonacci series are : ");

  c = 0;
  while (c < n) {
    if (c <= 1)
      next = c;
    else {
      next = first + second;
      first = second;
      second = next;
    }
    print_i(next);
    print_s(" ");
    c = c + 1;
  }

  return 0;
}
