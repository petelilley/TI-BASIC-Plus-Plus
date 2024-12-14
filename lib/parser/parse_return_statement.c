#include "parser_internal.h"

ast_node_t* parse_return_statement(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  if (compare_keyword(*t, 1, KW_RETURN) != KW_RETURN) {
    unexpected_token(*t, TOKEN_KEYWORD, d);
    return NULL;
  }

  (void)token_next(t);

  ast_node_t* expression = NULL;

  if (compare_token(*t, 1, TOKEN_NEWLINE) != TOKEN_NEWLINE) {
    expression = parse_expression(t, d);
    if (expression == NULL) {
      return NULL;
    }

    // Newline

    if (compare_token(*t, 1, TOKEN_NEWLINE) != TOKEN_NEWLINE) {
      unexpected_token(*t, TOKEN_NEWLINE, d);
      goto CLEANUP;
    }
  }

  (void)token_next(t);

  return ast_node_create_return_statement(expression, start_location);

CLEANUP:
  ast_node_destroy(expression);
  return NULL;
}

