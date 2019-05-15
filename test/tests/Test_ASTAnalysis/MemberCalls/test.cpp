class ClassOne {
public:
  int one_func() { return 1; }
};

class ClassTwo {
public:
  ClassOne two_func() {
    ClassOne obj;
    return obj;
  }
};

int main() {
  ClassTwo myObj;
  int val = myObj.two_func().one_func();
  return val;
}
