
class SubscriptExample {
public:
  SubscriptExample(int x) : m_val(x) {}

  int operator[](const int y) { return m_val + y; }

private:
  int m_val;
};

int main(int argc, char **argv) {
  SubscriptExample se(5);
  int foo = se[1];
  return 1;
}
