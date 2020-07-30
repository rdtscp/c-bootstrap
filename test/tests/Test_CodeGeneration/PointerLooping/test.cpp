void printf(const char *, const unsigned int);

int main(int argc, char **argv) {
  const char *raw = "Test";
  const char *buf = raw;
  unsigned int length = 0u;
  while (*buf != '\0') {
    ++length;
    ++buf;
  }
  printf("'Test' has length: %d!\n", length);
  return length - 4u;
}
