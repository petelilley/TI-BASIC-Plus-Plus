#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

#include <assert.h>
#include <stb_ds.h>
#include <stdarg.h>
#include <ti-basic-plus-plus/basic/diagnostics.h>

struct emit_tree_indent_data {
  uint64_t indents;
  uint8_t* colors;
};

static inline void begin_color(diagnostics_t* d, uint8_t color) {
  assert(d != NULL);

  if (!d->out_colorize) {
    return;
  }
  fprintf(d->out_stream, "\033[0;1;%dm", color);
}

static inline void end_color(diagnostics_t* d) {
  assert(d != NULL);

  if (!d->out_colorize) {
    return;
  }
  fputs("\033[0m", d->out_stream);
}

static inline void emit_tree_indent(diagnostics_t* d,
                                    size_t size,
                                    uint64_t indents,
                                    uint8_t* colors) {
  assert(d != NULL);
  assert(arrlenu(colors) >= size);

  for (size_t i = 0; i < size; ++i) {
    bool show_line = indents & (1 << i);

    if (show_line) {
      begin_color(d, colors[i]);
      fputs("│   ", d->out_stream);
      end_color(d);
    } else {
      fputs("    ", d->out_stream);
    }
  }
}

static inline void emit_tree_indent_data(
    diagnostics_t* d,
    size_t indent_size,
    const emit_tree_indent_data_t* indent_data) {
  assert(indent_data != NULL);

  emit_tree_indent(d, indent_size, indent_data->indents, indent_data->colors);
}

static inline void arrput_or_replace(uint8_t** arr,
                                     size_t index,
                                     uint8_t value) {
  assert(arr != NULL);

  if (index >= arrlenu(*arr)) {
    arrsetlen(*arr, index + 1);
  }

  (*arr)[index] = value;
}

void emit_tree_section(diagnostics_t* d,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent,
                       const char* section_name,
                       uint8_t color,
                       bool last_section,
                       emit_tree_section_build_func_t* elements_build,
                       emit_tree_section_build_func_t* children_build,
                       void* data) {
  assert(d != NULL);
  assert(section_name != NULL);

  emit_tree_indent_data_t _indent = {0};
  if (indent == NULL) {
    indent = &_indent;
  }

  arrput_or_replace(&indent->colors, indent_size, color);

  if (indent_size) {
    emit_tree_indent_data(d, indent_size - 1, indent);
    begin_color(d, indent->colors[indent_size - 1]);
    if (last_section) {
      fputs("└─> ", d->out_stream);
      indent->indents &= ~(1 << (indent_size - 1));
    } else {
      fputs("├─> ", d->out_stream);
    }
    end_color(d);
  } else {
    emit_tree_indent_data(d, indent_size, indent);
  }

  indent->indents |= 1 << indent_size;

  begin_color(d, color);
  fprintf(d->out_stream, "%s", section_name);
  end_color(d);
  fputc('\n', d->out_stream);

  bool has_children = children_build != NULL;

  if (elements_build != NULL) {
    elements_build(data, indent_size + 1, indent, d);
    emit_tree_indent_data(d, indent_size + has_children, indent);
    fputc('\n', d->out_stream);
  }

  if (children_build != NULL) {
    children_build(data, indent_size + 1, indent, d);
    if (!last_section) {
      emit_tree_indent_data(d, indent_size, indent);
      fputc('\n', d->out_stream);
    }
  }
}

void emit_tree_element(diagnostics_t* d,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent,
                       const char* element_name,
                       emit_tree_element_build_func_t* element_build,
                       void* data) {
  assert(d != NULL);
  assert(indent != NULL);
  assert(element_name != NULL);
  assert(element_build != NULL);

  emit_tree_indent_data(d, indent_size - 1, indent);

  begin_color(d, indent->colors[indent_size - 1]);
  fputs("│", d->out_stream);
  end_color(d);
  fprintf(d->out_stream, "│ %s", element_name);

  if (element_build != NULL) {
    fprintf(d->out_stream, " = ");

    element_build(data, d);
  }

  fputc('\n', d->out_stream);
}

void emit_tree_element_fmt(diagnostics_t* d,
                           size_t indent_size,
                           emit_tree_indent_data_t* indent,
                           const char* element_name,
                           const char* fmt,
                           ...) {
  assert(d != NULL);
  assert(indent != NULL);
  assert(element_name != NULL);
  assert(fmt != NULL);

  emit_tree_indent_data(d, indent_size - 1, indent);

  begin_color(d, indent->colors[indent_size - 1]);
  fputs("│", d->out_stream);
  end_color(d);
  fprintf(d->out_stream, "│ %s = ", element_name);

  va_list args;
  va_start(args, fmt);
  vfprintf(d->out_stream, fmt, args);
  va_end(args);

  fputc('\n', d->out_stream);
}

