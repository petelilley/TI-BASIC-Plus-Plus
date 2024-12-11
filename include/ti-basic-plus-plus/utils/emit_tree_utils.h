#ifndef EMIT_TREE_UTILS_H
#define EMIT_TREE_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct diagnostics;

typedef struct emit_tree_indent_data emit_tree_indent_data_t;

typedef void(emit_tree_section_build_func_t)(
    void* data,
    size_t indent_size,
    emit_tree_indent_data_t* indent_data,
    struct diagnostics* d);

typedef void(emit_tree_element_build_func_t)(void* data, struct diagnostics* d);

void emit_tree_section(struct diagnostics* d,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent_data,
                       const char* section_name,
                       uint8_t color,
                       bool last_section,
                       emit_tree_section_build_func_t* elements_build,
                       emit_tree_section_build_func_t* children_build,
                       void* data);

void emit_tree_element(struct diagnostics* d,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent_data,
                       const char* element_name,
                       emit_tree_element_build_func_t* element_build,
                       void* data);

void emit_tree_element_fmt(struct diagnostics* d,
                           size_t indent_size,
                           emit_tree_indent_data_t* indent_data,
                           const char* element_name,
                           const char* fmt,
                           ...);

#endif  // EMIT_TREE_UTILS_H

