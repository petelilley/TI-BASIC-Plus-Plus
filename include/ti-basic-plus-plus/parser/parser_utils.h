#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/lexer/token.h>
#include <ti-basic-plus-plus/ast/ast_node.h>

token_kind_t compare_token(const token_t* t, size_t num_kinds, ...);
punctuator_kind_t compare_punctuator(const token_t* t, size_t num_kinds, ...);
keyword_kind_t compare_keyword(const token_t* t, size_t num_kinds, ...);

void unexpected_token(token_t* t, token_kind_t kind, diagnostics_t* d);
void unexpected_token_expected(token_t* t,
                               token_kind_t kind,
                               const char* expected,
                               diagnostics_t* d);

// Iterates to the next token and returns it
static inline token_t* token_next(token_t** t) {
  assert(t != NULL);
  assert(*t != NULL);
  assert((*t)->next != NULL);

  *t = (*t)->next;
  return *t;
}

static inline token_t* token_prev(token_t** t) {
  assert(t != NULL);
  assert(*t != NULL);
  assert((*t)->prev != NULL);

  *t = (*t)->prev;
  return *t;
}

#endif  // PARSER_UTILS_H

