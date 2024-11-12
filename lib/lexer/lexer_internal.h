#ifndef LEXER_INTERNAL_H
#define LEXER_INTERNAL_H

#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/basic/input_file.h>
#include <ti-basic-plus-plus/lexer/token.h>

token_t* next_token(input_file_iterator_t* it, diagnostics_t* d);

void tokenize_line_comment(input_file_iterator_t* it);
void tokenize_block_comment(input_file_iterator_t* it, diagnostics_t* d);
token_t* tokenize_numeric_literal(input_file_iterator_t* it, diagnostics_t* d);
token_t* tokenize_string_literal(input_file_iterator_t* it, diagnostics_t* d);
token_t* tokenize_identifier(input_file_iterator_t* it);
token_t* tokenize_punctuator(input_file_iterator_t* it);

#endif  // LEXER_INTERNAL_H
