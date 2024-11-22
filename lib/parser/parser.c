#include <ti-basic-plus-plus/parser/parser.h>

#include "parser_internal.h"

#include <assert.h>
#include <stdarg.h>

ast_node_t* parse_tokens(token_t* head_token, diagnostics_t* d) {
  assert(head_token != NULL);
  assert(d != NULL);

  ast_node_t* root = ast_node_create(AST_ROOT);
  /* assert(root != NULL); */

  while (head_token != NULL && head_token->kind != TOKEN_EOF &&
         !should_exit(d)) {
    ast_node_t* decl = parse_global_decl(&head_token, d);
    if (decl == NULL) {
      return root;
    }

    arrput(root->children, decl);
  }

  return root;
}

token_kind_t compare_token(const token_t* t, size_t n, ...) {
  assert(n > 0);

  if (t == NULL) {
    return TOKEN_UNKNOWN;
  }

  token_kind_t matched_kind = TOKEN_UNKNOWN;
  
  va_list args;
  va_start(args, n);

  for (size_t i = 0; i < n; i++) {
    token_kind_t kind = va_arg(args, token_kind_t);
    if (t->kind == kind) {
      matched_kind = kind;
      break;
    }
  }

  va_end(args);

  return matched_kind;
}

punctuator_kind_t compare_punctuator(const token_t* t, size_t n, ...) {
  assert(n > 0);

  if (t == NULL || t->kind != TOKEN_PUNCTUATOR) {
    return PUNCT_UNKNOWN;
  }

  punctuator_kind_t matched_kind = PUNCT_UNKNOWN;

  va_list args;
  va_start(args, n);

  for (size_t i = 0; i < n; i++) {
    punctuator_kind_t kind = va_arg(args, punctuator_kind_t);
    if (t->data.punctuator == kind) {
      matched_kind = kind;
      break;
    }
  }

  va_end(args);

  return matched_kind;
}

keyword_kind_t compare_keyword(const token_t* t, size_t n, ...) {
  assert(n > 0);

  if (t == NULL || t->kind != TOKEN_KEYWORD) {
    return KW_UNKNOWN;
  }

  keyword_kind_t matched_kind = KW_UNKNOWN;

  va_list args;
  va_start(args, n);

  for (size_t i = 0; i < n; i++) {
    keyword_kind_t kind = va_arg(args, keyword_kind_t);
    if (t->data.keyword == kind) {
      matched_kind = kind;
      break;
    }
  }

  va_end(args);

  return matched_kind;
}

void unexpected_token(token_t* t, token_kind_t kind, diagnostics_t* d) {
  assert(t != NULL);
  assert(d != NULL);

  if (t->kind != kind || kind == TOKEN_UNKNOWN) {
    diag_report_source(d, &t->location, FATAL_ERROR_UNEXPECTED_TOKEN,
                       token_kind_to_string(t->kind));
  } else if (t->kind == TOKEN_PUNCTUATOR) {
    diag_report_source(d, &t->location, FATAL_ERROR_UNEXPECTED_PUNCTUATOR,
                       punct_to_string(t->data.punctuator));
  } else if (t->kind == TOKEN_KEYWORD) {
    diag_report_source(d, &t->location, FATAL_ERROR_UNEXPECTED_KEYWORD,
                       keyword_to_string(t->data.keyword));
  } else {
    diag_report_source(d, &t->location, FATAL_ERROR_UNEXPECTED_TOKEN, "token");
  }
}

