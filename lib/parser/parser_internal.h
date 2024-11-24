#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/calculator/variable.h>
#include <ti-basic-plus-plus/lexer/token.h>
#include <ti-basic-plus-plus/parser/ast_node.h>
#include <ti-basic-plus-plus/parser/parser_utils.h>

ast_node_t* parse_global_decl(token_t** t, diagnostics_t* d);

ast_node_t* parse_function_decl(token_t** t, diagnostics_t* d);
ast_node_t* parse_variable_decl(token_t** t, diagnostics_t* d);

variable_t parse_calculator_variable(token_t** t, diagnostics_t* d);

#endif  // PARSER_INTERNAL_H

