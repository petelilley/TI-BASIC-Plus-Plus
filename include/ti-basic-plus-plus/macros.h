#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef NDEBUG  // Release
#define UNREACHABLE()                                                \
  do {                                                               \
    fprintf(stderr, "unreachable code reached in %s:%d\n", __FILE__, \
            __LINE__);                                               \
    exit(1);                                                         \
  } while (0)
#else
#define UNREACHABLE() assert(false)
#endif

#endif  // MACROS_H

