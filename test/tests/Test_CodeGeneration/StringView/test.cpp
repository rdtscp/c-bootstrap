
void printf(const char *, const char *);

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
private:
  const uint m_size;
  const char *m_value;

public:
  string_view(const char *value)
      : m_size(char_buf_len(value)), m_value(value) {}

  ~string_view() {}

  const char *c_str() { return this->m_value; }

  const char *c_str() const { return this->m_value; }

  uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  const char *raw = "Test";

  string_view str(raw);

  printf("expect: Test\nactual: %s\n", str.c_str());

  return str.size() - 4u;
}
