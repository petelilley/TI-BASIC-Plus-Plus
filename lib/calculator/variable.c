#include <ti-basic-plus-plus/calculator/variable.h>

variable_type_t keyword_to_variable_type(keyword_kind_t kind) {
  switch (kind) {
    case KW_VOID:
      return VAR_VOID;
    case KW_NUMBER:
      return VAR_NUMBER;
    case KW_STRING:
      return VAR_STRING;
    case KW_MATRIX:
      return VAR_MATRIX;
    case KW_LIST:
      return VAR_LIST;
    default:
      return VAR_UNKNOWN;
  }
}

