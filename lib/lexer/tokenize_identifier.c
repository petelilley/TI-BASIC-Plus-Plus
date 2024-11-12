#include "lexer_internal.h"

#include <ctype.h>
#include <stb_ds.h>

token_t* tokenize_identifier(input_file_iterator_t* it) {
  assert(it != NULL);

  source_position_t start_position = if_get_position(it);
  source_position_t end_position = start_position;

  char* text = NULL;
  int c = if_current(it);
  while (isalnum(c) || c == '_') {
    arrput(text, c);
    end_position = if_get_position(it);
    c = if_next(it);
  }

  source_range_t range = range_create(it->file, start_position, end_position);

  token_t* token = NULL;

  keyword_kind_t keyword_kind = keyword_match(text, arrlen(text));
  if (keyword_kind == KW_UNKNOWN) {
    token = token_create(TOKEN_IDENTIFIER, range);
    token->data.string = text;
  } else {
    token = token_create(TOKEN_KEYWORD, range);
    token->data.keyword = keyword_kind;
    arrfree(text);
  }

  return token;
}

