
namespace Namespace {

class Class {
public:
  int m_val;

  Class(int val) : m_val(val) {}
  Class(const Class &rhs) : m_val(rhs.m_val) {}
  ~Class() {}
};

// TODO: Return the `Class` object.
int getClass(const int val) {
  Class output(val);
  return output.m_val;
}

} // namespace Namespace

// TODO: Use this function.
int getClass(const int val) { return Namespace::getClass(val); }

int main(int argc, char **argv) {
  Namespace::Class obj = Namespace::getClass(5);
  return 0;
}
