#define varA1 0
#define varA2 0
#define varA3 1
#define varA4 0

#define varB1 0
#define varB2 0
#define varB4 0

#if varA1
int funA1() {}
#elif varA2
int funA2() {}
#elif varA3
int funA3() {}
#elif varA4
int funA4() {}
#else
int funA5() {}
#endif

#if varB1
int funB1() {}
#elif varB2
int funB2() {}
#elif varB3
int funB3() {}
#elif varB4
int funB4() {}
#else
int funB5() {}
#endif

int main() {}