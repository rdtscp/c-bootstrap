class string {
public:
  void append(const string &rhs) {}

  bool operator==(const string &rhs) const { return true; }

  bool operator!=(const string &rhs) const { return !(*this == rhs); }

};

string operator+(string lhs, const string &rhs) {
  lhs.append(rhs);
  return lhs;
}
