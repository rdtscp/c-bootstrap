
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
      : m_size(char_buf_len(string_literal)),
        m_value(new char[this->m_size + 1u]) {

    uint idx = 0u;
    while (idx < this->m_size) {
      this->m_value[idx] = string_literal[idx];
      ++idx;
    }
    this->m_value[m_size] = '\0';
  }

  ~string() {
    // this->m_size = 0u;
    // delete[] this->m_value;
    // this->m_value = nullptr;
    // TODO: Delete m_value;
  }

  const char *c_str() { return this->m_value; }

  const char *c_str() const { return this->m_value; }

  uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  const char *raw = "Test";

  string str(raw);

  printf("expect: Test\nactual: %s\n", str.c_str());

  return str.size() - 4u;
}
