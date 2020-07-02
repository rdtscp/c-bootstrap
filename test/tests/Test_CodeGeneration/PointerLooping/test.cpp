void printf(const char *, const unsigned int);

unsigned int char_buf_len(const char *buf) {
  unsigned int length = 0u;
  while (*buf != '\0') {
    ++length;
    ++buf;
  }
  return length;
}

int main(int argc, char **argv) {
  const char *raw = "Test";
  const unsigned int len = char_buf_len(raw);
  printf("'Test' has length: %d!\n", len);

  return 0;
}
