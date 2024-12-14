#include <ti-basic-plus-plus/ast/ast_node.h>

#include <ti-basic-plus-plus/basic/input_file.h>
#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

static void build_node(ast_node_t* node,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent,
                       bool last_node,
                       diagnostics_t* d);

static emit_tree_section_build_func_t build_elements_root;
static emit_tree_section_build_func_t build_children_root;

static emit_tree_section_build_func_t build_elements_variable_reservation;

static emit_tree_section_build_func_t build_elements_variable_declaration;
static emit_tree_section_build_func_t build_children_variable_declaration;

static emit_tree_section_build_func_t build_elements_function_declaration;
static emit_tree_section_build_func_t build_children_function_declaration;
static emit_tree_section_build_func_t build_children_function_parameters;

static emit_tree_section_build_func_t build_elements_statement_block;
static emit_tree_section_build_func_t build_children_statement_block;

static emit_tree_section_build_func_t build_elements_binary_expression;
static emit_tree_section_build_func_t build_children_binary_expression;

static emit_tree_section_build_func_t build_elements_unary_expression;
static emit_tree_section_build_func_t build_children_unary_expression;

static emit_tree_section_build_func_t build_elements_identifier_expression;
static emit_tree_section_build_func_t build_elements_numeric_literal;
static emit_tree_section_build_func_t build_elements_string_literal;

static emit_tree_section_build_func_t build_elements_function_call;
static emit_tree_section_build_func_t build_children_function_call;
static emit_tree_section_build_func_t build_children_function_arguments;

void ast_node_emit(ast_node_t* node, diagnostics_t* d) {
  build_node(node, 0, NULL, true, d);
}

static void build_node(ast_node_t* node,
                       size_t indent_size,
                       emit_tree_indent_data_t* indent,
                       bool last_node,
                       diagnostics_t* d) {
  assert(node != NULL);
  assert(d != NULL);

  const char* tag = NULL;
  emit_tree_section_build_func_t* elements_func = NULL;
  emit_tree_section_build_func_t* children_func = NULL;
  uint8_t color = 1;

  switch (node->kind) {
    case AST_ROOT:
      color = 31;
      tag = "root";
      elements_func = &build_elements_root;
      children_func = &build_children_root;
      break;
    case AST_VARIABLE_RESERVATION:
      color = 1;
      tag = "var_res";
      elements_func = &build_elements_variable_reservation;
      break;
    case AST_VARIABLE_DECL:
      color = 32;  // green
      tag = "var_decl";
      elements_func = &build_elements_variable_declaration;
      if (node->data.variable_decl.expr != NULL) {
        children_func = &build_children_variable_declaration;
      }
      break;
    case AST_FUNCTION_DECL:
      color = 34;  // blue
      tag = "func_decl";
      elements_func = &build_elements_function_declaration;
      if (node->data.function_decl.body != NULL ||
          node->data.function_decl.parameters != NULL) {
        children_func = &build_children_function_declaration;
      }
      break;
    case AST_STATEMENT_BLOCK:
      color = 35;  // magenta
      tag = "stmt_block";
      elements_func = &build_elements_statement_block;
      children_func = &build_children_statement_block;
      break;
    case AST_BINARY_EXPRESSION:
      color = 36;  // cyan
      tag = "bin_expr";
      elements_func = &build_elements_binary_expression;
      children_func = &build_children_binary_expression;
      break;
    case AST_UNARY_EXPRESSION:
      color = 36;  // cyan
      tag = "unary_expr";
      elements_func = &build_elements_unary_expression;
      children_func = &build_children_unary_expression;
      break;
    case AST_IDENTIFIER:
      color = 33;  // yellow
      tag = "identifier";
      elements_func = &build_elements_identifier_expression;
      break;
    case AST_NUMERIC_LITERAL:
      color = 33;  // yellow
      tag = "numeric_literal";
      elements_func = &build_elements_numeric_literal;
      break;
    case AST_STRING_LITERAL:
      color = 33;  // yellow
      tag = "string_literal";
      elements_func = &build_elements_string_literal;
      break;
    case AST_FUNCTION_CALL:
      color = 34;  // blue
      tag = "func_call";
      elements_func = &build_elements_function_call;
      if (node->children != NULL) {
        children_func = &build_children_function_call;
      }
      break;
    default:
      break;
  }

  if (tag != NULL) {
    emit_tree_section(d, indent_size, indent, tag, color, last_node,
                      elements_func, children_func, node);
  }
}

static void build_elements_root(void* _node,
                                size_t indent_size,
                                emit_tree_indent_data_t* indent,
                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_ROOT);

  const char* file_path = node->location.file->path;
  emit_tree_element_fmt(d, indent_size, indent, "file_path", "%s", file_path);
}

static void build_children_root(void* _node,
                                size_t indent_size,
                                emit_tree_indent_data_t* indent,
                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_ROOT);

  size_t num_decls = arrlenu(node->children);

  for (size_t i = 0; i < num_decls; ++i) {
    build_node(node->children[i], indent_size, indent, i == num_decls - 1, d);
  }
}

static void build_elements_variable_reservation(void* _node,
                                                size_t indent_size,
                                                emit_tree_indent_data_t* indent,
                                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_RESERVATION);

  range_emit(&node->location, indent_size, indent, d);

  variable_emit(&node->data.reserved_variable, indent_size, indent, d);
}

static void build_elements_variable_declaration(void* _node,
                                                size_t indent_size,
                                                emit_tree_indent_data_t* indent,
                                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_DECL);

  range_emit(&node->location, indent_size, indent, d);

  ast_variable_decl_data_t* data = &node->data.variable_decl;

  emit_tree_element_fmt(d, indent_size, indent, "type", "%s",
                        variable_type_to_string(data->type));

  emit_tree_element_fmt(d, indent_size, indent, "name", "%.*s",
                        (int)arrlen(data->name), data->name);

  emit_tree_element_fmt(d, indent_size, indent, "is_constant", "%s",
                        data->is_constant ? "true" : "false");
}

static void build_children_variable_declaration(void* _node,
                                                size_t indent_size,
                                                emit_tree_indent_data_t* indent,
                                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_VARIABLE_DECL);

  ast_variable_decl_data_t* data = &node->data.variable_decl;

  if (data->expr != NULL) {
    build_node(data->expr, indent_size, indent, true, d);
  }
}

static void build_elements_function_declaration(void* _node,
                                                size_t indent_size,
                                                emit_tree_indent_data_t* indent,
                                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_DECL);

  range_emit(&node->location, indent_size, indent, d);

  ast_function_decl_data_t* data = &node->data.function_decl;

  emit_tree_element_fmt(d, indent_size, indent, "return_type", "%s",
                        variable_type_to_string(data->return_type));

  emit_tree_element_fmt(d, indent_size, indent, "name", "%.*s",
                        (int)arrlen(data->name), data->name);
}

static void build_children_function_parameters(void* _node,
                                               size_t indent_size,
                                               emit_tree_indent_data_t* indent,
                                               diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_DECL);

  ast_function_decl_data_t* data = &node->data.function_decl;

  size_t num_params = arrlenu(data->parameters);
  for (size_t i = 0; i < num_params; ++i) {
    build_node(data->parameters[i], indent_size, indent, i == num_params - 1,
               d);
  }
}

static void build_children_function_declaration(void* _node,
                                                size_t indent_size,
                                                emit_tree_indent_data_t* indent,
                                                diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_DECL);

  ast_function_decl_data_t* data = &node->data.function_decl;

  if (data->parameters != NULL) {
    emit_tree_section(d, indent_size, indent, "parameters", 1,
                      data->body == NULL, NULL,
                      build_children_function_parameters, node);
  }

  if (data->body != NULL) {
    build_node(data->body, indent_size, indent, true, d);
  }
}

static void build_elements_statement_block(void* _node,
                                           size_t indent_size,
                                           emit_tree_indent_data_t* indent,
                                           diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_STATEMENT_BLOCK);

  range_emit(&node->location, indent_size, indent, d);
}

static void build_children_statement_block(void* _node,
                                           size_t indent_size,
                                           emit_tree_indent_data_t* indent,
                                           diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_STATEMENT_BLOCK);

  size_t num_stmts = arrlenu(node->children);
  for (size_t i = 0; i < num_stmts; ++i) {
    build_node(node->children[i], indent_size, indent, i == num_stmts - 1, d);
  }
}

static void build_elements_binary_expression(void* _node,
                                             size_t indent_size,
                                             emit_tree_indent_data_t* indent,
                                             diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_BINARY_EXPRESSION);

  range_emit(&node->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "operator", "%s",
                        op_to_string(node->data.operator));
}

static void build_children_binary_expression(void* _node,
                                             size_t indent_size,
                                             emit_tree_indent_data_t* indent,
                                             diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_BINARY_EXPRESSION);

  build_node(node->children[0], indent_size, indent, false, d);
  build_node(node->children[1], indent_size, indent, true, d);
}

static void build_elements_unary_expression(void* _node,
                                            size_t indent_size,
                                            emit_tree_indent_data_t* indent,
                                            diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_UNARY_EXPRESSION);

  range_emit(&node->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "operator", "%s",
                        op_to_string(node->data.operator));
}

static void build_children_unary_expression(void* _node,
                                            size_t indent_size,
                                            emit_tree_indent_data_t* indent,
                                            diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_UNARY_EXPRESSION);

  build_node(node->children[0], indent_size, indent, true, d);
}

static void build_elements_identifier_expression(
    void* _node,
    size_t indent_size,
    emit_tree_indent_data_t* indent,
    diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_IDENTIFIER);

  range_emit(&node->location, indent_size, indent, d);

  const char* identifier = node->data.identifier;

  emit_tree_element_fmt(d, indent_size, indent, "identifier", "%.*s",
                        (int)arrlen(identifier), identifier);
}

static void build_elements_numeric_literal(void* _node,
                                           size_t indent_size,
                                           emit_tree_indent_data_t* indent,
                                           diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_NUMERIC_LITERAL);

  range_emit(&node->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "%f",
                        node->data.numeric_literal);
}

static void build_elements_string_literal(void* _node,
                                          size_t indent_size,
                                          emit_tree_indent_data_t* indent,
                                          diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_STRING_LITERAL);

  range_emit(&node->location, indent_size, indent, d);

  const char* string_literal = node->data.string_literal;

  emit_tree_element_fmt(d, indent_size, indent, "value", "\"%.*s\"",
                        (int)arrlen(string_literal), string_literal);
}

static void build_elements_function_call(void* _node,
                                         size_t indent_size,
                                         emit_tree_indent_data_t* indent,
                                         diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_CALL);

  range_emit(&node->location, indent_size, indent, d);

  const char* identifier = node->data.identifier;

  emit_tree_element_fmt(d, indent_size, indent, "name", "%.*s",
                        (int)arrlen(identifier), identifier);
}

static void build_children_function_call(void* _node,
                                         size_t indent_size,
                                         emit_tree_indent_data_t* indent,
                                         diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_CALL);

  emit_tree_section(d, indent_size, indent, "arguments", 1, true, NULL,
                    build_children_function_arguments, node);
}

static void build_children_function_arguments(void* _node,
                                              size_t indent_size,
                                              emit_tree_indent_data_t* indent,
                                              diagnostics_t* d) {
  assert(_node != NULL);
  assert(d != NULL);
  ast_node_t* node = (ast_node_t*)_node;

  assert(node->kind == AST_FUNCTION_CALL);

  size_t num_args = arrlenu(node->children);
  for (size_t i = 0; i < num_args; ++i) {
    build_node(node->children[i], indent_size, indent, i == num_args - 1, d);
  }
}

