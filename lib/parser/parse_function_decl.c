#include "parser_internal.h"

static ast_node_t** parse_parameter_list(token_t** t, diagnostics_t* d);
static ast_node_t* parse_parameter(token_t** t, diagnostics_t* d);

ast_node_t* parse_function_decl(token_t** t, diagnostics_t* d) {
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

  // (

  if (compare_punctuator(*t, 1, PUNCT_LPAREN) != PUNCT_LPAREN) {
    unexpected_token(*t, TOKEN_PUNCTUATOR, d);
    return NULL;
  }

  (void)token_next(t);

  // <parameters>

  ast_node_t** parameters = parse_parameter_list(t, d);
  if (should_exit(d)) {
    return NULL;
  }

  // )

  if (compare_punctuator(*t, 1, PUNCT_RPAREN) != PUNCT_RPAREN) {
    unexpected_token(*t, TOKEN_PUNCTUATOR, d);
    return NULL;
  }

  source_range_t end_location = (*t)->location;

  (void)token_next(t);

  // TODO: Body

  // TODO: Newline

  source_range_t location = range_cat(&start_location, &end_location);

  return ast_node_create_function_decl(keyword_to_variable_type(keyword_kind),
                                       identifier, parameters, NULL,
                                       location, identifier_location);
}

static ast_node_t** parse_parameter_list(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  ast_node_t** parameters = NULL;

  token_kind_t token = compare_token(*t, 2, TOKEN_KEYWORD, TOKEN_PUNCTUATOR);

  const char* expected_type_or_rparen_str = "parameter type or )";
  const char* expected_type_str = "parameter type";

  const char* expected_str = expected_type_or_rparen_str;

  while (true) {
    if (token == TOKEN_UNKNOWN) {
      unexpected_token_expected(*t, TOKEN_UNKNOWN, expected_str, d);
      return NULL;
    }

    if (token == TOKEN_KEYWORD) {
      ast_node_t* parameter = parse_parameter(t, d);
      if (parameter == NULL || should_exit(d)) {
        goto CLEANUP;
      }

      arrput(parameters, parameter);

      token = TOKEN_PUNCTUATOR;
      punctuator_kind_t punct =
          compare_punctuator(*t, 2, PUNCT_RPAREN, PUNCT_COMMA);
      expected_str = expected_type_or_rparen_str;
      if (punct == PUNCT_UNKNOWN) {
        unexpected_token_expected(*t, TOKEN_PUNCTUATOR, ") or ,", d);
        goto CLEANUP;
      } else if (punct == PUNCT_COMMA) {
        (void)token_next(t);
        token = compare_token(*t, 1, TOKEN_KEYWORD);
        expected_str = expected_type_str;
      }

      continue;
    } else if (token == TOKEN_PUNCTUATOR) {
      punctuator_kind_t punct = compare_punctuator(*t, 1, PUNCT_RPAREN);
      if (punct == PUNCT_UNKNOWN) {
        unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "parameter type or )",
                                  d);
        return NULL;
      }

      break;
    }
  }

  return parameters;

CLEANUP:
  arrfree(parameters);
  return NULL;
}

static ast_node_t* parse_parameter(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  // <type>

  keyword_kind_t keyword_kind =
      compare_keyword(*t, 5, KW_VOID, KW_NUMBER, KW_STRING, KW_MATRIX, KW_LIST);
  if (keyword_kind == KW_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_KEYWORD, "parameter type", d);
    return NULL;
  }

  (void)token_next(t);

  // <identifier>

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) != TOKEN_IDENTIFIER) {
    unexpected_token_expected(*t, TOKEN_IDENTIFIER, "parameter name", d);
    return NULL;
  }

  source_range_t identifier_location = (*t)->location;

  char* identifier = (*t)->data.string;

  (void)token_next(t);

  // TODO: Default values?

  source_range_t location = range_cat(&start_location, &identifier_location);

  ast_node_t* parameter = ast_node_create_variable_decl(
      keyword_to_variable_type(keyword_kind), identifier, NULL, false, location,
      identifier_location);

  return parameter;
}
 
