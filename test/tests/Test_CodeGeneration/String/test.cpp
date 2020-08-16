
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

class string {
private:
  uint m_size;
  char *m_value;

public:
  /* Constructor */
  string(const char *string_literal)
      : m_size(char_buf_len(string_literal)), m_value(new char[m_size + 1u]) {
    this->m_size = char_buf_len(string_literal);
    this->m_value = new char[m_size + 1u];

    unsigned int idx = 0u;
    while (idx < 4u) {
      this->m_value[idx] = string_literal[idx];
      ++idx;
    }
    this->m_value[m_size] = '\0';
  }

  ~string() {
    // TODO: Delete m_value;
  }

  const char *c_str() { return this->m_value; }

  const char *c_str() const { return this->m_value; }

  uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  const char *raw = "Test";

  string str(raw);

  // char *copy = new char[5u];

  // const char *raw_ptr = raw;
  // unsigned int idx = 0u;
  // while (idx < 4u) {
  //   copy[idx] = *raw_ptr;
  //   ++raw_ptr;
  //   ++idx;
  // }

  printf("expect: Test\nactual: %s\n", str.c_str());

  return str.size() - 4u;
}
