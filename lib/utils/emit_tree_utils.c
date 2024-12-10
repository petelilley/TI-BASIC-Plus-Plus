#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

#include <assert.h>
#include <stdarg.h>

static inline void emit_tree_indent(FILE* stream,
                                    size_t indent_size,
                                    unsigned indents) {
  for (size_t i = 0; i < indent_size; ++i) {
    if (indents & (1 << i)) {
      fputs("│   ", stream);
    } else {
      fputs("    ", stream);
    }
  }
}

void emit_tree_section(FILE* stream,
                       size_t indent_size,
                       unsigned indents,
                       const char* section_name,
                       bool last_section,
                       emit_tree_section_build_func_t* elements_build,
                       emit_tree_section_build_func_t* children_build,
                       void* data) {
  assert(stream != NULL);
  assert(section_name != NULL);

  if (indent_size) {
    emit_tree_indent(stream, indent_size - 1, indents);
    if (last_section) {
      fputs("└─> ", stream);
      indents &= ~(1 << (indent_size - 1));
    } else {
      fputs("├─> ", stream);
    }
  } else {
    emit_tree_indent(stream, indent_size, indents);
  }

  indents |= 1 << indent_size;

  fprintf(stream, "%s\n", section_name);

  bool has_children = children_build != NULL;

  if (elements_build != NULL) {
    elements_build(data, indent_size + 1, indents, stream);
    emit_tree_indent(stream, indent_size + has_children, indents);
    fputc('\n', stream);
  }

  if (children_build != NULL) {
    children_build(data, indent_size + 1, indents, stream);
    if (!last_section) {
      emit_tree_indent(stream, indent_size, indents);
      fputc('\n', stream);
    }
  }
}

void emit_tree_element(FILE* stream,
                       size_t indent_size,
                       unsigned indents,
                       const char* element_name,
                       emit_tree_element_build_func_t* element_build,
                       void* data) {
  assert(stream != NULL);
  assert(element_name != NULL);
  assert(element_build != NULL);

  emit_tree_indent(stream, indent_size - 1, indents);

  fprintf(stream, "││ %s = ", element_name);
  /* fprintf(stream, "%s = ", element_name); */

  element_build(data, stream);
}

void emit_tree_element_fmt(FILE* stream,
                           size_t indent_size,
                           unsigned indents,
                           const char* element_name,
                           const char* fmt,
                           ...) {
  assert(stream != NULL);
  assert(element_name != NULL);
  assert(fmt != NULL);

  emit_tree_indent(stream, indent_size - 1, indents);

  fprintf(stream, "││ %s = ", element_name);
  /* fprintf(stream, "%s = ", element_name); */

  va_list args;
  va_start(args, fmt);
  vfprintf(stream, fmt, args);
  va_end(args);

  fputc('\n', stream);
}

