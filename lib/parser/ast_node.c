#include <ti-basic-plus-plus/parser/ast_node.h>

ast_node_t* ast_node_create(ast_node_kind_t kind) {
  (void)kind;

  return NULL;
}

void ast_node_destroy(ast_node_t* node) {
  (void)node;
}

void emit_ast(ast_node_t* root, FILE* stream) {
  (void)root;
  (void)stream;
}
