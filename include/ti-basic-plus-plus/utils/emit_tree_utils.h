#ifndef EMIT_TREE_UTILS_H
#define EMIT_TREE_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef void(emit_tree_section_build_func_t)(void* data,
                                             size_t indent_size,
                                             unsigned indents,
                                             FILE* stream);

typedef void(emit_tree_element_build_func_t)(void* data, FILE* stream);

void emit_tree_section(FILE* stream,
                       size_t indent_size,
                       unsigned indents,
                       const char* section_name,
                       bool last_section,
                       emit_tree_section_build_func_t* elements_build,
                       emit_tree_section_build_func_t* children_build,
                       void* data);

void emit_tree_element(FILE* stream,
                       size_t indent_size,
                       unsigned indents,
                       const char* element_name,
                       emit_tree_element_build_func_t* element_build,
                       void* data);

void emit_tree_element_fmt(FILE* stream,
                           size_t indent_size,
                           unsigned indents,
                           const char* element_name,
                           const char* fmt,
                           ...);

#endif  // EMIT_TREE_UTILS_H

