#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);

typedef struct {
  int quot;
  int rem;
} div_t;

#ifdef __cplusplus
}
#endif

#endif
