#ifndef AST_NODE_H
#define AST_NODE_H

#include <stb_ds.h>
#include <stdio.h>
#include <ti-basic-plus-plus/basic/source_location.h>
#include <ti-basic-plus-plus/calculator/builtin_constant.h>
#include <ti-basic-plus-plus/calculator/builtin_function.h>
#include <ti-basic-plus-plus/calculator/variable.h>
#include <ti-basic-plus-plus/parser/operator.h>

typedef enum {
  AST_ROOT,
  AST_FUNCTION_DECL,
  AST_VARIABLE_DECL,
  AST_STATEMENT,
  AST_BLOCK,
  AST_VARIABLE_RESERVATION,
  AST_PARAMETER,

  AST_BINARY_EXPRESSION,
  AST_UNARY_EXPRESSION,
  AST_IDENTIFIER,
  AST_NUMERIC_LITERAL,
  AST_STRING_LITERAL,

  AST_FUNCTION_CALL,

  AST_IF_STATEMENT,
  AST_WHILE_STATEMENT,
  /* AST_FOR_STATEMENT, */
  AST_RETURN_STATEMENT,
} ast_node_kind_t;

typedef struct ast_variable_decl_data {
  variable_type_t type;
  const char* name;
  struct ast_node* expr;
  bool is_constant;
} ast_variable_decl_data_t;

typedef struct ast_variable_decl_metadata {
  variable_t variable;
} ast_variable_decl_metadata_t;

typedef struct ast_function_decl_data {
  variable_type_t return_type;
  const char* name;
  struct ast_node** parameters;
  struct ast_node* body;
} ast_function_decl_data_t;

typedef struct ast_function_call_data {
  const char* name;
  struct ast_node** arguments;
} ast_function_call_data_t;

typedef struct ast_function_call_metadata {
  bool is_builtin;
  builtin_function_t builtin_function;
  struct ast_node* function_decl;
} ast_function_call_metadata_t;

typedef struct ast_identifier_metadata {
  bool is_builtin;
  builtin_constant_t builtin_constant;
  struct ast_node* variable_decl;
} ast_identifier_metadata_t;

typedef struct ast_node {
  ast_node_kind_t kind;

  source_range_t location;
  source_range_t error_location;
  bool has_error_location;

  struct ast_node** children;

  union {
    variable_t reserved_variable;
    ast_variable_decl_data_t variable_decl;
    ast_function_decl_data_t function_decl;
    const char* identifier;
    double numeric_literal;
    const char* string_literal;
    operator_kind_t operator;
    ast_function_call_data_t function_call;
  } data;

  union {
    ast_variable_decl_metadata_t variable_decl;
    ast_function_call_metadata_t function_call;
    ast_identifier_metadata_t identifier;
  } metadata;
} ast_node_t;

ast_node_t* ast_node_create(ast_node_kind_t kind);
void ast_node_destroy(ast_node_t* node);

ast_node_t* ast_node_create_variable_reservation(variable_t variable,
                                                 source_range_t location,
                                                 source_range_t error_location);

ast_node_t* ast_node_create_variable_decl(variable_type_t type,
                                          const char* name,
                                          ast_node_t* expr,
                                          bool is_constant,
                                          source_range_t location,
                                          source_range_t error_location);

ast_node_t* ast_node_create_function_decl(variable_type_t return_type,
                                          const char* name,
                                          ast_node_t** parameters,
                                          ast_node_t* body,
                                          source_range_t location,
                                          source_range_t error_location);

void emit_ast(ast_node_t* root, FILE* stream);

#endif  // AST_NODE_H

