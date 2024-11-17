#define DRIVER_OPTIONS_IMPL
#include <driver/options.h>

#include <ti-basic-plus-plus/basic/file_type.h>

#include <assert.h>
#include <ctype.h>
#include <stb_ds.h>
#include <stdio.h>
#include <string.h>

driver_config_t driver_config;

#define OPT_NAME(name) &"-" name[1]

// clang-format off
const option_t driver_options[] = {
    {OPTION_HELP,                false,     NULL, {OPT_NAME("h"), OPT_NAME("help")}, "Print this help message, then exit"},
    {OPTION_VERSION,             false,     NULL, {OPT_NAME("version"),       NULL}, "Print the compiler version, then exit"},
    {OPTION_WARNINGS_AS_ERRORS,  false,     NULL, {OPT_NAME("Werror"),        NULL}, "Treat warnings as errors"},
    {OPTION_OUTPUT,              true,  "<file>", {OPT_NAME("o"),             NULL}, "Write output to file"},
    {OPTION_SEND_OUTPUT,         false,     NULL, {OPT_NAME("s"), OPT_NAME("send")}, "Send output program to calculator"},
    {OPTION_VERBOSE,             false,     NULL, {OPT_NAME("v"),             NULL}, "Enable verbose output"},
    {OPTION_SUPPRESS_WARNINGS,   false,     NULL, {OPT_NAME("w"),             NULL}, "Suppress warnings"},
    {OPTION_DUMP_TOKENS,         false,     NULL, {OPT_NAME("dump-tokens"),   NULL}, "Emit parsed tokens"},
    {OPTION_DUMP_AST,            false,     NULL, {OPT_NAME("dump-ast"),      NULL}, "Emit generated AST"},
};
// clang-format on

const size_t driver_options_count = sizeof(driver_options) / sizeof(option_t);

static void print_usage(FILE* output);
static void print_version(void);

static void driver_config_init(void);

static void process_input_options(input_option_t* input_options, const char** input_paths,
                                  diagnostics_t* d);

static void driver_config_init(void) {
  memset(&driver_config, 0, sizeof(driver_config_t));
}

void parse_arguments(int argc, const char** argv, diagnostics_t* d) {
  assert(argc > 0 && argv != NULL);
  assert(d != NULL);

  driver_config_init();

  driver_config.invocation = argv[0];
  ++argv, --argc;

  const char** input_paths = NULL;
  input_option_t* input_options = NULL;

  while (argc > 0) {
    argument_parser_error_t error =
        option_parse(&argc, &argv, driver_options, driver_options_count,
                     &input_options, &input_paths);

    if (error == INVALID_OPTION) {
      diag_report(d, FATAL_ERROR_OPTION_INVALID, *argv);
      goto CLEANUP;
    } else if (error == MISSING_VALUE) {
      diag_report(d, FATAL_ERROR_OPTION_MISSING_VALUE, *argv);
      goto CLEANUP;
    }
  }

  process_input_options(input_options, input_paths, d);

CLEANUP:
  arrfree(input_paths);
  arrfree(input_options);
  return;
}

static void process_input_options(input_option_t* input_options, const char** input_paths,
                                  diagnostics_t* d) {
  assert(d != NULL);
  bool seen_options[_OPTION_COUNT] = {0};

  option_id_t id;
  const char* value;
  for (size_t i = 0; i < arrlenu(input_options); ++i) {
    if (should_exit(d)) {
      return;
    }

    id = input_options[i].option->id;
    value = input_options[i].value;

    assert(input_options[i].option->has_value <= (value != NULL));

    if (seen_options[id]) {
      diag_report(d, FATAL_ERROR_OPTION_DUPLICATE,
                  input_options[i].option->names[0]);
      return;
    }
    seen_options[id] = true;

    switch (id) {
      case OPTION_HELP:
        print_usage(stdout);
        d->should_exit = true;
        return;
      case OPTION_VERSION:
        print_version();
        d->should_exit = true;
        return;
      case OPTION_OUTPUT:
        driver_config.output_path = value;
        break;
      case OPTION_SEND_OUTPUT:
        driver_config.send = true;
        break;
      case OPTION_VERBOSE:
        driver_config.verbose = true;
        break;
      case OPTION_SUPPRESS_WARNINGS:
        d->suppress_warnings = true;
        break;
      case OPTION_WARNINGS_AS_ERRORS:
        d->warnings_as_errors = true;
        break;
      case OPTION_DUMP_TOKENS:
        driver_config.dump_tokens = true;
        break;
      case OPTION_DUMP_AST:
        driver_config.dump_ast = true;
        break;
      default:
        assert(false);
        return;
    }
  }

  size_t num_input_paths = arrlenu(input_paths);
  if (num_input_paths < 1) {
    diag_report(d, FATAL_ERROR_NO_INPUT_FILE);
    return;
  } else if (num_input_paths > 1) {
    diag_report(d, FATAL_ERROR_MULTIPLE_INPUT_FILES);
    return;
  }
  driver_config.input_path = input_paths[0];
  validate_file_exists(driver_config.input_path, d);
  if (should_exit(d)) {
    return;
  }

  // Check input file extension.

  const char* input_extension_ptr;
  file_extension_t input_extension =
      get_file_extension(driver_config.input_path, &input_extension_ptr);

  switch (input_extension) {
    case FILE_EXT_TIBASICPP:
      driver_config.source_path = driver_config.input_path;
      break;
    case FILE_EXT_TIBASIC:
      driver_config.tibasic_path = driver_config.input_path;
      break;
    case FILE_EXT_8XP:
      driver_config.program_path = driver_config.input_path;
      break;
    default:
      diag_report_file(d, driver_config.input_path,
                       FATAL_ERROR_UNKNOWN_FILE_EXTENSION, input_extension_ptr);
      return;
  }

  // Check output file extension


  if (driver_config.output_path == NULL) {
    if (input_extension != FILE_EXT_8XP || !driver_config.send) {
      diag_report(d, FATAL_ERROR_NO_OUTPUT);
      return;
    }
  } else {
    const char* output_extension_ptr;
    file_extension_t output_extension =
        get_file_extension(driver_config.output_path, &output_extension_ptr);

    if (driver_config.send && output_extension != FILE_EXT_8XP) {
      diag_report_file(d, driver_config.output_path, FATAL_ERROR_SEND_OUTPUT_NOT_8XP);
      return;
    }

    if (input_extension == output_extension) {
      diag_report_file(d, driver_config.output_path,
                       FATAL_ERROR_INPUT_OUTPUT_SAME_EXTENSION);
      return;
    }

    if (input_extension == FILE_EXT_8XP) {
      diag_report_file(d, driver_config.input_path,
                       FATAL_ERROR_CANNOT_CONVERT_FROM_8XP);
      return;
    }

    switch (output_extension) {
      case FILE_EXT_8XP:
        validate_8xp_filename(driver_config.output_path, output_extension_ptr,
                              driver_config.program_name, d);
        if (is_fatal(d)) {
          return;
        }

        driver_config.program_path = driver_config.output_path;
        break;
      case FILE_EXT_TIBASIC:
        driver_config.tibasic_path = driver_config.output_path;
        break;
      case FILE_EXT_TIBASICPP:
        diag_report_file(d, driver_config.output_path,
                         FATAL_ERROR_CANNOT_CONVERT_TO_TIBASICPP);
        return;
      default:
        diag_report_file(d, driver_config.output_path,
                         FATAL_ERROR_UNKNOWN_FILE_EXTENSION, output_extension_ptr);
        return;
    }
  }
}

static void print_usage(FILE* output) {
  fprintf(output, "Usage: ti-basic++ <task> [options] <input file>\n");
  fprintf(output, "Tasks:\n");

  // clang-format off
  fputs("  build              Compile a TI-BASIC++ program\n", output);
  fputs("  send               Send a compiled .8xp program to a calculator\n", output);
  fputs("  buildsend          Compile a TI-BASIC++ program and send it to a calculator\n", output);
  fputs("  help               Print this help message, then exit\n", output);
  fputs("  version            Print the compiler version, then exit\n", output);
  // clang-format on

  fprintf(output, "\nOptions:\n");

  static const size_t MAX_HELP_OFFSET = 21;

  const option_t* option = driver_options;
  for (size_t i = 0; i < driver_options_count; ++i, ++option) {
    if (option->help == NULL)
      continue;

    assert(option->names[0] != NULL);

    size_t help_offset = 2 + 1 + strlen(option->names[0]);

    fprintf(output, "  -%s", option->names[0]);

    if (option->has_value) {
      if (option->value_name != NULL) {
        fprintf(output, " %s", option->value_name);
        help_offset += 1 + strlen(option->value_name);
      } else {
        fprintf(output, " <value>");
        help_offset += 8;
      }
    }

    if (help_offset >= MAX_HELP_OFFSET) {
      fputc('\n', output);
      help_offset = 0;
    }

    while (help_offset < MAX_HELP_OFFSET) {
      fputc(' ', output);
      ++help_offset;
    }

    fprintf(output, "%s\n", option->help);
  }
}

static void print_version(void) {
  printf("ti-basic++ version 0.0.1\n");
}

