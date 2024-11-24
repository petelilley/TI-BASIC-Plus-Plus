#include "parser_internal.h"

#include <ti-basic-plus-plus/macros.h>

static ast_node_t* parse_variable_or_function_decl(token_t** t,
                                                   diagnostics_t* d);

static ast_node_t* parse_reserve_decl(token_t** t, diagnostics_t* d);

ast_node_t* parse_global_decl(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  while (*t != NULL) {
    token_kind_t token_kind =
        compare_token(*t, 3, TOKEN_NEWLINE, TOKEN_KEYWORD, TOKEN_EOF);

    if (token_kind == TOKEN_UNKNOWN) {
      unexpected_token(*t, TOKEN_UNKNOWN, d);
      return NULL;
    } else if (token_kind == TOKEN_NEWLINE) {
      (void)token_next(t);
      continue;
    } else if (token_kind == TOKEN_EOF) {
      return NULL;
    } else if (token_kind != TOKEN_KEYWORD) {
      UNREACHABLE();
    }

    keyword_kind_t keyword_kind = compare_keyword(
        *t, 6, KW_VOID, KW_NUMBER, KW_STRING, KW_MATRIX, KW_LIST, KW_RESERVE);
    if (keyword_kind == KW_UNKNOWN) {
      unexpected_token(*t, TOKEN_KEYWORD, d);
      return NULL;
    }

    switch (keyword_kind) {
      // Types, could be a variable decl or a function decl
      case KW_VOID:
      case KW_NUMBER:
      case KW_STRING:
      case KW_MATRIX:
      case KW_LIST:
        return parse_variable_or_function_decl(t, d);
      case KW_RESERVE:
        return parse_reserve_decl(t, d);
      default:
        UNREACHABLE();
    }
  }

  return NULL;
}

static ast_node_t* parse_variable_or_function_decl(token_t** start,
                                                   diagnostics_t* d) {
  assert(start != NULL);
  assert(*start != NULL);
  assert(d != NULL);

  token_t** t = start;

  // <type>

  (void)token_next(t);

  // <identifier>

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) != TOKEN_IDENTIFIER) {
    unexpected_token(*t, TOKEN_IDENTIFIER, d);
    return NULL;
  }

  (void)token_next(t);

  // (, :, =

  punctuator_kind_t punct =
      compare_punctuator(*t, 3, PUNCT_LPAREN, PUNCT_COLON, PUNCT_ASGN);
  if (punct == PUNCT_UNKNOWN) {
    unexpected_token(*t, TOKEN_PUNCTUATOR, d);
    return NULL;
  }

  if (punct == PUNCT_LPAREN) {
    return parse_function_decl(start, d);
  }

  return parse_variable_decl(start, d);
}

static ast_node_t* parse_reserve_decl(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  source_range_t start_location = (*t)->location;

  // reserve

  if (compare_keyword(*t, 1, KW_RESERVE) != KW_RESERVE) {
    unexpected_token(*t, TOKEN_KEYWORD, d);
    return NULL;
  }

  (void)token_next(t);

  source_range_t variable_start_location = (*t)->location;

  // <variable>

  variable_t variable = parse_calculator_variable(t, d);
  if (variable.type == VAR_UNKNOWN) {
    return NULL;
  }

  source_range_t end_location = (*t)->prev->location;

  if (compare_token(*t, 1, TOKEN_NEWLINE) != TOKEN_NEWLINE) {
    unexpected_token_expected(*t, TOKEN_NEWLINE, "newline", d);
    return NULL;
  }

  (void)token_next(t);

  source_range_t location = range_cat(&start_location, &end_location),
                 variable_location =
                     range_cat(&variable_start_location, &end_location);

  return ast_node_create_variable_reservation(variable, location,
                                              variable_location);
}

