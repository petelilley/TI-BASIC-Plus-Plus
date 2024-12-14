#include "parser_internal.h"

ast_node_t* parse_if_statement(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  ast_node_t* if_condition = NULL;
  ast_node_t* if_body = NULL;

  ast_node_t** elif_statements = NULL;
  ast_node_t* else_statement = NULL;

  // if

  if (!parse_condition_statement(t, KW_IF, &if_condition, &if_body, d)) {
    return NULL;
  }

  // elif

  while (compare_keyword(*t, 1, KW_ELIF) == KW_ELIF) {
    ast_node_t* elif_condition = NULL;
    ast_node_t* elif_body = NULL;

    source_range_t elif_start_location = (*t)->location;

    if (!parse_condition_statement(t, KW_ELIF, &elif_condition, &elif_body,
                                   d)) {
      goto CLEANUP;
    }

    source_range_t elif_end_location = (*t)->prev->location;
    source_range_t location =
        range_cat(&elif_start_location, &elif_end_location);

    ast_node_t* elif =
        ast_node_create_else_if_statement(elif_condition, elif_body, location);

    arrput(elif_statements, elif);
  }

  // else

  if (compare_keyword(*t, 1, KW_ELSE) == KW_ELSE) {
    source_range_t else_start_location = (*t)->location;

    (void)token_next(t);

    // Newline

    while ((*t)->kind == TOKEN_NEWLINE) {
      (void)token_next(t);
    }

    // {

    source_range_t body_location = (*t)->location;

    if (compare_punctuator(*t, 1, '{') != '{') {
      unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "'{'", d);
      return NULL;
    }

    ast_node_t* else_body = parse_statement_block(t, d);
    if (should_exit(d)) {
      return NULL;
    }

    if (else_body == NULL) {
      diag_report_source(d, &body_location,
                         WARNING_EMPTY_CONDITION_STATEMENT_BODY, "else");
    }

    source_range_t else_end_location = (*t)->prev->location;
    source_range_t location =
        range_cat(&else_start_location, &else_end_location);

    else_statement =
        ast_node_create_else_if_statement(NULL, else_body, location);
  }

  source_range_t end_location = (*t)->prev->location;
  source_range_t location = range_cat(&start_location, &end_location);

  ast_node_t* if_statement = ast_node_create_if_statement(
      if_condition, if_body, elif_statements, else_statement, location);

  return if_statement;

CLEANUP:
  for (size_t num = arrlenu(elif_statements), i = 0; i < num; ++i) {
    ast_node_destroy(elif_statements[i]);
  }
  arrfree(elif_statements);

  return NULL;
}

