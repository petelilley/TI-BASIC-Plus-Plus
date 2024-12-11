#ifndef DRIVER_OPTIONS_H
#define DRIVER_OPTIONS_H

#include <stdbool.h>
#include <ti-basic-plus-plus/basic/diagnostics.h>
#include <ti-basic-plus-plus/basic/options.h>
#include <ti-basic-plus-plus/basic/stringref.h>

enum {
  OPTION_HELP,         // -h, --help
  OPTION_VERSION,      // --version
                       //
  OPTION_OUTPUT,       // -o<file>
  OPTION_SEND_OUTPUT,  // -s, --send

  OPTION_VERBOSE,             // -v
  OPTION_SUPPRESS_WARNINGS,   // -w
  OPTION_WARNINGS_AS_ERRORS,  // -Werror

  OPTION_DUMP_TOKENS,  // --dump-tokens
  OPTION_DUMP_AST,     // --dump-ast

  OPTION_COLOR,  // --color <auto|always|never>

  _OPTION_COUNT,
};

extern const option_t driver_options[];
extern const size_t driver_options_count;

typedef struct {
  const char* invocation;

  const char* input_path;
  const char* output_path;

  const char* source_path;
  const char* tibasic_path;
  const char* program_path;
  char program_name[8];

  enum {
    COLOR_AUTO,
    COLOR_ALWAYS,
    COLOR_NEVER,
  } color;

  bool send;
  bool verbose;
  bool dump_tokens;
  bool dump_ast;
} driver_config_t;

#ifndef DRIVER_OPTIONS_IMPL
extern const driver_config_t driver_config;
#endif

void parse_arguments(int argc, const char** argv, diagnostics_t* d);

#endif  // DRIVER_OPTIONS_H

