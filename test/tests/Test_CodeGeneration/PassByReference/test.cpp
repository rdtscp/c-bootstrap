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
  uint m_size;
  const char *m_value;

public:
  string_view(const char *value) : m_value(value), m_size(char_buf_len(value)) {
    printf("\tConstruct string_view: '%s'\n", this->m_value);
  }

  string_view(const string_view &rhs)
      : m_value(rhs.m_value), m_size(rhs.m_size) {
    printf("\tCopy string_view: '%s'\n", this->m_value);
    printf("\trhs string_view: '%d'\n", rhs.m_size);
    printf("\tthis string_view: '%d'\n", this->m_size);
  }

  ~string_view() {
    printf("\tDestruct string_view: '%s'\n", this->m_value);
    this->m_size = 0u;
    this->m_value = nullptr;
  }

  const char *c_str() const { return this->m_value; }

  const uint size() const { return this->m_size; }
};

void print(const string_view &msg) {
  const string_view copy(msg);
  printf("print('%s')\n", copy.c_str());
}

int main(int argc, char **argv) {
  const char *raw = "Hello World";

  string_view sv1(raw);

  print(sv1);

  return 11u - sv1.size();
}
