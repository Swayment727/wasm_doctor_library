#ifndef ERROR_REPORTER
#define ERROR_REPORTER

#include <stdbool.h>
#include <stdint.h>

#include "wasm_state.h"
#include "wasm_types.h"

/**
 * Can potentially be extended in the future with line number through the use of DWARF information or with stack trace.
 */
struct error_location {
        char *function_name;
};

struct undefined_memory_use {
        wasmptr_t address;
        uint32_t size;
        bool *validity;
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

struct invalid_free {
        wasmptr_t address;
        struct error_location location;
};

struct invalid_read {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct invalid_write {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct zero_address_access {
        wasmptr_t address;
        uint32_t size;
        struct error_location location;
};

struct error_reporter {
        struct wasm_state *state;
        bool report;

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

        uint32_t invalid_free_errors_size;
        struct invalid_free *invalid_free_errors;

        uint32_t invalid_read_errors_size;
        struct invalid_read *invalid_read_errors;

        uint32_t invalid_write_errors_size;
        struct invalid_write *invalid_write_errors;

        uint32_t zero_address_access_errors_size;
        struct zero_address_access *zero_address_access_errors;
};

void add_undefined_memory_use(struct error_reporter *reporter, wasmptr_t address, uint8_t size, bool *validity,
                              char *function_name);
void add_undefined_local_use(struct error_reporter *reporter, uint32_t idx, char *function_name);
void add_use_after_free(struct error_reporter *reporter, wasmptr_t address, uint8_t size_in_bytes, char *function_name);
void add_memory_leak(struct error_reporter *reporter, wasmptr_t address, uint8_t size_in_bytes, char *function_name);
void add_double_free(struct error_reporter *reporter, wasmptr_t address, char *function_name);
void add_invalid_free(struct error_reporter *reporter, wasmptr_t address, char *function_name);
void add_invalid_read(struct error_reporter *reporter, wasmptr_t address, uint8_t size_in_bytes, char *function_name);
void add_invalid_write(struct error_reporter *reporter, wasmptr_t address, uint8_t size_in_bytes, char *function_name);
void add_zero_address_access(struct error_reporter *reporter, char *function_name);
void reporter_init(struct error_reporter *reporter, struct wasm_state *state, bool report);
void reporter_exit(struct error_reporter *reporter);

#endif /* ERROR_REPORTER */
