class className {

private:
  int privateInt;
  int constInt;

public:
  className(int a, int b) : privateInt(a), constInt(b) {}
};

int main(int argc, char *argv) {
  className foo;
  return 1;
}