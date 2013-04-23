#ifndef __GLOBAL__
#define __GLOBAL__


void *operator new(unsigned int s, const char *filename, int line);
void *operator new[](unsigned int s, const char *filename, int line);
void operator delete(void* p, const char *filename, int line);
void operator delete[](void* p, const char *filename, int line);
#define new new(__FILE__, __LINE__)

#endif

