#include "parser_internal.h"

ast_node_t* parse_statement_block(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  ast_node_t** statements = NULL;
  source_range_t start_location = (*t)->location;

  // {

  if (compare_punctuator(*t, 1, PUNCT_LBRACE) != PUNCT_LBRACE) {
    unexpected_token(*t, TOKEN_PUNCTUATOR, d);
    return NULL;
  }

  (void)token_next(t);

  while (!is_fatal(d)) {
    token_kind_t token = (*t)->kind;

    source_range_t* location = &(*t)->location;
    ast_node_t* statement = NULL;

    if (token == TOKEN_NEWLINE) {
      (void)token_next(t);
      continue;
    }
    else if (token == TOKEN_KEYWORD) {
      switch ((*t)->data.keyword) {
        case KW_NUMBER:
        case KW_STRING:
        case KW_MATRIX:
        case KW_LIST:
        case KW_VOID:
          statement = parse_variable_decl(t, d);
          if (statement == NULL) {
            goto CLEANUP;
          }
          break;
        case KW_IF:
          // TODO: If statement
          break;
        case KW_ELIF:
          diag_report_source(d, location, FATAL_ERROR_ELIF_WITHOUT_IF);
          goto CLEANUP;
        case KW_ELSE:
          diag_report_source(d, location, FATAL_ERROR_ELSE_WITHOUT_IF);
          goto CLEANUP;
        case KW_WHILE:
          // TODO: While loop
          break;
        case KW_FOR:
          // TODO: For loop
          break;
        case KW_RETURN:
          // TODO: Return statement
          break;
        case KW_RESERVE:
          diag_report_source(d, location, ERROR_RESERVE_NOT_GLOBAL);
          statement = parse_reserve_decl(t, d);
          if (statement == NULL) {
            goto CLEANUP;
          }
          break;
        case KW_NOT:
          // TODO: Expression
          break;
        default:
          unexpected_token(*t, TOKEN_KEYWORD, d);
          goto CLEANUP;
      }
    }
    else if (token == TOKEN_PUNCTUATOR) {
      punctuator_kind_t punct = (*t)->data.punctuator;

      if (punct == PUNCT_RBRACE) {
        break;
      }
      else if (punct == PUNCT_LBRACE) {
        statement = parse_statement_block(t, d);
      }
      else {
        unexpected_token(*t, TOKEN_PUNCTUATOR, d);
        goto CLEANUP;
      }
    }
    else {
      unexpected_token_expected(*t, TOKEN_UNKNOWN, "statement or '}'", d);
      goto CLEANUP;
    }

    if (statement != NULL) {
      arrput(statements, statement);
      statement = NULL;
    }
  }

  if (is_fatal(d)) {
    goto CLEANUP;
  }

  // }

  if (compare_punctuator(*t, 1, PUNCT_RBRACE) != PUNCT_RBRACE) {
    unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "'}'", d);
    return NULL;
  }

  source_range_t end_location = (*t)->location;

  (void)token_next(t);

  // Newline

  if (compare_token(*t, 1, TOKEN_NEWLINE) != TOKEN_NEWLINE) {
    unexpected_token_expected(*t, TOKEN_NEWLINE, "newline", d);
    goto CLEANUP;
  }

  (void)token_next(t);

  if (!arrlen(statements)) {
    return NULL;
  }

  source_range_t location = range_cat(&start_location, &end_location);
  ast_node_t* block = ast_node_create(AST_STATEMENT_BLOCK);
  block->location = location;
  block->children = statements;

  return block;

CLEANUP:
  // TODO: Cleanup
  return NULL;

}
