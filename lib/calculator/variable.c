#include <ti-basic-plus-plus/calculator/variable.h>

#include <assert.h>
#include <ti-basic-plus-plus/macros.h>

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

const char* variable_type_to_string(variable_type_t type) {
  switch (type) {
    case VAR_VOID:
      return "Void";
    case VAR_NUMBER:
      return "Number";
    case VAR_STRING:
      return "String";
    case VAR_MATRIX:
      return "Matrix";
    case VAR_LIST:
      return "List";
    case VAR_MATRIX_ELEMENT:
      return "Matrix Element";
    case VAR_LIST_ELEMENT:
      return "List Element";
    default:
      return "unknown";
  }
}

void emit_variable(const variable_t* variable,
                   size_t indent_num,
                   FILE* stream) {
  assert(variable != NULL);
  assert(stream != NULL);

  for (size_t i = 0; i < indent_num; ++i) {
    (void)fputc('\t', stream);
  }

  fputs("Variable: ", stream);

  switch (variable->type) {
    case VAR_UNKNOWN:
    case VAR_VOID:
      fputs("Unknown\n", stream);
      return;
    case VAR_NUMBER:
      fprintf(stream, "%c\n", variable->id.letter);
      return;
    case VAR_STRING:
      fprintf(stream, "Str%d\n", variable->id.index);
      return;
    case VAR_MATRIX:
      fprintf(stream, "[%c]\n", variable->id.letter);
      return;
    case VAR_LIST:
      fprintf(stream, "L%d\n", variable->id.index);
      return;
    case VAR_MATRIX_ELEMENT:
      fprintf(stream, "[%c](%d, %d)\n", variable->id.letter, variable->index_x,
              variable->index_y);
      return;
    case VAR_LIST_ELEMENT:
      fprintf(stream, "L%d(%d)\n", variable->id.index, variable->index_x);
      return;
    default:
      UNREACHABLE();
  }
}

