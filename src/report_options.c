#include "./report_options.h"

void 
set_report_level(enum report_level level){
    current_report_level = level;
}

enum report_level
get_report_level(){
    return current_report_level;
}

void
set_report_to_file(bool report){
    report_to_file = report;
}

bool
get_report_to_file(){
    return report_to_file;
}
