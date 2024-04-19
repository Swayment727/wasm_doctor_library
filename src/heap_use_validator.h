#ifndef HEAP_USE_VALIDATOR
#define HEAP_USE_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t wasmptr_t;

struct heap_use_validator {
        uint32_t blocks_size;
        struct allocated_block *blocks;
};

struct allocated_block {
        wasmptr_t block_start;
        uint32_t size_in_bytes;
        bool freed;
};

void register_malloc(struct heap_use_validator *validator,
                     wasmptr_t block_start, uint32_t size_in_bytes);
void register_free(struct heap_use_validator *validator,
                   wasmptr_t block_start);
void heap_use_validator_exit(struct heap_use_validator *validator);

#endif /* HEAP_USE_VALIDATOR */
