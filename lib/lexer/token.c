#include <ti-basic-plus-plus/lexer/token.h>

#include <assert.h>
#include <stb_ds.h>
#include <stdlib.h>
#include <string.h>

const char* token_kind_to_string(token_kind_t kind) {
  switch (kind) {
    case TOKEN_UNKNOWN:
      return "unknown";
    case TOKEN_IDENTIFIER:
      return "identifier";
    case TOKEN_KEYWORD:
      return "keyword";
    case TOKEN_PUNCTUATOR:
      return "punctuator";
    case TOKEN_NUMBER_LITERAL:
      return "numeric literal";
    case TOKEN_STRING_LITERAL:
      return "string literal";
    case TOKEN_NEWLINE:
      return "newline";
    case TOKEN_EOF:
      return "EOF";
    default:
      assert(false);
  }
  return NULL;
}

token_t* token_create(token_kind_t kind, source_range_t location) {
  assert(kind != TOKEN_UNKNOWN);

  token_t* token = malloc(sizeof(token_t));
  if (token == NULL) {
    assert(false);
    return NULL;
  }

  (void)memset(token, 0, sizeof(token_t));

  token->kind = kind;
  token->location = location;

  return token;
}

static void token_destroy(token_t* token) {
  if (token->kind == TOKEN_STRING_LITERAL || token->kind == TOKEN_IDENTIFIER) {
    arrfree(token->data.string);
  }

  free(token);
}

void token_list_destroy(token_t* head) {
  assert(head != NULL);

  token_t* current = head;
  while (current != NULL) {
    head = current;
    current = current->next;

    token_destroy(head);
  }
}

