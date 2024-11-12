#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <ti-basic-plus-plus/basic/source_location.h>

#include <stdbool.h>
#include <stddef.h>

typedef enum {
// <SEVERITY>_<ID>
#define DIAG(SEVERITY, ID, TEXT) SEVERITY##_##ID,
#define DIAG_FILE(SEVERITY, ID, TEXT) SEVERITY##_##ID,
#define DIAG_LOCATION(SEVERITY, ID, TEXT) SEVERITY##_##ID,
#include <ti-basic-plus-plus/basic/diagnostics.inc>
#undef DIAG
#undef DIAG_FILE
#undef DIAG_LOCATION
} diagnostic_id_t;

typedef struct {
  size_t max_errors;

  size_t error_count;
  bool should_exit;

  bool suppress_remarks;
  bool suppress_warnings;
  bool warnings_as_errors;
} diagnostics_t;

void diagnostics_init(diagnostics_t* d);
diagnostics_t diagnostics_create(void);

void diag_report(diagnostics_t* d, diagnostic_id_t id, ...);
void diag_report_file(diagnostics_t* d, const char* file, diagnostic_id_t id, ...);
void diag_report_source(diagnostics_t* d, source_range_t* range, diagnostic_id_t id, ...);

static inline bool has_errors(const diagnostics_t* d) {
  assert(d != NULL);
  return d->error_count > 0;
}

static inline bool is_fatal(const diagnostics_t* d) {
  assert(d != NULL);
  return (d->error_count >= d->max_errors) ||
         d->should_exit;
}

static inline bool should_exit(const diagnostics_t* d) {
  assert(d != NULL);
  return is_fatal(d) || has_errors(d);
}

static inline int get_exit_status(const diagnostics_t* d) {
  assert(d != NULL);
  return has_errors(d) ? 1 : 0;
}

#endif  // DIAGNOSTICS_H

