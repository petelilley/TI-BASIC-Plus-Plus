#include <ti-basic-plus-plus/calculator/variable.h>

variable_type_t keyword_to_variable_type(keyword_kind_t kind) {
  (void)kind;

  return VAR_UNKNOWN;
}

bool match_variable(const char* string, size_t length, variable_t* var) {
  (void)string;
  (void)length;
  (void)var;

  return false;
}
 
