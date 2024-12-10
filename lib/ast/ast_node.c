#include <ti-basic-plus-plus/ast/ast_node.h>

#include <ti-basic-plus-plus/macros.h>

ast_node_t* ast_node_create(ast_node_kind_t kind) {
  ast_node_t* node = (ast_node_t*)malloc(sizeof(ast_node_t));
  assert(node != NULL);

  (void)memset(node, 0, sizeof(ast_node_t));
  node->kind = kind;

  return node;
}

void ast_node_destroy(ast_node_t* node) {
  assert(node != NULL);

  size_t num_children = arrlenu(node->children);
  for (size_t i = 0; i < num_children; i++) {
    ast_node_destroy(node->children[i]);
  }

  arrfree(node->children);
  free(node);
}

ast_node_t* ast_node_create_root(input_file_t* input_file) {
  assert(input_file != NULL);

  ast_node_t* node = ast_node_create(AST_ROOT);
  node->location.file = input_file;

  return node;
}

ast_node_t* ast_node_create_variable_reservation(
    variable_t variable,
    source_range_t location,
    source_range_t error_location) {
  ast_node_t* node = ast_node_create(AST_VARIABLE_RESERVATION);
  node->location = location;
  node->error_location = error_location;
  node->data.reserved_variable = variable;

  return node;
}

ast_node_t* ast_node_create_variable_decl(variable_type_t type,
                                          const char* name,
                                          ast_node_t* expr,
                                          bool is_constant,
                                          source_range_t location,
                                          source_range_t error_location) {
  assert(type != VAR_UNKNOWN);
  assert(name != NULL);

  ast_node_t* node = ast_node_create(AST_VARIABLE_DECL);
  node->location = location;
  node->error_location = error_location;
  node->data.variable_decl.type = type;
  node->data.variable_decl.name = name;
  node->data.variable_decl.expr = expr;
  node->data.variable_decl.is_constant = is_constant;

  return node;
}

ast_node_t* ast_node_create_function_decl(variable_type_t return_type,
                                          const char* name,
                                          ast_node_t** parameters,
                                          ast_node_t* body,
                                          source_range_t location,
                                          source_range_t error_location) {
  assert(return_type != VAR_UNKNOWN);
  assert(name != NULL);
  /* assert(body != NULL); */

  ast_node_t* node = ast_node_create(AST_FUNCTION_DECL);
  node->location = location;
  node->error_location = error_location;
  node->data.function_decl.return_type = return_type;
  node->data.function_decl.name = name;
  node->data.function_decl.parameters = parameters;
  node->data.function_decl.body = body;

  return node;
}

ast_node_t* ast_node_create_unary_expr(operator_kind_t operator,
                                       ast_node_t * operand,
                                       source_range_t location) {
  assert(operator!= OP_UNKNOWN);
  assert(operand != NULL);

  ast_node_t* node = ast_node_create(AST_UNARY_EXPRESSION);
  node->location = node->error_location = location;
  node->data.operator= operator;
  arrput(node->children, operand);

  return node;
}

ast_node_t* ast_node_create_binary_expr(ast_node_t* lhs,
                                        operator_kind_t operator,
                                        ast_node_t * rhs,
                                        source_range_t location) {
  assert(operator!= OP_UNKNOWN);
  assert(lhs != NULL);
  assert(rhs != NULL);

  ast_node_t* node = ast_node_create(AST_BINARY_EXPRESSION);
  node->location = node->error_location = location;
  node->data.operator= operator;
  arrput(node->children, lhs);
  arrput(node->children, rhs);

  return node;
}

#if 0

static void emit_ast_internal(ast_node_t* node, FILE* stream, size_t depth);

void emit_ast(ast_node_t* root, FILE* stream) {
  emit_ast_internal(root, stream, 0);
}

static inline void emit_indent(FILE* stream, int depth) {
  for (int i = 0; i < depth; i++) {
    fprintf(stream, "\t");
  }
}

static void emit_ast_root(ast_node_t* node, FILE* stream, size_t depth);
static void emit_ast_variable_reservation(ast_node_t* node,
                                          FILE* stream,
                                          size_t depth);
static void emit_ast_variable_decl(ast_node_t* node,
                                   FILE* stream,
                                   size_t depth);
static void emit_ast_function_decl(ast_node_t* node,
                                   FILE* stream,
                                   size_t depth);
static void emit_ast_statement_block(ast_node_t* node,
                                     FILE* stream,
                                     size_t depth);
static void emit_ast_binary_expression(ast_node_t* node,
                                       FILE* stream,
                                       size_t depth);
static void emit_ast_unary_expression(ast_node_t* node,
                                      FILE* stream,
                                      size_t depth);

static void emit_ast_root(ast_node_t* node, FILE* stream, size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_ROOT);

  emit_indent(stream, depth);
  fputs("Root:\n", stream);
  ++depth;

  size_t num_decls = arrlenu(node->children);

  if (num_decls) {
    emit_indent(stream, depth);
    fputs("Global Declarations:\n", stream);

    for (size_t i = 0; i < num_decls; ++i) {
      emit_ast_internal(node->children[i], stream, depth + 1);
    }
  }
}

static void emit_ast_variable_reservation(ast_node_t* node,
                                          FILE* stream,
                                          size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_VARIABLE_RESERVATION);

  emit_indent(stream, depth);
  fputs("Variable Reservation:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  emit_variable(&node->data.reserved_variable, depth, stream);
}

static void emit_ast_variable_decl(ast_node_t* node,
                                   FILE* stream,
                                   size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_VARIABLE_DECL);

  emit_indent(stream, depth);
  fputs("Variable Declaration:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  ast_variable_decl_data_t* data = &node->data.variable_decl;

  emit_indent(stream, depth);
  fprintf(stream, "Type: %s\n", variable_type_to_string(data->type));

  emit_indent(stream, depth);
  fprintf(stream, "Name: %.*s\n", (int)arrlen(data->name), data->name);

  emit_indent(stream, depth);
  fprintf(stream, "Constant: %s\n", data->is_constant ? "True" : "False");

  if (data->expr != NULL) {
    emit_indent(stream, depth + 1);
    fputs("Expression:\n", stream);

    emit_ast_internal(data->expr, stream, depth + 2);
  }
}

static void emit_ast_function_decl(ast_node_t* node,
                                   FILE* stream,
                                   size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_FUNCTION_DECL);

  emit_indent(stream, depth);
  fputs("Function Declaration:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  ast_function_decl_data_t* data = &node->data.function_decl;

  emit_indent(stream, depth);
  fprintf(stream, "Return Type: %s\n",
          variable_type_to_string(data->return_type));

  emit_indent(stream, depth);
  fprintf(stream, "Name: %.*s\n", (int)arrlen(data->name), data->name);

  size_t num_parameters = arrlenu(data->parameters);
  if (num_parameters) {
    emit_indent(stream, depth);
    fputs("Parameters:\n", stream);

    for (size_t i = 0; i < num_parameters; ++i) {
      emit_ast_variable_decl(data->parameters[i], stream, depth + 1);
    }
  }

  if (data->body != NULL) {
    emit_ast_statement_block(data->body, stream, depth);
  }
}

static void emit_ast_statement_block(ast_node_t* node,
                                     FILE* stream,
                                     size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_STATEMENT_BLOCK);

  emit_indent(stream, depth);
  fputs("Block:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  size_t num_statements = arrlenu(node->children);

  if (num_statements) {
    emit_indent(stream, depth);
    fputs("Statements:\n", stream);

    for (size_t i = 0; i < num_statements; ++i) {
      emit_ast_internal(node->children[i], stream, depth + 1);
    }
  }
}

static void emit_ast_binary_expression(ast_node_t* node,
                                       FILE* stream,
                                       size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_BINARY_EXPRESSION);

  emit_indent(stream, depth);
  fputs("Binary Expression:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  assert(arrlen(node->children) == 2);

  emit_indent(stream, depth);
  fputs("LHS:\n", stream);

  emit_ast_internal(node->children[0], stream, depth + 1);

  emit_indent(stream, depth);
  fprintf(stream, "Operator: %s\n", op_to_string(node->data.operator));

  emit_indent(stream, depth);
  fputs("RHS:\n", stream);

  emit_ast_internal(node->children[1], stream, depth + 1);
}

static void emit_ast_unary_expression(ast_node_t* node,
                                      FILE* stream,
                                      size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);
  assert(node->kind == AST_UNARY_EXPRESSION);

  emit_indent(stream, depth);
  fputs("Unary Expression:\n", stream);
  ++depth;

  emit_range(&node->location, depth, stream);

  assert(arrlen(node->children) == 1);

  emit_indent(stream, depth);
  fprintf(stream, "Operator: %s\n",
          op_to_string(
              node->data.operator));  // TODO: More information to differentiate
                                      // between prefix/suffix, etc.

  emit_indent(stream, depth);
  fputs("Operand:\n", stream);

  emit_ast_internal(node->children[0], stream, depth + 1);
}

static void emit_ast_internal(ast_node_t* node, FILE* stream, size_t depth) {
  assert(node != NULL);
  assert(stream != NULL);

  switch (node->kind) {
    case AST_ROOT:
      emit_ast_root(node, stream, depth);
      break;
    case AST_FUNCTION_DECL:
      emit_ast_function_decl(node, stream, depth);
      break;
    case AST_VARIABLE_DECL:
      emit_ast_variable_decl(node, stream, depth);
      break;
    case AST_STATEMENT_BLOCK:
      emit_ast_statement_block(node, stream, depth);
      break;
    case AST_VARIABLE_RESERVATION:
      emit_ast_variable_reservation(node, stream, depth);
      break;
    case AST_BINARY_EXPRESSION:
      emit_ast_binary_expression(node, stream, depth);
      break;
    case AST_UNARY_EXPRESSION:
      emit_ast_unary_expression(node, stream, depth);
      break;
    case AST_IDENTIFIER:
      emit_indent(stream, depth);
      fprintf(stream, "Identifier\n");
      break;
    case AST_NUMERIC_LITERAL:
      emit_indent(stream, depth);
      fprintf(stream, "Numeric Literal\n");
      break;
    case AST_STRING_LITERAL:
      emit_indent(stream, depth);
      fprintf(stream, "String Literal\n");
      break;
    case AST_FUNCTION_CALL:
      emit_indent(stream, depth);
      fprintf(stream, "Function Call\n");
      break;
    case AST_IF_STATEMENT:
      emit_indent(stream, depth);
      fprintf(stream, "If Statement\n");
      break;
    case AST_WHILE_STATEMENT:
      emit_indent(stream, depth);
      fprintf(stream, "While Statement\n");
      break;
    /* case AST_FOR_STATEMENT: */
    /*   name = "For Statement"; */
    /*   break; */
    case AST_RETURN_STATEMENT:
      emit_indent(stream, depth);
      fprintf(stream, "Return Statement\n");
      break;
    default:
      UNREACHABLE();
  }
}

#endif
