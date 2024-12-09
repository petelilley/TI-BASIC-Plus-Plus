#include "parser_internal.h"

ast_node_t* parse_if_statement(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  // if
  
  keyword_kind_t keyword_kind = compare_keyword(*t, 1, KW_IF);
  if (keyword_kind == KW_UNKNOWN) {
    unexpected_token(*t, TOKEN_KEYWORD, d);
    return NULL;
  }

  (void)token_next(t);

  // TODO: Parse expression...


  return NULL;
}

