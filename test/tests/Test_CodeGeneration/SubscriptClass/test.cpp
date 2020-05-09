void printf(const char *, int);

class Incrementer {
public:

  int operator[](const int x) {
    return x + 1;
  }

};

int main(int argc, const char **argv) {
  Incrementer inc;

  printf("inc[5] has value: %d\n", inc[5]);
  return 0;
}
