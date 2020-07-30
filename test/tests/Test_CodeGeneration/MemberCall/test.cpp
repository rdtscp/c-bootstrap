void printf(const char *, int);

class Base {
public:
  int m_size;

  int get_size() const {
    return this->m_size + 1;
  }
};

int main(int argc, char **argv) {
  Base obj;
  printf("Created obj for: %d\n", 13);
  obj.m_size = 13;
  printf("Hello: %d\n", obj.get_size());
  const int size = obj.get_size();
  printf("Hello: %d\n", size);
  return 0;
}
