#include "parser_internal.h"

static variable_t parse_var_matrix(token_t** t, diagnostics_t* d);
static variable_t parse_var_number(token_t** t, diagnostics_t* d);
static variable_t parse_var_theta(token_t** t, diagnostics_t* d);
static variable_t parse_var_string(token_t** t, diagnostics_t* d);
static variable_t parse_var_list(token_t** t, diagnostics_t* d);

variable_t parse_calculator_variable(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  // A-Z, Theta, Str0-Str9, L1-L6, L1-L6(1-999), [A]-[J], [A]-[J](1-99,1-99)

  token_kind_t token_kind =
      compare_token(*t, 2, TOKEN_IDENTIFIER, TOKEN_PUNCTUATOR);
  if (token_kind == TOKEN_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_UNKNOWN, "calculator variable name", d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  if (token_kind == TOKEN_PUNCTUATOR) {  // [A]-[J]
    return parse_var_matrix(t, d);
  }

  size_t id_length = arrlenu((*t)->data.string);
  switch (id_length) {
    case 1:  // A-Z
      return parse_var_number(t, d);
    case 5:  // Theta
      return parse_var_theta(t, d);
    case 2:  // L1-L6
      return parse_var_list(t, d);
    case 4:  // Str0-Str9
      return parse_var_string(t, d);
  }

  diag_report_source(d, &(*t)->location, ERROR_INVALID_VARIABLE_NAME, id_length,
                     (*t)->data.string);

  (void)token_next(t);

  return (variable_t){.type = VAR_UNKNOWN};
}

static variable_t parse_var_matrix(token_t** t, diagnostics_t* d) {
  // [

  if (compare_punctuator(*t, 1, PUNCT_LBRACKET) != PUNCT_LBRACKET) {
    unexpected_token(*t, TOKEN_PUNCTUATOR, d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  // A-J

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_IDENTIFIER, "identifier A-J", d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  char* id = (*t)->data.string;
  size_t id_length = arrlenu(id);

  char letter;

  bool failed = false;
  if (id_length != 1 || (letter = id[0], (letter < 'A' || letter > 'J'))) {
    diag_report_source(d, &(*t)->location, ERROR_INVALID_MATRIX_VARIABLE_NAME,
                       id_length, id);
    failed = true;
  }

  (void)token_next(t);

  // ]

  if (compare_punctuator(*t, 1, PUNCT_RBRACKET) != PUNCT_RBRACKET) {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "closing ]", d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  if (failed) {
    return (variable_t){.type = VAR_UNKNOWN};
  }

  return (variable_t){.type = VAR_MATRIX, .id.letter = letter};
}

static variable_t parse_var_number(token_t** t, diagnostics_t* d) {
  // A-Z

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_IDENTIFIER, "A-Z, or Theta", d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  char* id_string = (*t)->data.string;
  size_t id_length = arrlenu(id_string);

  char letter;

  if (id_length != 1 || (letter = id_string[0], letter < 'A' || letter > 'Z')) {
    diag_report_source(d, &(*t)->location, ERROR_INVALID_VARIABLE_NAME,
                       id_length, id_string);
    (void)token_next(t);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  return (variable_t){.type = VAR_NUMBER, .id.letter = letter};
}

static variable_t parse_var_theta(token_t** t, diagnostics_t* d) {
  // Theta

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token_expected(*t, TOKEN_IDENTIFIER, "A-Z, or Theta", d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  char* id_string = (*t)->data.string;
  size_t id_length = arrlenu(id_string);

  if (id_length != 5 || strncmp(id_string, "Theta", 5) != 0) {
    diag_report_source(d, &(*t)->location, ERROR_INVALID_VARIABLE_NAME,
                       id_length, id_string);
    (void)token_next(t);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  return (variable_t){.type = VAR_NUMBER, .id.letter = 'Z' + 1};
}

static variable_t parse_var_string(token_t** t, diagnostics_t* d) {
  // Str0-Str9

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token(*t, TOKEN_IDENTIFIER, d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  char* id_string = (*t)->data.string;
  size_t id_length = arrlenu(id_string);

  char id;

  if (id_length != 4 || memcmp(id_string, "Str", 3) != 0 ||
      (id = id_string[3], id < '0' || id > '9')) {
    diag_report_source(d, &(*t)->location, ERROR_INVALID_VARIABLE_NAME,
                       id_length, id_string);
    (void)token_next(t);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  return (variable_t){.type = VAR_STRING, .id.index = id - '0'};
}

static variable_t parse_var_list(token_t** t, diagnostics_t* d) {
  // L1-L6

  if (compare_token(*t, 1, TOKEN_IDENTIFIER) == TOKEN_UNKNOWN) {
    unexpected_token(*t, TOKEN_IDENTIFIER, d);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  char* id_string = (*t)->data.string;
  size_t id_length = arrlenu(id_string);

  char id;

  if (id_length != 2 || id_string[0] != 'L' ||
      (id = id_string[1], id < '1' || id > '6')) {
    diag_report_source(d, &(*t)->location, ERROR_INVALID_VARIABLE_NAME,
                       id_length, id_string);
    (void)token_next(t);
    return (variable_t){.type = VAR_UNKNOWN};
  }

  (void)token_next(t);

  return (variable_t){.type = VAR_LIST, .id.index = id - '0'};
}

