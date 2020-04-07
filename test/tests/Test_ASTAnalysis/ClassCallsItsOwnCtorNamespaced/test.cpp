namespace atl {
class string {
public:
  /* Constructor */
  string(const char *string_literal) {
    const unsigned int string_length = 0u;
    string_value = new char[string_length + 1u];
    char *string_value_ptr = string_value;
    for (unsigned int i = 0u; i < string_length; ++i) {
      *string_value_ptr = *string_literal;
      ++string_value_ptr;
      ++string_literal;
    }
    *string_value_ptr = '\0';
  }

  string(const atl::string &rhs) {
    if (&rhs == this)
      return;

    m_size = rhs.size();
    string_value = new char[m_size + 1u];
    char *string_value_ptr = string_value;
    for (unsigned int idx = 0u; idx < m_size; ++idx) {
      *string_value_ptr = rhs[idx];
      ++string_value_ptr;
    }
    *string_value_ptr = '\0';
  }

  string operator+(const string &rhs) {
    // Calculate the new size.
    const unsigned int lhs_len = size();
    const unsigned int rhs_len = rhs.size();
    const unsigned int string_length = lhs_len + rhs_len;

    // Allocate memory for the new string.
    char *new_string_value = new char[string_length + 1];
    char *new_string_value_ptr = new_string_value;

    // Copy the LHS.
    char *lhs_ptr = string_value;
    for (unsigned int i = 0u; i < lhs_len; ++i) {
      *new_string_value_ptr = *lhs_ptr;
      ++lhs_ptr;
      ++new_string_value_ptr;
    }

    // Copy the RHS.
    char *rhs_ptr = rhs.c_str();
    for (unsigned int i = 0u; i < rhs_len; ++i) {
      *new_string_value_ptr = *rhs_ptr;
      ++rhs_ptr;
      ++new_string_value_ptr;
    }

    // Null terminate.
    *new_string_value_ptr = '\0';

    string output(new_string_value);
    delete[] new_string_value;
    return output;
  }

  const char operator[](const unsigned int index) const {
    return this->at(index);
  }

  const char &at(const unsigned int index) const {
    if (index < this->size()) {
      return string_value[index];
    }
    throw "atl::string::at Out of Bounds Exception";
  }

  char *c_str() { return string_value; }

  char *c_str() const { return string_value; }

  unsigned int size() const { return 0u; }

private:
  char *string_value;
  int m_size;
};

} // namespace atl
