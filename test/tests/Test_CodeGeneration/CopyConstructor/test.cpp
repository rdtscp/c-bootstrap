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

class string_view {
  const char *m_value;
  uint m_size;

public:
  string_view(const char *value) : m_value(value), m_size(char_buf_len(value)) {
    printf("\tConstruct string_view: '%s'\n", m_value);
  }

  string_view(const string_view &rhs)
      : m_value(rhs.m_value), m_size(rhs.m_size) {
    printf("\tCopy string_view: '%s'\n", this->m_value);
    printf("\trhs string_view: '%d'\n", rhs.m_size);
    printf("\tthis string_view: '%d'\n", this->m_size);
  }

  ~string_view() {
    printf("\tDestruct string_view: '%s'\n", this->m_value);
    m_size = 0u;
    m_value = nullptr;
  }

  const char *c_str() const { return this->m_value; }

  const uint size() const { return this->m_size; }
};

int main(int argc, char **argv) {
  const char *raw = "Hello World";

  string_view sv1(raw);
  printf("Printing sv1: '%s'\n", sv1.c_str());
  printf("Size of sv1:  %d\n", sv1.size());

  {
    string_view sv2(sv1);
    printf("Printing sv2: '%s'\n", sv2.c_str());
    printf("Size of sv2:  %d\n", sv2.size());
  }

  return 11u - sv1.size();
}
