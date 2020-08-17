typedef unsigned int uint;

uint char_buf_len(const char *buf) {
  uint length = 0u;
  while (*buf != '\0') {
    ++length;
    ++buf;
  }
  return length;
}

class string {
public:
  uint m_size;
  char *m_value;

  /* Constructor */
  string(const char *string_literal)
      : m_size(char_buf_len(string_literal)),
        m_value(new char[this->m_size + 1u]) {
    uint idx = 0u;
    while (idx < this->m_size) {
      this->m_value[idx] = string_literal[idx];
      ++idx;
    }
    m_value[this->m_size] = '\0';
  }
};

int main(int argc, char **argv) {
  string foo("Hello World");

  return foo.m_size;
}
