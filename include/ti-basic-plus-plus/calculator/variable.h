#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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
const char* variable_type_to_string(variable_type_t type);

typedef struct variable {
  variable_type_t type;

  union {
    char letter;
    uint8_t index;
  } id;

  // For matrix/list elements.
  int index_x, index_y;
} variable_t;

void emit_variable(const variable_t* variable, size_t indent_num, FILE* stream);

#endif  // VARIABLE_H

