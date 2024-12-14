#include "parser_internal.h"

ast_node_t* parse_while_loop(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  ast_node_t* while_condition = NULL;
  ast_node_t* while_body = NULL;

  if (!parse_condition_statement(t, KW_WHILE, &while_condition, &while_body, d)) {
    return NULL;
  }

  source_range_t end_location = (*t)->prev->location;

  source_range_t location = range_cat(&start_location, &end_location);

  return ast_node_create_while_loop(while_condition, while_body, location);
}
