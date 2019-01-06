
#define __x86_64__

#if !defined(__sys_cdefs_arch_unknown__) && defined(__i386__)
int one();
#elif !defined(__sys_cdefs_arch_unknown__) && defined(__x86_64__)
int two();
#else
int three();
#error Unsupported architecture
#endif