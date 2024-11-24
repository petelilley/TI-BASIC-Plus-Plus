#include <ti-basic-plus-plus/parser/ast_node.h>

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

ast_node_t* ast_node_create_variable_reservation(
    variable_t variable,
    source_range_t location,
    source_range_t error_location) {
  assert(variable.type != VAR_UNKNOWN);

  ast_node_t* node = ast_node_create(AST_VARIABLE_RESERVATION);
  node->location = location;
  node->error_location = error_location;
  node->data.reserved_variable = variable;

  return node;
}

void emit_ast(ast_node_t* root, FILE* stream) {
  (void)root;
  (void)stream;
}

