#include <stdbool.h>
#include <stdlib.h>

#include "error_reporter.h"
#include "heap_use_validator.h"

void
register_malloc(struct heap_use_validator *validator, wasmptr_t block_start, uint32_t size_in_bytes)
{
        validator->blocks = (struct allocated_block *)realloc(validator->blocks,
                                                              sizeof(*validator->blocks) * ++validator->blocks_size);
        validator->blocks[validator->blocks_size - 1].block_start = block_start;
        validator->blocks[validator->blocks_size - 1].size_in_bytes = size_in_bytes;
        validator->blocks[validator->blocks_size - 1].freed = false;
}

void
register_free(struct heap_use_validator *validator, wasmptr_t block_start)
{
        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start == block_start) {
                        if (validator->blocks[i].freed == false) {
                                validator->blocks[i].freed = true;
                                return;
                        } else {
                                add_double_free(
                                        validator->reporter, block_start,
                                        validator->reporter->state
                                                ->function_names[validator->reporter->state->function_names_size - 1]);
                                return;
                        }
                }
        }

        add_invalid_free(
                validator->reporter, block_start,
                validator->reporter->state->function_names[validator->reporter->state->function_names_size - 1]);
}

void
check_use_after_free(struct heap_use_validator *validator, wasmptr_t address,
                     uint32_t bit_size) // TODO: check address - it is in bit form, will it fit?
{
        bool found_on_heap = false;
        bool found_allocated = false;

        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start * 8 <= address + bit_size &&
                    address <= validator->blocks[i].block_start * 8 + validator->blocks[i].size_in_bytes * 8) {
                        found_on_heap = true;

                        if (validator->blocks[i].freed == true) {
                                found_allocated = true;
                        }
                }
        }

        if (found_on_heap && !found_allocated) {
                add_use_after_free(validator->reporter, address, bit_size,
                                   validator->reporter->state
                                           ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
heap_use_validator_init(struct heap_use_validator *validator, struct error_reporter *reporter)
{
        validator->blocks_size = 0;
        validator->blocks = NULL;

        validator->reporter = reporter;
}

void
heap_use_validator_exit(struct heap_use_validator *validator)
{
        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].freed == false) {
                        add_memory_leak(validator->reporter, validator->blocks[i].block_start,
                                        validator->blocks[i].size_in_bytes,
                                        validator->reporter->state
                                                ->function_names[validator->reporter->state->function_names_size - 1]);
                }
        }

        free(validator->blocks);
}
