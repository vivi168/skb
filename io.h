#ifndef PCVER
#include <sys/types.h>

char* load_file(char*, u_long*);
#else
char* load_file(char*, int*);
#endif
