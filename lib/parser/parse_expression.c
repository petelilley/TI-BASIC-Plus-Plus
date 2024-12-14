#include "parser_internal.h"

#include <ti-basic-plus-plus/macros.h>
#include <ti-basic-plus-plus/parser/operator.h>

typedef enum expr_fragment_kind {
  EXPR_UNKNOWN,
  EXPR_UNIT,
  EXPR_OPERATOR,
} expr_fragment_kind_t;

typedef struct expr_fragment {
  expr_fragment_kind_t kind;

  union {
    ast_node_t* unit;
    operator_kind_t operator;
  } data;
} expr_fragment_t;

static ast_node_t* compose_expression(expr_fragment_t** fragments);

static ast_node_t* parse_unit(token_t** t, diagnostics_t* d);
static operator_kind_t parse_operator(token_t** t, diagnostics_t* d);

ast_node_t* parse_expression(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  expr_fragment_t* fragments = NULL;
  expr_fragment_kind_t expected_frag = EXPR_UNIT;

  while (true) {
    if (expected_frag == EXPR_UNIT) {
      ast_node_t* unit = parse_unit(t, d);
      if (unit == NULL) {
        goto CLEANUP;
      }

      expr_fragment_t frag = {.kind = EXPR_UNIT, .data.unit = unit};
      arrput(fragments, frag);

      expected_frag = EXPR_OPERATOR;

    } else {  // EXPR_OPERATOR
      operator_kind_t op = parse_operator(t, d);
      if (op == OP_UNKNOWN) {
        break;
      }

      expr_fragment_t frag = {.kind = EXPR_OPERATOR, .data.operator= op };
      arrput(fragments, frag);

      expected_frag = EXPR_UNIT;
    }
  }

  ast_node_t* expr = compose_expression(&fragments);
  arrfree(fragments);
  return expr;

CLEANUP:
  for (size_t i = 0; i < arrlenu(fragments); i += 2) {
    assert(fragments[i].kind == EXPR_UNIT);
    ast_node_destroy(fragments[i].data.unit);
  }
  arrfree(fragments);

  return NULL;
}

static void parse_prefix_operators(token_t** t, operator_kind_t** ops) {
  while (true) {
    if ((*t)->kind == TOKEN_KEYWORD && (*t)->data.keyword == KW_NOT) {
      arrput(*ops, OP_NOT);
      (void)token_next(t);
      continue;
    }
    punctuator_kind_t punct_kind =
        compare_punctuator(*t, 2, '+', '-');
    if (punct_kind == PUNCT_UNKNOWN) {
      break;
    }
    (void)token_next(t);

    operator_kind_t op = OP_POS;
    if (punct_kind == '-') {
      op = OP_NEG;
    }
    arrput(*ops, op);
  }
}

static ast_node_t* parse_unit(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  ast_node_t* unit = NULL;

  operator_kind_t* prefix_ops = NULL;
  /* operator_kind_t* suffix_ops = NULL; // TODO: */

  // Prefix operators.

  parse_prefix_operators(t, &prefix_ops);

  token_kind_t token_kind =
      compare_token(*t, 4, TOKEN_IDENTIFIER, TOKEN_PUNCTUATOR,
                    TOKEN_NUMBER_LITERAL, TOKEN_STRING_LITERAL);

  if (token_kind == TOKEN_UNKNOWN || (token_kind == TOKEN_PUNCTUATOR &&
                                      (*t)->data.punctuator != '(')) {
    unexpected_token_expected(*t, TOKEN_UNKNOWN, "expression", d);
    goto CLEANUP;

  } else if (token_kind == TOKEN_IDENTIFIER) {
    if (compare_punctuator((*t)->next, 1, '(') != PUNCT_UNKNOWN) {
      unit = parse_function_call(t, d);
      if (unit == NULL) {
        goto CLEANUP;
      }
    } else {
      unit = ast_node_create(AST_IDENTIFIER);
      unit->data.identifier = (*t)->data.string;
      unit->location = unit->error_location = (*t)->location;
      (void)token_next(t);
    }

  } else if (token_kind == TOKEN_NUMBER_LITERAL) {
    unit = ast_node_create(AST_NUMERIC_LITERAL);
    unit->data.numeric_literal = (*t)->data.number;
    unit->location = unit->error_location = (*t)->location;
    (void)token_next(t);

  } else if (token_kind == TOKEN_STRING_LITERAL) {
    unit = ast_node_create(AST_STRING_LITERAL);
    unit->data.string_literal = (*t)->data.string;
    unit->location = unit->error_location = (*t)->location;
    (void)token_next(t);

  } else if (token_kind == TOKEN_PUNCTUATOR) {  // Parenthesized expression.
    (void)token_next(t);

    unit = parse_expression(t, d);
    if (unit == NULL) {
      goto CLEANUP;
    }
    if (compare_punctuator(*t, 1, ')') == PUNCT_UNKNOWN) {
      unexpected_token_expected(*t, TOKEN_PUNCTUATOR, "')'", d);
      goto CLEANUP;
    }

    (void)token_next(t);
  } else {
    UNREACHABLE();
  }

  assert(unit != NULL);

  if (prefix_ops != NULL) {
    for (ptrdiff_t i = arrlenu(prefix_ops) - 1; i >= 0; i--) {
      unit = ast_node_create_unary_expr(prefix_ops[i], unit, unit->location);
    }
  }

  return unit;

CLEANUP:
  arrfree(prefix_ops);
  if (unit != NULL) {
    ast_node_destroy(unit);
  }

  return NULL;
}

static operator_kind_t parse_operator(token_t** t, diagnostics_t* d) {
  assert(t != NULL);
  assert(*t != NULL);
  assert(d != NULL);

  keyword_kind_t op_kw = compare_keyword(*t, 2, KW_AND, KW_OR);
  if (op_kw != KW_UNKNOWN) {
    (void)token_next(t);
    return op_from_keyword(op_kw);
  }

  operator_kind_t op;
  if ((*t)->kind != TOKEN_PUNCTUATOR || 
      (op = op_from_punctuator((*t)->data.punctuator), op == OP_UNKNOWN)) {
    return OP_UNKNOWN;
  }

  (void)token_next(t);
  return op;
}

static ast_node_t* compose_expression(expr_fragment_t** fragments) {
  assert(fragments != NULL);
  assert(arrlen(*fragments) != 0);

  size_t num_fragments = arrlenu(*fragments);
  if (num_fragments == 1) {
    expr_fragment_t frag = (*fragments)[0];
    assert(frag.kind == EXPR_UNIT);
    return frag.data.unit;
  }

  int highest_precedence = 0;
  size_t highest_precedence_index = 0;

  for (size_t i = 1; i < num_fragments; i += 2) {
    expr_fragment_t frag = (*fragments)[i];
    assert(frag.kind == EXPR_OPERATOR);
    int precedence = op_get_precedence(frag.data.operator);
    if (precedence > highest_precedence) {
      highest_precedence = precedence;
      highest_precedence_index = i;
    }
  }

  ast_node_t* lhs = (*fragments)[highest_precedence_index - 1].data.unit;
  operator_kind_t op = (*fragments)[highest_precedence_index].data.operator;
  ast_node_t* rhs = (*fragments)[highest_precedence_index + 1].data.unit;

  source_range_t location = range_cat(&lhs->location, &rhs->location);

  ast_node_t* unit = ast_node_create_binary_expr(lhs, op, rhs, location);

  if (num_fragments == 3) {
    return unit;
  }

  (*fragments)[highest_precedence_index - 1].data.unit = unit;
  arrdeln(*fragments, highest_precedence_index, 2);

  return compose_expression(fragments);
}

