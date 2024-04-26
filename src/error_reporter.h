#ifndef ERROR_REPORTER
#define ERROR_REPORTER

#include <stdint.h>

#include "wasm_types.h"

struct error_location {
        char *function_name;
};

struct undefined_memory_use {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct undefined_local_use {
        uint32_t idx;
        uint32_t size;
        struct error_location location;
};

struct use_after_free {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct memory_leak {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct double_free {
        wasmptr_t address;
        struct error_location location;
};

struct error_reporter {
        uint32_t undefined_memory_use_errors_size;
        struct undefined_memory_use *undefined_memory_use_errors;

        uint32_t undefined_local_use_errors_size;
        struct undefined_local_use *undefined_local_use_errors;

        uint32_t use_after_free_errors_size;
        struct use_after_free *use_after_free_errors;

        uint32_t memory_leak_errors_size;
        struct memory_leak *memory_leak_errors;

        uint32_t double_free_errors_size;
        struct double_free *double_free_errors;
};

void report(struct error_reporter *reporter);
void add_undefined_memory_use(struct error_reporter *reporter,
                              uint32_t address, uint32_t size,
                              char *function_name);
void add_undefined_local_use(struct error_reporter *reporter, uint32_t idx,
                             uint32_t size, char *function_name);
void add_use_after_free(struct error_reporter *reporter, uint32_t address,
                        uint32_t size, char *function_name);
void add_memory_leak(struct error_reporter *reporter, uint32_t address,
                     uint32_t size, char *function_name);
void add_doubled_free(struct error_reporter *reporter, uint32_t address,
                      char *function_name);
void reporter_exit(struct error_reporter *reporter);

#endif /* ERROR_REPORTER */
