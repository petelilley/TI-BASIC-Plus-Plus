#include <ti-basic-plus-plus/ast/ast_node.h>

#include <ti-basic-plus-plus/basic/input_file.h>
#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

static void build_node(void* _node,
                       size_t indent_size,
                       unsigned indents,
                       bool last_node,
                       FILE* stream);

static emit_tree_section_build_func_t build_elements_root;
static emit_tree_section_build_func_t build_children_root;

static emit_tree_section_build_func_t build_elements_variable_reservation;

static emit_tree_section_build_func_t build_elements_variable_declaration;
static emit_tree_section_build_func_t build_children_variable_declaration;

static emit_tree_section_build_func_t build_elements_function_declaration;
static emit_tree_section_build_func_t build_children_function_declaration;

void ast_node_emit(ast_node_t* node, FILE* stream) {
  build_node(node, 0, 0, true, stream);
}

static void build_node(void* _node,
                       size_t indent_size,
                       unsigned indents,
                       bool last_node,
                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  const char* tag = NULL;
  emit_tree_section_build_func_t* elements_func = NULL;
  emit_tree_section_build_func_t* children_func = NULL;

  switch (node->kind) {
    case AST_ROOT:
      tag = "\033[0;1;31mroot\033[0m";
      elements_func = &build_elements_root;
      children_func = &build_children_root;
      break;
    case AST_VARIABLE_RESERVATION:
      tag = "var_res";
      elements_func = &build_elements_variable_reservation;
      break;
    case AST_VARIABLE_DECL:
      tag = "var_decl";
      elements_func = &build_elements_variable_declaration;
      children_func = &build_children_variable_declaration;
      break;
    case AST_FUNCTION_DECL:
      tag = "func_decl";
      elements_func = &build_elements_function_declaration;
      children_func = &build_children_function_declaration;
      break;
    default:
      break;
  }

  if (tag != NULL) {
    emit_tree_section(stream, indent_size, indents, tag, last_node, elements_func,
                      children_func, node);
  }
}

/*
static void build_root(void* _node,
                       size_t indent_size,
                       unsigned indents,
                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_ROOT);

  emit_tree_section(stream, indent_size, indents, "header", false,
                    build_root_header, node);
  emit_tree_section(stream, indent_size, indents, "code", true, build_root_code,
                    node);
}
*/

static void build_elements_root(void* _node,
                              size_t indent_size,
                              unsigned indents,
                              FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_ROOT);

  const char* file_path = node->location.file->path;
  emit_tree_element_fmt(stream, indent_size, indents, "file_path", "%s",
                        file_path);
}

static void build_children_root(void* _node,
                            size_t indent_size,
                            unsigned indents,
                            FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_ROOT);

  size_t num_decls = arrlenu(node->children);

  for (size_t i = 0; i < num_decls; ++i) {
    build_node(node->children[i], indent_size, indents, i == num_decls - 1,
               stream);
  }
}

static void build_elements_variable_reservation(void* _node,
                                       size_t indent_size,
                                       unsigned indents,
                                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_RESERVATION);

  range_emit(&node->location, indent_size, indents, stream);

  variable_emit(&node->data.reserved_variable, indent_size, indents, stream);
}

static void build_elements_variable_declaration(void* _node,
                                       size_t indent_size,
                                       unsigned indents,
                                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_DECL);

  range_emit(&node->location, indent_size, indents, stream);

  ast_variable_decl_data_t* data = &node->data.variable_decl;

  emit_tree_element_fmt(stream, indent_size, indents, "type", "%s",
                        variable_type_to_string(data->type));

  emit_tree_element_fmt(stream, indent_size, indents, "name", "%.*s",
                        (int)arrlen(data->name), data->name);

  emit_tree_element_fmt(stream, indent_size, indents, "is_constant", "%s",
                        data->is_constant ? "true" : "false");
}

static void build_children_variable_declaration(void* _node,
                                       size_t indent_size,
                                       unsigned indents,
                                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_DECL);

  ast_variable_decl_data_t* data = &node->data.variable_decl;

  (void)data;
  (void)indent_size;
  (void)indents;
}

static void build_elements_function_declaration(void* _node,
                                       size_t indent_size,
                                       unsigned indents,
                                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_DECL);

  range_emit(&node->location, indent_size, indents, stream);

  ast_function_decl_data_t* data = &node->data.function_decl;

  (void)data;
}

static void build_children_function_declaration(void* _node,
                                       size_t indent_size,
                                       unsigned indents,
                                       FILE* stream) {
  assert(_node != NULL);
  assert(stream != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_DECL);

  ast_function_decl_data_t* data = &node->data.function_decl;

  size_t num_params = arrlenu(data->parameters);
  for (size_t i = 0; i < num_params; ++i) {
    build_node(data->parameters[i], indent_size, indents, i == num_params - 1,
               stream);
  }
}

