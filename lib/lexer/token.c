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

static void emit_token_unknown(token_t* token, FILE* stream);
static void emit_token_identifier(token_t* token, FILE* stream);
static void emit_token_keyword(token_t* token, FILE* stream);
static void emit_token_punctuator(token_t* token, FILE* stream);
static void emit_token_number_literal(token_t* token, FILE* stream);
static void emit_token_string_literal(token_t* token, FILE* stream);
static void emit_token_newline(token_t* token, FILE* stream);
static void emit_token_eof(token_t* token, FILE* stream);

void emit_token(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);

  void (*emit_function)(token_t*, FILE*) = NULL;

  switch (token->kind) {
    case TOKEN_UNKNOWN:
      emit_function = emit_token_unknown;
      break;
    case TOKEN_IDENTIFIER:
      emit_function = emit_token_identifier;
      break;
    case TOKEN_KEYWORD:
      emit_function = emit_token_keyword;
      break;
    case TOKEN_PUNCTUATOR:
      emit_function = emit_token_punctuator;
      break;
    case TOKEN_NUMBER_LITERAL:
      emit_function = emit_token_number_literal;
      break;
    case TOKEN_STRING_LITERAL:
      emit_function = emit_token_string_literal;
      break;
    case TOKEN_NEWLINE:
      emit_function = emit_token_newline;
      break;
    case TOKEN_EOF:
      emit_function = emit_token_eof;
      break;
    default:
      assert(false);
      break;
  }

  if (emit_function != NULL) {
    emit_function(token, stream);
  }
}

void emit_token_list(token_t* head, FILE* stream) {
  assert(head != NULL);
  assert(stream != NULL);

  while (head != NULL) {
    emit_token(head, stream);
    head = head->next;
  }
}

static void emit_token_unknown(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_UNKNOWN);

  (void)fputs("Token: Unknown\n", stream);
  emit_range(&token->location, 1, stream);
}

static void emit_token_identifier(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_IDENTIFIER);

  (void)fputs("Token: Identifier\n", stream);
  emit_range(&token->location, 1, stream);

  (void)fprintf(stream, "\tText: '%.*s'\n", (int)arrlen(token->data.string),
                token->data.string);
}

static void emit_token_keyword(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_KEYWORD);

  (void)fputs("Token: Keyword\n", stream);
  emit_range(&token->location, 1, stream);

  (void)fprintf(stream, "\tValue: '%s'\n",
                keyword_to_string(token->data.keyword));
}

static void emit_token_punctuator(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_PUNCTUATOR);

  (void)fputs("Token: Punctuator\n", stream);
  emit_range(&token->location, 1, stream);

  (void)fprintf(stream, "\tValue: '%s'\n",
                punct_to_string(token->data.punctuator));
}

static void emit_token_number_literal(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_NUMBER_LITERAL);

  (void)fputs("Token: Number Literal\n", stream);
  emit_range(&token->location, 1, stream);

  (void)fprintf(stream, "\tValue: '%f'\n", token->data.number);
}

static void emit_token_string_literal(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_STRING_LITERAL);

  (void)fputs("Token: String Literal\n", stream);
  emit_range(&token->location, 1, stream);

  (void)fprintf(stream, "\tText: '%.*s'\n", (int)arrlen(token->data.string),
                token->data.string);
}

static void emit_token_newline(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_NEWLINE);

  (void)fputs("Token: Newline\n", stream);
  emit_range(&token->location, 1, stream);
}

static void emit_token_eof(token_t* token, FILE* stream) {
  assert(token != NULL);
  assert(stream != NULL);
  assert(token->kind == TOKEN_EOF);

  (void)fputs("Token: EOF\n", stream);
  emit_range(&token->location, 1, stream);
}
 
