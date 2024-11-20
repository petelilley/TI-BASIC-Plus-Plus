#include <driver/options.h>

#include <ti-basic-plus-plus/basic/input_file.h>
#include <ti-basic-plus-plus/lexer/lexer.h>
#include <ti-basic-plus-plus/parser/ast_node.h>
#include <ti-basic-plus-plus/parser/parser.h>

static diagnostics_t d;

static void compile(void);

int main(int argc, const char** argv) {
  d = diagnostics_create();

  do {
    parse_arguments(argc, argv, &d);
    if (should_exit(&d)) {
      break;
    }

    if (driver_config.source_path != NULL) {
      compile();
      if (should_exit(&d)) {
        break;
      }
    }
  } while (false);

  return get_exit_status(&d);
}

static void compile(void) {
  assert(driver_config.source_path != NULL);

  input_file_t input_file;

  if (!if_init(&input_file, driver_config.input_path)) {
    diag_report_file(&d, driver_config.input_path, FATAL_ERROR_FILE_OPEN_FAILED);
    return;
  }

  token_t* head_token = NULL;
  ast_node_t* ast_root = NULL;

  do {
    // Lexical analysis

    head_token = tokenize_file(&input_file, &d);
    if (head_token == NULL || should_exit(&d)) {
      break;
    }

    if (driver_config.dump_tokens) {
      emit_token_list(head_token, stdout);
    }

    ast_root = parse_tokens(head_token, &d);
    if (ast_root == NULL || should_exit(&d)) {
      break;
    }

    if (driver_config.dump_ast) {
      emit_ast(ast_root, stdout);
    }

    // TODO: Semantic analysis, code generation

  } while (false);

  if (ast_root != NULL) {
    ast_node_destroy(ast_root);
  }
  if (head_token != NULL) {
    token_list_destroy(head_token);
  }

  if_destroy(&input_file);
}
