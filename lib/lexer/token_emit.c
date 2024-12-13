#include <ti-basic-plus-plus/lexer/token.h>

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/basic/input_file.h>
#include <ti-basic-plus-plus/utils/emit_tree_utils.h>

#include <stb_ds.h>

static void build_token(token_t* token,
                        size_t indent_size,
                        emit_tree_indent_data_t* indent,
                        diagnostics_t* d);

static emit_tree_section_build_func_t build_elements_token_list;
static emit_tree_section_build_func_t build_children_token_list;

static emit_tree_section_build_func_t build_elements_token_unknown;
static emit_tree_section_build_func_t build_elements_token_identifier;
static emit_tree_section_build_func_t build_elements_token_keyword;
static emit_tree_section_build_func_t build_elements_token_punctuator;
static emit_tree_section_build_func_t build_elements_token_number_literal;
static emit_tree_section_build_func_t build_elements_token_string_literal;
static emit_tree_section_build_func_t build_elements_token_newline;
static emit_tree_section_build_func_t build_elements_token_eof;

void token_list_emit(token_t* head, diagnostics_t* d) {
  emit_tree_section(d, 0, NULL, "tokens", 1, true, build_elements_token_list,
                    build_children_token_list, head);
}

static void build_elements_token_list(void* _head,
                                      size_t indent_size,
                                      emit_tree_indent_data_t* indent,
                                      diagnostics_t* d) {
  assert(_head != NULL);
  assert(d != NULL);

  token_t* head = (token_t*)_head;

  const char* file_path = head->location.file->path;
  emit_tree_element_fmt(d, indent_size, indent, "file_path", "%s", file_path);
}

static void build_children_token_list(void* _head,
                                      size_t indent_size,
                                      emit_tree_indent_data_t* indent,
                                      diagnostics_t* d) {
  assert(_head != NULL);
  assert(d != NULL);

  token_t* head = (token_t*)_head;
  (void)head;
  (void)indent_size;
  (void)indent;

  for (token_t* t = head; t != NULL; t = t->next) {
    build_token(t, indent_size, indent, d);
  }
}

static void build_token(token_t* token,
                        size_t indent_size,
                        emit_tree_indent_data_t* indent,
                        diagnostics_t* d) {
  assert(token != NULL);
  assert(d != NULL);

  const char* tag = NULL;
  emit_tree_section_build_func_t* build_func = NULL;
  uint8_t color = 1;

  switch (token->kind) {
    case TOKEN_UNKNOWN:
      tag = "unknown";
      build_func = build_elements_token_unknown;
      color = 31;  // red
      break;
    case TOKEN_IDENTIFIER:
      tag = "identifier";
      build_func = build_elements_token_identifier;
      color = 33;  // yellow
      break;
    case TOKEN_KEYWORD:
      tag = "keyword";
      build_func = build_elements_token_keyword;
      color = 32;  // green
      break;
    case TOKEN_PUNCTUATOR:
      tag = "punctuator";
      build_func = build_elements_token_punctuator;
      color = 35;  // magenta
      break;
    case TOKEN_NUMBER_LITERAL:
      tag = "number_literal";
      build_func = build_elements_token_number_literal;
      color = 36;  // cyan
      break;
    case TOKEN_STRING_LITERAL:
      tag = "string_literal";
      build_func = build_elements_token_string_literal;
      color = 36;  // cyan
      break;
    case TOKEN_NEWLINE:
      tag = "newline";
      build_func = build_elements_token_newline;
      color = 1;
      break;
    case TOKEN_EOF:
      tag = "eof";
      build_func = build_elements_token_eof;
      color = 1;
      break;
  }

  if (tag != NULL) {
    emit_tree_section(d, indent_size, indent, tag, color, token->next == NULL,
                      build_func, NULL, token);
  }
}

void build_elements_token_unknown(void* _token,
                                  size_t indent_size,
                                  emit_tree_indent_data_t* indent,
                                  diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_UNKNOWN);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element(d, indent_size, indent, "unknown", NULL, NULL);
}

void build_elements_token_identifier(void* _token,
                                     size_t indent_size,
                                     emit_tree_indent_data_t* indent,
                                     diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_IDENTIFIER);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "'%.*s'",
                        (int)arrlen(token->data.string), token->data.string);
}

void build_elements_token_keyword(void* _token,
                                  size_t indent_size,
                                  emit_tree_indent_data_t* indent,
                                  diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_KEYWORD);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "'%s'",
                        keyword_to_string(token->data.keyword));
}

void build_elements_token_punctuator(void* _token,
                                     size_t indent_size,
                                     emit_tree_indent_data_t* indent,
                                     diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_PUNCTUATOR);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "'%s'",
                        punct_to_string(token->data.punctuator));
}

void build_elements_token_number_literal(void* _token,
                                         size_t indent_size,
                                         emit_tree_indent_data_t* indent,
                                         diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_NUMBER_LITERAL);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "'%f'",
                        token->data.number);
}

void build_elements_token_string_literal(void* _token,
                                         size_t indent_size,
                                         emit_tree_indent_data_t* indent,
                                         diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_STRING_LITERAL);

  range_emit(&token->location, indent_size, indent, d);

  emit_tree_element_fmt(d, indent_size, indent, "value", "\"%.*s\"",
                        (int)arrlen(token->data.string), token->data.string);
}

void build_elements_token_newline(void* _token,
                                  size_t indent_size,
                                  emit_tree_indent_data_t* indent,
                                  diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_NEWLINE);

  range_emit(&token->location, indent_size, indent, d);
}

void build_elements_token_eof(void* _token,
                              size_t indent_size,
                              emit_tree_indent_data_t* indent,
                              diagnostics_t* d) {
  assert(_token != NULL);
  assert(d != NULL);

  token_t* token = (token_t*)_token;

  assert(token->kind == TOKEN_EOF);

  range_emit(&token->location, indent_size, indent, d);
}

