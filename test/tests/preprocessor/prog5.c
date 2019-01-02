#define varA 1

#if varA
#define varB 0
int funB0() {}
#else
#define varB 1
int funB1() {}

#endif

#if varB
#define varC 1
int func1() {}
#else
#define varC 0
int funC0() {}
#endif

int main() {}