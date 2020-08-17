typedef unsigned int uint;

void printf(const char *, const char *);
void printf(const char *, uint);

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
    this->m_value[this->m_size] = '\0';
    printf("\tConstruct string: '%s'\n", this->m_value);
  }

  string(const string &rhs)
      : m_size(rhs.m_size), m_value(new char[this->m_size + 1u]) {
    // printf("\tCopying string: '%s'\n", rhs.c_str());
    uint idx = 0u;
    while (idx < this->m_size) {
      this->m_value[idx] = rhs.m_value[idx];
      ++idx;
    }
    this->m_value[this->m_size] = '\0';

    printf("\tCopied string: '%s'\n", this->m_value);
  }

  ~string() {
    printf("\tDestruct string: '%s'\n", this->m_value);
    this->m_size = 0u;
    delete[] this->m_value;
    this->m_value = nullptr;
  }

  const char *c_str() { return this->m_value; }

  const char *c_str() const { return this->m_value; }

  uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  const char *raw = "Hello World";

  string sv1(raw);
  printf("Printing sv1: '%s'\n", sv1.c_str());
  printf("Size of sv1:  %d\n", sv1.size());

  {
    string sv2(sv1);
    printf("Printing sv2: '%s'\n", sv2.c_str());
    printf("Size of sv2:  %d\n", sv2.size());
  }

  return 11u - sv1.size();
}
