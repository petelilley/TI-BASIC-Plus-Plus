#ifndef BUILTIN_FUNCTION_H
#define BUILTIN_FUNCTION_H

#include <stddef.h>

typedef enum builtin_function {
  FUNC_UNKNOWN = 0,
  FUNC_DISP,
  FUNC_LINE,
  FUNC_CIRCLE,
  FUNC_TEXT,
  FUNC_CLRDRAW,
} builtin_function_t;

builtin_function_t match_builtin_function(const char* string, size_t length);

#endif  // BUILTIN_FUNCTION_H

