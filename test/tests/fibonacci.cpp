void print_s(char *s) {}

void print_i(int i) {}

void print_c(char c) {}

char read_c() { return 'c'; }

int read_i() { return 0; }

void *mcmalloc(int size) {
  void *out;
  return out;
}

int main(int argc, char **argv) {
  int n;
  int first;
  int second;
  int next;
  int c;

  // Use argc to determine n (argc is 1 with no args, 2 with one arg, etc.)
  // This way we can test with different inputs without dereferencing argv
  if (argc == 1) {
    n = 10;  // Default: fib(10) = 34
  } else if (argc == 2) {
    n = 5;   // fib(5) = 3
  } else {
    n = 7;   // fib(7) = 8
  }

  first = 0;
  second = 1;

  c = 0;
  while (c < n) {
    if (c <= 1)
      next = c;
    else {
      next = first + second;
      first = second;
      second = next;
    }
    c = c + 1;
  }
  
  return next;
}
