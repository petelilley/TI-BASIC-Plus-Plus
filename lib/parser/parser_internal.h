#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/lexer/token.h>
#include <ti-basic-plus-plus/parser/ast_node.h>

token_kind_t compare_token(const token_t* t, size_t num_kinds, ...);
punctuator_kind_t compare_punctuator(const token_t* t, size_t num_kinds, ...);
keyword_kind_t compare_keyword(const token_t* t, size_t num_kinds, ...);

void unexpected_token(token_t* t, token_kind_t kind, diagnostics_t* d);

ast_node_t* parse_global_decl(token_t** t, diagnostics_t* d);

#endif  // PARSER_INTERNAL_H

