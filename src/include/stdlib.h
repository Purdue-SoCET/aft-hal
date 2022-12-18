#ifndef STDLIB_H_
#define STDLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void print(const char* x);
void putinthex(int x);
void *malloc(size_t sz);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H_ */
