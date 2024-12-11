#define _DEFAULT_SOURCE
#include <ti-basic-plus-plus/os/terminal.h>

#include <unistd.h>

bool is_tty(FILE* stream) {
  return isatty(fileno(stream));
}
 
