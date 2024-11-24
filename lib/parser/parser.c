#include <ti-basic-plus-plus/parser/parser.h>

#include "parser_internal.h"

ast_node_t* parse_tokens(token_t* head_token, diagnostics_t* d) {
  assert(head_token != NULL);
  assert(d != NULL);

  ast_node_t* root = ast_node_create(AST_ROOT);

  while (head_token != NULL && head_token->kind != TOKEN_EOF &&
         !should_exit(d)) {
    ast_node_t* decl = parse_global_decl(&head_token, d);
    if (decl == NULL) {
      return root;
    }

    arrput(root->children, decl);
  }

  return root;
}

