#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <ti-basic-plus-plus/lexer/keyword.h>

typedef enum variable_type {
  VAR_UNKNOWN = 0,

  VAR_VOID,
  VAR_NUMBER,
  VAR_STRING,
  VAR_MATRIX,
  VAR_LIST,

  VAR_MATRIX_ELEMENT,
  VAR_LIST_ELEMENT,
} variable_type_t;

variable_type_t keyword_to_variable_type(keyword_kind_t kind);

typedef struct variable {
  variable_type_t type;
  union {
    char letter;
    uint8_t index;
  } id;
} variable_t;

#endif  // VARIABLE_H

