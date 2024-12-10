#ifndef PARSER_H
#define PARSER_H

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/lexer/token.h>
#include <ti-basic-plus-plus/ast/ast_node.h>

ast_node_t* parse_tokens(token_t* head_token, diagnostics_t* d);

#endif // PARSER_H
