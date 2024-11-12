#include <ti-basic-plus-plus/lexer/lexer.h>

#include <ctype.h>

#include "lexer_internal.h"

token_t* tokenize_file(input_file_t* input_file, diagnostics_t* d) {
  assert(input_file != NULL);
  assert(d != NULL);

  input_file_iterator_t it = if_iterator_create(input_file);

  token_t* head = NULL;
  token_t* tail = NULL;

  token_t* token;

  while ((token = next_token(&it, d)) != NULL) {
    if (head == NULL) {
      head = token;
      tail = token;
    } else {
      tail->next = token;
      token->prev = tail;
      tail = token;
    }

    if (token->kind == TOKEN_EOF) {
      break;
    }

    if (has_errors(d)) {
      return head;
    }
  }

  return head;
}

token_t* next_token(input_file_iterator_t* it, diagnostics_t* d) {
  assert(it != NULL);
  assert(d != NULL);

  token_t* token;

  int c;
  while ((c = if_current(it)) != EOF) {
    int next_c = if_peek_next(it);

    if (c == '\n') {
      token = token_create(TOKEN_NEWLINE, range_at_current(it));
      (void)if_next(it);
      return token;
    }

    if (isspace(c)) {
      (void)if_next(it);
      continue;
    }

    if (c == '/' && next_c == '/') {
      tokenize_line_comment(it);
      continue;
    }

    if (c == '/' && next_c == '*') {
      tokenize_block_comment(it, d);
      continue;
    }

    if (isdigit(c) || (c == '.' && isdigit(next_c))) {
      return tokenize_numeric_literal(it, d);
    }

    if (c == '"' || c == '\'') {
      return tokenize_string_literal(it, d);
    }

    if (isalpha(c) || c == '_') {
      return tokenize_identifier(it);
    }

    token = tokenize_punctuator(it);
    if (token != NULL) {
      return token;
    }

    source_range_t range = range_at(it->file, if_get_position(it));
    diag_report_source(d, &range, FATAL_ERROR_TOKEN_INVALID, (char)c, c);
    return NULL;
  }

  return token_create(TOKEN_EOF, range_at_current(it));
}
