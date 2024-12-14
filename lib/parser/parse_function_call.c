#include "parser_internal.h"

#include <ti-basic-plus-plus/macros.h>

static ast_node_t** parse_argument_list(token_t** t, diagnostics_t* d);

ast_node_t* parse_function_call(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  // <identifier>

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token(*t, TOKEN_IDENTIFIER, d);
    return NULL;
  }

  source_range_t identifier_location = (*t)->location;

  char* identifier = (*t)->data.string;

  (void)token_next(t);

  // (

  if (compare_punctuator(*t, 1, '(') == PUNCT_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "'('", d);
    return NULL;
  }

  (void)token_next(t);

  // <arguments>

  ast_node_t** arguments = parse_argument_list(t, d);
  if (should_exit(d)) {
    return NULL;
  }

  // )

  if (compare_punctuator(*t, 1, ')') != ')') {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "')'", d);
    return NULL;
  }

  source_range_t end_location = (*t)->location;

  (void)token_next(t);

  source_range_t location = range_cat(&start_location, &end_location);

  ast_node_t* function_call = ast_node_create_function_call(
      identifier, arguments, location, identifier_location);

  return function_call;
}

static ast_node_t** parse_argument_list(token_t** t, diagnostics_t* d) {
  ast_node_t** arguments = NULL;

  punctuator_kind_t punct = compare_punctuator(*t, 1, ')');

  while (punct != ')') {
    if (punct == ',') {
      (void)token_next(t);
    }

    ast_node_t* argument = parse_expression(t, d);
    if (argument == NULL) {
      return NULL;
    }

    arrput(arguments, argument);

    punct = compare_punctuator(*t, 2, ',', ')');
    if (punct == PUNCT_UNKNOWN) {
      unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "',' or ')'", d);
      goto CLEANUP;
    }
  }

  return arguments;

CLEANUP:
  for (size_t num = arrlenu(arguments), i = 0; i < num; ++i) {
    ast_node_destroy(arguments[i]);
  }
  arrfree(arguments);

  return NULL;
}

