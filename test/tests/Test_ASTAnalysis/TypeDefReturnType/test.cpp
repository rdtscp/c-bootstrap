class string {
public:
  typedef unsigned int uint;

  void foo() {
    uint x = this->size();
  }

  uint size() const { return 64; }

};
