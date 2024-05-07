#ifndef HEAP_USE_VALIDATOR
#define HEAP_USE_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "shadow_stack_validator.h"

struct global_block {
        size_t block_start;
        size_t size_in_bytes;
};

struct allocated_block {
        size_t block_start;
        size_t size_in_bytes;
        bool freed;
        char *allocated_in_function_name;
};

struct heap_use_validator {
        struct error_reporter *reporter;
        size_t global_blocks_size;
        struct shadow_stack_validator *shadow_stack_validator;
        struct global_block *global_blocks;
        size_t blocks_size;
        struct allocated_block *blocks;
};

void register_global_data(struct heap_use_validator *validator, size_t address, size_t size_in_bytes);
void register_malloc(struct heap_use_validator *validator, size_t block_start, size_t size_in_bytes);
void register_free(struct heap_use_validator *validator, size_t block_start);
void check_use_after_free(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes);
void check_read_validity(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes);
void check_write_validity(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes);
void heap_use_validator_init(struct heap_use_validator *validator,
                             struct shadow_stack_validator *shadow_stack_validator, struct error_reporter *reporter);
void heap_use_validator_exit(struct heap_use_validator *validator);

#endif /* HEAP_USE_VALIDATOR */
