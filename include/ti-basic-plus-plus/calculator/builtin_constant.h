#ifndef BUILTIN_CONSTANT_H
#define BUILTIN_CONSTANT_H

#include <stddef.h>

typedef enum builtin_constant {
  CONST_UNKNOWN = 0,
  CONST_PI,
  CONST_E,
  CONST_I,
  // TODO: More
} builtin_constant_t;

builtin_constant_t match_builtin_constant(const char* string, size_t length);

#endif  // BUILTIN_CONSTANT_H

