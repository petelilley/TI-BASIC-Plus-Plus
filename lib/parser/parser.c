#include <ti-basic-plus-plus/parser/parser.h>

#include "parser_internal.h"

#include <assert.h>

ast_node_t* parse_tokens(token_t* head_token, diagnostics_t* d) {
  assert(head_token != NULL);
  assert(d != NULL);

  ast_node_t* root = ast_node_create(AST_ROOT);
  assert(root != NULL);

  while (head_token != NULL && head_token->kind != TOKEN_EOF && !should_exit(d)) {
    ast_node_t* global_decl = parse_global_decl(&head_token, d);
    if (global_decl == NULL) {
      return root;
    }

    arrput(root->children, global_decl);
  }

  return root;
}

token_kind_t compare_token(const token_t* t, size_t n, ...) {
  (void)t;
  (void)n;

  return TOKEN_UNKNOWN;
}

punctuator_kind_t compare_punctuator(const token_t* t, size_t n, ...) {
  (void)t;
  (void)n;

  return PUNCT_UNKNOWN;
}

keyword_kind_t compare_keyword(const token_t* t, size_t n, ...) {
  (void)t;
  (void)n;

  return KW_UNKNOWN;
}

void unexpected_token(token_t* t, token_kind_t kind, diagnostics_t* d) {
  (void)t;
  (void)kind;
  (void)d;
}
