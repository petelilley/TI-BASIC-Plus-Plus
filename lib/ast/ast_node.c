#include <ti-basic-plus-plus/ast/ast_node.h>

#include <ti-basic-plus-plus/macros.h>

ast_node_t* ast_node_create(ast_node_kind_t kind) {
  ast_node_t* node = (ast_node_t*)malloc(sizeof(ast_node_t));
  assert(node != NULL);

  (void)memset(node, 0, sizeof(ast_node_t));
  node->kind = kind;

  return node;
}

void ast_node_destroy(ast_node_t* node) {
  assert(node != NULL);

  size_t num_children = arrlenu(node->children);
  for (size_t i = 0; i < num_children; i++) {
    ast_node_destroy(node->children[i]);
  }

  arrfree(node->children);
  free(node);
}

ast_node_t* ast_node_create_root(input_file_t* input_file) {
  assert(input_file != NULL);

  ast_node_t* node = ast_node_create(AST_ROOT);
  node->location.file = input_file;

  return node;
}

ast_node_t* ast_node_create_variable_reservation(
    variable_t variable,
    source_range_t location,
    source_range_t error_location) {
  ast_node_t* node = ast_node_create(AST_VARIABLE_RESERVATION);
  node->location = location;
  node->error_location = error_location;
  node->data.reserved_variable = variable;

  return node;
}

ast_node_t* ast_node_create_variable_decl(variable_type_t type,
                                          const char* name,
                                          ast_node_t* expr,
                                          bool is_constant,
                                          source_range_t location,
                                          source_range_t error_location) {
  assert(type != VAR_UNKNOWN);
  assert(name != NULL);

  ast_node_t* node = ast_node_create(AST_VARIABLE_DECL);
  node->location = location;
  node->error_location = error_location;
  node->data.variable_decl.type = type;
  node->data.variable_decl.name = name;
  node->data.variable_decl.expr = expr;
  node->data.variable_decl.is_constant = is_constant;

  return node;
}

ast_node_t* ast_node_create_function_decl(variable_type_t return_type,
                                          const char* name,
                                          ast_node_t** parameters,
                                          ast_node_t* body,
                                          source_range_t location,
                                          source_range_t error_location) {
  assert(return_type != VAR_UNKNOWN);
  assert(name != NULL);
  /* assert(body != NULL); */

  ast_node_t* node = ast_node_create(AST_FUNCTION_DECL);
  node->location = location;
  node->error_location = error_location;
  node->data.function_decl.return_type = return_type;
  node->data.function_decl.name = name;
  node->data.function_decl.parameters = parameters;
  node->data.function_decl.body = body;

  return node;
}

ast_node_t* ast_node_create_unary_expr(operator_kind_t operator,
                                       ast_node_t * operand,
                                       source_range_t location) {
  assert(operator!= OP_UNKNOWN);
  assert(operand != NULL);

  ast_node_t* node = ast_node_create(AST_UNARY_EXPRESSION);
  node->location = node->error_location = location;
  node->data.operator= operator;
  arrput(node->children, operand);

  return node;
}

ast_node_t* ast_node_create_binary_expr(ast_node_t* lhs,
                                        operator_kind_t operator,
                                        ast_node_t * rhs,
                                        source_range_t location) {
  assert(operator!= OP_UNKNOWN);
  assert(lhs != NULL);
  assert(rhs != NULL);

  ast_node_t* node = ast_node_create(AST_BINARY_EXPRESSION);
  node->location = node->error_location = location;
  node->data.operator= operator;
  arrput(node->children, lhs);
  arrput(node->children, rhs);

  return node;
}

ast_node_t* ast_node_create_function_call(const char* name,
                                          ast_node_t** arguments,
                                          source_range_t location,
                                          source_range_t error_location) {
  assert(name != NULL);

  ast_node_t* node = ast_node_create(AST_FUNCTION_CALL);
  node->location = location;
  node->error_location = error_location;
  node->data.identifier = name;
  node->children = arguments;

  return node;
}
