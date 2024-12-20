#include <ti-basic-plus-plus/basic/source_location.h>

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/basic/input_file.h>

bool range_is_valid(const source_range_t* range) {
  assert(range != NULL);

  if (range->file == NULL) {
    return false;
  }

  if (range->begin.line < 1 || range->begin.column < 1) {
    return false;
  }

  if (range->end.line < 1 || range->end.column < 1) {
    return false;
  }

  if (range->begin.line > range->end.line) {
    return false;
  }

  if (range->begin.line == range->end.line &&
      range->begin.column > range->end.column) {
    return false;
  }

  return true;
}

source_range_t range_create(struct input_file* file,
                            source_position_t begin,
                            source_position_t end) {
  assert(file != NULL);
  assert(are_positions_equal(&begin, &end) || begin.line < end.line ||
         (begin.line == end.line && begin.column <= end.column));

  return (source_range_t){
      .file = file,
      .begin = begin,
      .end = end,
  };
}

source_range_t range_at_current(input_file_iterator_t* it) {
  assert(it != NULL);

  return range_at(it->file, if_get_position(it));
}

source_range_t range_cat(source_range_t* begin, source_range_t* end) {
  assert(range_is_valid(begin));
  assert(range_is_valid(end));

  assert(begin->file == end->file);

  if (are_positions_equal(&begin->begin, &end->begin) &&
      are_positions_equal(&begin->end, &end->end)) {
    return *begin;
  }

  assert(begin->end.line < end->begin.line ||
         (begin->end.line == end->begin.line &&
          begin->end.column <= end->begin.column));

  source_range_t result = *begin;
  result.end = end->end;

  return result;
}

void range_emit(source_range_t* range,
                size_t indent_size,
                emit_tree_indent_data_t* indent,
                diagnostics_t* d) {
  assert(range != NULL);
  assert(d != NULL);

  emit_tree_element_fmt(d, indent_size, indent, "location",
                        "%s:%zu:%zu-%zu:%zu", range->file->path,
                        range->begin.line, range->begin.column, range->end.line,
                        range->end.column);
}

