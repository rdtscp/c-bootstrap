void printf(const char *, const char *);
void printf(const char *, const unsigned int);

typedef unsigned int uint;

uint char_buf_len(const char *buf) {
  uint length = 0u;
  while (*buf != '\0') {
    ++length;
    ++buf;
  }
  return length;
}

class string_view {
public:
  uint m_size;
  char *m_value;

  string_view(char *value) : m_size(char_buf_len(value)), m_value(value) {}
  ~string_view() {}
};

int main(int argc, char **argv) {
  string_view str("Hello string_view!");

  printf("expect: 'Hello string view!'\nactual: '%s'\n", str.m_value);
  printf("expect: 18\nactual: %d\n", str.m_size);

  return str.m_size - 18u;
}
