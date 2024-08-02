#ifndef REPORT_OPTIONS
#define REPORT_OPTIONS

#include <stdbool.h>

enum report_level {
        NO_REPORT,
        BOOL_REPORT,
        TYPE_REPORT,
        STACKTRACE_REPORT,
};

static enum report_level current_report_level = NO_REPORT;
static bool report_to_file = false;

void set_report_level(enum report_level level);
enum report_level get_report_level();


void set_report_to_file(bool report);
bool get_report_to_file();

#endif /* REPORT_OPTIONS_ */
