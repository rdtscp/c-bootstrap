class MyClass {
public:
  const int func(const int input) const;
  int func(const int input);
  // int operator[](const int input);
  // const int operator[](const int input) const;
};

int main(int argc, char **argv) {
  MyClass myObj;
  int regular1 = myObj.func(5);
  // int regular2 = myObj[5];

  const MyClass myObjConst;
  int consted1 = myObjConst.func(5);
  // int consted2 = myObjConst[5];

  return 0;
}
