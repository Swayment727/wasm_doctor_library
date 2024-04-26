#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap_use_validator.h"

void
register_malloc(struct heap_use_validator *validator, wasmptr_t block_start,
                uint32_t size_in_bytes)
{
        // TODO: check the first realloc
        validator->blocks = (struct allocated_block *)realloc(
                validator->blocks,
                sizeof(*validator->blocks) * ++validator->blocks_size);
        validator->blocks[validator->blocks_size - 1].block_start =
                block_start;
        validator->blocks[validator->blocks_size - 1].size_in_bytes =
                size_in_bytes;
        validator->blocks[validator->blocks_size - 1].freed = false;

        printf("------------------- malloced from %u - %u\n", block_start,
               block_start + size_in_bytes);
}

void
register_free(struct heap_use_validator *validator, wasmptr_t block_start)
{
        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start == block_start) {
                        validator->blocks[i].freed = true;
                        return;
                }
        }

        // TODO: report free of unallocated block
        printf("Attempting to free unallocated block of memory.");
}

void
heap_use_validator_init(struct heap_use_validator *validator,
                        struct error_reporter *reporter)
{
        validator->reporter = reporter;
}

void
heap_use_validator_exit(struct heap_use_validator *validator)
{
        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].freed == false) {
                        // TODO: report memory leak
                        printf("Memory leak of %u bytes at address %u\n",
                               validator->blocks[i].size_in_bytes,
                               validator->blocks[i].block_start);
                }
        }

        free(validator->blocks);
}

// TODO: Add a reporter that aggregates the errors and possibly prints them
