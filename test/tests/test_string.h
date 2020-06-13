#pragma once
// #include <iostream>

// #include "swap.h"

void printf(const char *, const char *);

namespace test {
  typedef unsigned int uint;
  void swap(uint &lhs, uint &rhs) {
    uint temp = lhs;
    lhs = rhs;
    rhs = temp;
  }
  void swap(char * &lhs, char * &rhs) {
    char *temp = lhs;
    lhs = rhs;
    rhs = temp;
  }
}

namespace test {

class string {
private:
  typedef unsigned int uint;
  uint m_size;
  char *m_value;

public:
  typedef const char *const_iterator;
  typedef char *iterator;

  /* Constructor */
  string() : m_size(0u), m_value(new char[1]) {
    *m_value = '\0';
  }

  /* Constructor */
  string(const uint count, const char c) : m_size(count), m_value(new char[m_size + 1u]) {
    for (uint idx = 0u; idx < m_size; ++idx) {
      m_value[idx] = c;
    }
    m_value[m_size] = '\0';
  }

  /* Constructor */
  string(const char *string_literal) : m_size(char_buf_len(string_literal)), m_value(new char[m_size + 1u]) {
    this->m_size = char_buf_len(string_literal);
    this->m_value = new char[m_size + 1u];
    for (uint idx = 0u; idx < m_size; ++idx) {
      this->m_value[idx] = *string_literal;
      ++string_literal;
    }
    this->m_value[m_size] = '\0';
  }

  /* Copy Constructor */
  string(const string &rhs) : m_size(rhs.m_size), m_value(new char[m_size + 1u]) {
    for (uint idx = 0u; idx < m_size; ++idx) {
      m_value[idx] = rhs[idx];
    }
    m_value[m_size] = '\0';
  }

  /* Assignment Operator */
  string &operator=(string rhs) {
    test::swap(m_size, rhs.m_size);
    test::swap(m_value, rhs.m_value);
    return *this;
  }

  /* Move Constructor */
  string(string &&rhs) : m_size(0u), m_value(nullptr) {
    test::swap(m_size, rhs.m_size);
    test::swap(m_value, rhs.m_value);
  }

  /* Destructor */
  ~string() {
    m_size = 0u;
    delete[] m_value;
    m_value = nullptr;
  }

  char &operator[](const uint index) {
    return this->at(index);
  }

  const char operator[](const uint index) const {
    return this->at(index);
  }

  string &operator+=(const char rhs) {
    this->append(string(1u, rhs));
    return *this;
  }

  string &operator+=(const char *rhs) {
    this->append(string(rhs));
    return *this;
  }

  string &operator+=(const string &rhs) {
    this->append(rhs);
    return *this;
  }

  bool operator<(const string &rhs) const {
    if (m_size < rhs.m_size) {
      return true;
    }

    if (m_size > rhs.m_size) {
      return false;
    }

    for (uint idx = 0u; idx < m_size; ++idx) {
      const char currLhs = this->at(idx);
      const char currRhs = rhs.at(idx);
      if (static_cast<int>(currLhs) < static_cast<int>(currRhs)) {
        return true;
      }
    }
    return false;
  }

  bool operator>(const string &rhs) const {
    if (m_size < rhs.m_size) {
      return false;
    }
    if (m_size > rhs.m_size) {
      return true;
    }

    for (uint idx = 0u; idx < m_size; ++idx) {
      const char currLhs = this->at(idx);
      const char currRhs = rhs.at(idx);
      if (static_cast<int>(currLhs) > static_cast<int>(currRhs)) {
        return true;
      }
    }

    return false;
  }

  bool operator<=(const string &rhs) const {
    if (m_size < rhs.m_size) {
      return true;
    }
    if (m_size > rhs.m_size) {
      return false;
    }

    for (uint idx = 0u; idx < m_size; ++idx) {
      const char currLhs = this->at(idx);
      const char currRhs = rhs.at(idx);
      if (static_cast<int>(currLhs) > static_cast<int>(currRhs)) {
        return false;
      }
    }
    return true;
  }

  bool operator>=(const string &rhs) const {
    if (m_size < rhs.m_size) {
      return false;
    }
    if (m_size > rhs.m_size) {
      return true;
    }

    for (uint idx = 0u; idx < m_size; ++idx) {
      const char currLhs = this->at(idx);
      const char currRhs = rhs.at(idx);
      if (static_cast<int>(currLhs) < static_cast<int>(currRhs)) {
        return false;
      }
    }

    return true;
  }

  bool operator==(const string &rhs) const {
    if (m_size != rhs.m_size) {
      return false;
    }

    for (uint idx = 0u; idx < m_size; ++idx) {
      if (this->at(idx) != rhs.at(idx)) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const string &rhs) const { return !(*this == rhs); }

  bool operator<(const char *rhs) const { return *this < string(rhs); }

  bool operator>(const char *rhs) const { return *this > string(rhs); }

  bool operator<=(const char *rhs) const { return *this <= string(rhs); }

  bool operator>=(const char *rhs) const { return *this >= string(rhs); }

  bool operator==(const char *rhs) const { return *this == string(rhs); }

  bool operator!=(const char *rhs) const { return !(*this == rhs); }

  char &at(const uint index) {
    if (index < this->size()) {
      return m_value[index];
    }
    printf("%s\n", "test::string::at Out of Bounds Exception");
    throw;
  }

  const char &at(const uint index) const {
    if (index < this->size()) {
      return m_value[index];
    }
    printf("%s\n", "test::string::at Out of Bounds Exception");
    throw;
  }

  void append(const string &rhs) {
    if (rhs.size() == 0u) {
      return;
    }

    if (rhs.size() == 1u && rhs[0u] == '\0') {
      return;
    }

    const uint new_size = m_size + rhs.m_size;

    // Allocate memory for the new string.
    char *new_string_value = new char[new_size + 1u];
    char *new_string_value_ptr = new_string_value;

    // Copy the LHS.
    const char *lhs_ptr = m_value;
    for (uint idx = 0u; idx < m_size; ++idx) {
      *new_string_value_ptr = *lhs_ptr;
      ++lhs_ptr;
      ++new_string_value_ptr;
    }

    // Copy the RHS.
    const char *rhs_ptr = rhs.m_value;
    for (uint idx = 0u; idx < rhs.m_size; ++idx) {
      *new_string_value_ptr = *rhs_ptr;
      ++rhs_ptr;
      ++new_string_value_ptr;
    }

    // Null terminate.
    *new_string_value_ptr = '\0';

    delete[] m_value;
    m_size = new_size;
    m_value = new_string_value;
  }

  const_iterator begin() const { return &m_value[0u]; }

  const_iterator end() const {
    const uint len = this->size();
    return &m_value[len];
  }

  char *c_str() { return m_value; }

  char *c_str() const { return m_value; }

  int find(const char searchChar) const {
    const uint len = this->length();
    char currChar;
    for (uint idx = 0u; idx < len; ++idx) {
      currChar = this->at(idx);
      if (currChar == searchChar) {
        return static_cast<int>(idx);
      }
    }
    return -1;
  }

  uint length() const { return this->size(); }

  uint size() const { return m_size; }

private:
  static uint char_buf_len(const char *buf) {
    uint length = 0u;
    while (*buf != '\0') {
      ++length;
      ++buf;
    }
    return length;
  }
};

/* operator+ */

static string operator+(string lhs, const char rhs) {
  lhs += rhs;
  return lhs;
}

static string operator+(string lhs, const char *rhs) {
  lhs += rhs;
  return lhs;
}

static string operator+(string lhs, const string &rhs) {
  lhs += rhs;
  return lhs;
}

static string operator+(const char *lhs, const string &rhs) {
  return string(lhs) + rhs;
}

/* Misc */

static bool isspace(const char c) {
  return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

static bool isalpha(const char c) {
  return ((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')));
}

static bool isdigit(const char c) { return (c >= '0' && c <= '9'); }

/* https://www.geeksforgeeks.org/write-your-own-atoi/ */
static int stoi(const char *str) {
  int output = 0;              // Initialize result
  int sign = 1;                // Initialize sign as positive
  unsigned int start_idx = 0u; // Initialize index of first digit

  // If number is negative, then update sign
  if (str[0] == '-') {
    sign = -1;
    ++start_idx; // Also update index of first digit
  }

  // Iterate through all digits and update the result
  for (unsigned int idx = start_idx; str[idx] != '\0'; ++idx)
    output = output * 10 + static_cast<int>(str[idx]) - static_cast<int>('0');

  // Return result with sign
  return sign * output;
}

static int stoi(const string &str) { return stoi(str.c_str()); }

/* https://www.geeksforgeeks.org/implement-itoa/ */
static string to_string(int num) {
  bool isNegative = false;

  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0)
    return string("0");

  // In standard itoa(), negative numbers are handled only with
  // base 10. Otherwise numbers are considered unsigned.
  if (num < 0) {
    isNegative = true;
    num = -num;
  }

  // Process individual digits
  string reverseDigits;
  while (num != 0) {
    unsigned int rem = num % 10;
    const char next_char =
        (rem > 9u)
            ? static_cast<char>((rem - 10u) + static_cast<unsigned int>('a'))
            : static_cast<char>(rem + static_cast<unsigned int>('0'));
    reverseDigits += next_char;
    num = num / 10;
  }

  // If number is negative, append '-'
  if (isNegative)
    reverseDigits += '-';

  // Reverse the string
  string output;
  for (int idx = static_cast<int>(reverseDigits.size()) - 1; idx >= 0; --idx) {
    const unsigned int index = static_cast<unsigned int>(idx);
    char currChar = reverseDigits[index];
    output += currChar;
  }

  return output;
}

} // namespace test
