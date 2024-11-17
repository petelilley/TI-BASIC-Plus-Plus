#include <driver/options.h>

int main(int argc, const char** argv) {
  diagnostics_t d = diagnostics_create();

  do {
    parse_arguments(argc, argv, &d);
    if (should_exit(&d)) {
      break;
    }

    printf("invocation: %s\n", driver_config.invocation);
    printf("input_path: %s\n", driver_config.input_path);
    printf("output_path: %s\n", driver_config.output_path);
    printf("source_path: %s\n", driver_config.source_path);
    printf("tibasic_path: %s\n", driver_config.tibasic_path);
    printf("program_path: %s\n", driver_config.program_path);
    printf("output_program_name: %.*s\n", 8, driver_config.program_name);
    printf("send: %d\n", driver_config.send);
    printf("verbose: %d\n", driver_config.verbose);
    printf("dump_tokens: %d\n", driver_config.dump_tokens);
    printf("dump_ast: %d\n", driver_config.dump_ast);

  } while (false);

  return 0;
}

