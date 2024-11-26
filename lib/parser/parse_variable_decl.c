#include "parser_internal.h"

ast_node_t* parse_variable_decl(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  // <type>

  keyword_kind_t keyword_kind =
      compare_keyword(*t, 5, KW_VOID, KW_NUMBER, KW_STRING, KW_MATRIX, KW_LIST);
  if (keyword_kind == KW_UNKNOWN) {
    unexpected_token(*t, TOKEN_KEYWORD, d);
    return NULL;
  }

  (void)token_next(t);

  // <identifier>

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) != TOKEN_IDENTIFIER) {
    unexpected_token(*t, TOKEN_IDENTIFIER, d);
    return NULL;
  }

  source_range_t identifier_location = (*t)->location;

  char* identifier = (*t)->data.string;

  (void)token_next(t);

  // :, =

  punctuator_kind_t punct = compare_punctuator(*t, 2, PUNCT_COLON, PUNCT_ASGN);

  if (punct == PUNCT_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, ": or =", d);
    return NULL;
  }

  if (punct == PUNCT_COLON) {
    (void)token_next(t);

    // <variable>

    variable_t variable = parse_calculator_variable(t, d);
    if (variable.type == VAR_UNKNOWN) {
      return NULL;
    }

    punct = compare_punctuator(*t, 1, PUNCT_ASGN);

    if (punct == PUNCT_UNKNOWN) {
      unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "=", d);
      return NULL;
    }
  }

  assert(punct == PUNCT_ASGN);

  (void)token_next(t);

  // <expression>

  // TODO: Implement

  // Just newline for now

  if (compare_token(*t, 1, TOKEN_NEWLINE) != TOKEN_NEWLINE) {
    unexpected_token_expected(*t, TOKEN_NEWLINE, "newline", d);
    return NULL;
  }

  source_range_t end_location = (*t)->location;

  (void)token_next(t);

  source_range_t location = range_cat(&start_location, &end_location);

  return ast_node_create_variable_decl(keyword_to_variable_type(keyword_kind),
                                       identifier, NULL, false, location,
                                       identifier_location);
}

