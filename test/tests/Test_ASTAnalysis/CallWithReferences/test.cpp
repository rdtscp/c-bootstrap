// namespace atl {
//   typedef unsigned int uint;

//   void foo(uint &param) {

//   }

//   void bar() {
//     uint x = 0u;
//     foo(x);
//   }
// }

typedef unsigned int uint;
void swap(uint &lhs, uint &rhs) {
  uint temp = lhs;
  lhs = rhs;
  rhs = lhs;
} 
void swap(char * &lhs, char * &rhs) {
  char *temp = lhs;
  lhs = rhs;
  rhs = lhs;
} 

class string {
public:
typedef unsigned int uint;
  uint m_size;
  char * m_ptr;

  string &operator=(string rhs) {
    swap(m_size, rhs.m_size);
    swap(m_ptr, rhs.m_ptr);

  }

};
