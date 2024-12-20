#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ti-basic-plus-plus/lexer/keyword.h>
#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

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

void variable_emit(variable_t* variable,
                   size_t indent_size,
                   emit_tree_indent_data_t* indent,
                   struct diagnostics* d);

#endif  // VARIABLE_H

