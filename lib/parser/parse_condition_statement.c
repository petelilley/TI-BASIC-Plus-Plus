#include "parser_internal.h"

bool parse_condition_statement(token_t** t,
                               keyword_kind_t keyword,
                               ast_node_t** condition_out,
                               ast_node_t** body_out,
                               diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(keyword != KW_UNKNOWN);
  assert(condition_out != NULL);
  assert(body_out != NULL);
  assert(d != NULL);

  *condition_out = NULL;
  *body_out = NULL;

  keyword_kind_t keyword_kind = compare_keyword(*t, 1, keyword);
  if (keyword_kind == KW_UNKNOWN) {
    unexpected_token(*t, TOKEN_KEYWORD, d);
    return false;
  }

  (void)token_next(t);

  ast_node_t* condition = parse_expression(t, d);
  if (condition == NULL) {
    return false;
  }

  // Newline

  while ((*t)->kind == TOKEN_NEWLINE) {
    (void)token_next(t);
  }

  // {

  source_range_t body_location = (*t)->location;

  if (compare_punctuator(*t, 1, '{') != '{') {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "'{'", d);
    goto CLEANUP;
  }

  ast_node_t* body = parse_statement_block(t, d);
  if (should_exit(d)) {
    goto CLEANUP;
  }

  *condition_out = condition;
  *body_out = body;

  if (body == NULL) {
    diag_report_source(d, &body_location, WARNING_EMPTY_CONDITION_STATEMENT_BODY,
                       keyword_to_string(keyword));
  }

  return true;

CLEANUP:
  if (condition) {
    ast_node_destroy(condition);
  }
  return false;
}

