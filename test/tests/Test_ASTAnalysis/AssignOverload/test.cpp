
void printf(const char *, const char *);

typedef unsigned int uint;

namespace test {
void swap(uint &lhs, uint &rhs) {
  uint temp = lhs;
  lhs = rhs;
  rhs = temp;
}
void swap(const char *&lhs, const char *&rhs) {
  const char *temp = lhs;
  lhs = rhs;
  rhs = temp;
}
} // namespace test

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
  uint m_size;
  const char *m_value;

public:
  string_view(const char *value)
      : m_size(char_buf_len(value)), m_value(value) {}

  string_view(const string_view &rhs)
      : m_size(rhs.m_size), m_value(rhs.m_value) {}

  string_view &operator=(string_view rhs) {
    test::swap(this->m_size, rhs.m_size);
    test::swap(this->m_value, rhs.m_value);
  }

  ~string_view() {}

  const char *c_str() { return this->m_value; }

  const char *c_str() const { return this->m_value; }

  uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  string_view str("Test");

  string_view copy = str;

  return 0;
}
