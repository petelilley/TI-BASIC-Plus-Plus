#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include <ti-basic-plus-plus/ast/ast_node.h>
#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/calculator/variable.h>
#include <ti-basic-plus-plus/lexer/token.h>
#include <ti-basic-plus-plus/parser/parser_utils.h>

ast_node_t* parse_global_decl(token_t** t, diagnostics_t* d);
ast_node_t* parse_reserve_decl(token_t** t, diagnostics_t* d);

ast_node_t* parse_function_decl(token_t** t, diagnostics_t* d);
ast_node_t* parse_variable_decl(token_t** t, diagnostics_t* d);

variable_t parse_calculator_variable(token_t** t, diagnostics_t* d);

ast_node_t* parse_expression(token_t** t, diagnostics_t* d);
ast_node_t* parse_function_call(token_t** t, diagnostics_t* d);

ast_node_t* parse_statement_block(token_t** t, diagnostics_t* d);
ast_node_t* parse_if_statement(token_t** t, diagnostics_t* d);
ast_node_t* parse_while_loop(token_t** t, diagnostics_t* d);
ast_node_t* parse_return_statement(token_t** t, diagnostics_t* d);

bool parse_condition_statement(token_t** t,
                               keyword_kind_t keyword,
                               ast_node_t** condition,
                               ast_node_t** body,
                               diagnostics_t* d);

#endif  // PARSER_INTERNAL_H

