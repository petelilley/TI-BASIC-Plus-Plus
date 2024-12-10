#include <ti-basic-plus-plus/calculator/variable.h>

#include <assert.h>
#include <ti-basic-plus-plus/macros.h>
#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

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
      return "void";
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

void variable_emit(variable_t* variable,
                   size_t indent_size,
                   unsigned indents,
                   FILE* stream) {
  assert(variable != NULL);
  assert(stream != NULL);

  switch (variable->type) {
    case VAR_UNKNOWN:
    case VAR_VOID:

      emit_tree_element_fmt(stream, indent_size, indents, "variable",
                            "unknown");
      return;
    case VAR_NUMBER:
      emit_tree_element_fmt(stream, indent_size, indents, "variable", "%c",
                            variable->id.letter);
      return;
    case VAR_STRING:
      emit_tree_element_fmt(stream, indent_size, indents, "variable", "Str%d",
                            variable->id.index);
      return;
    case VAR_MATRIX:
      emit_tree_element_fmt(stream, indent_size, indents, "variable", "[%c]",
                            variable->id.letter);
      return;
    case VAR_LIST:
      emit_tree_element_fmt(stream, indent_size, indents, "variable", "L%d",
                            variable->id.index);
      return;
    case VAR_MATRIX_ELEMENT:
      emit_tree_element_fmt(stream, indent_size, indents, "variable",
                            "[%c](%d, %d)", variable->id.letter,
                            variable->index_x, variable->index_y);
      return;
    case VAR_LIST_ELEMENT:
      emit_tree_element_fmt(stream, indent_size, indents, "variable", "L%d(%d)",
                            variable->id.index, variable->index_x);
      return;
    default:
      UNREACHABLE();
  }
}

