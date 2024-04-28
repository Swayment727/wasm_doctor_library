#ifndef HEAP_USE_VALIDATOR
#define HEAP_USE_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "wasm_types.h"

struct heap_use_validator {
        struct error_reporter *reporter;
        uint32_t blocks_size;
        struct allocated_block *blocks;
};

struct allocated_block {
        wasmptr_t block_start;
        uint32_t size_in_bytes;
        bool freed;
        char *allocated_in_function_name;
};

void register_malloc(struct heap_use_validator *validator, wasmptr_t block_start, uint32_t size_in_bytes);
void register_free(struct heap_use_validator *validator, wasmptr_t block_start);
void check_use_after_free(struct heap_use_validator *validator, wasmptr_t address, uint32_t bit_size);
void heap_use_validator_init(struct heap_use_validator *validator, struct error_reporter *reporter);
void heap_use_validator_exit(struct heap_use_validator *validator);

#endif /* HEAP_USE_VALIDATOR */
