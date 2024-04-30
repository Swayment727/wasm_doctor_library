#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"
#include "heap_use_validator.h"

void
register_global_data(struct heap_use_validator *validator, wasmptr_t address, uint32_t size_in_bytes)
{
        validator->global_blocks =
                realloc(validator->global_blocks, ++validator->global_blocks_size * sizeof(*validator->global_blocks));
        validator->global_blocks[validator->global_blocks_size - 1].block_start = address;
        validator->global_blocks[validator->global_blocks_size - 1].size_in_bytes = size_in_bytes;
}

void
register_malloc(struct heap_use_validator *validator, wasmptr_t block_start, uint32_t size_in_bytes)
{
        validator->blocks = realloc(validator->blocks, ++validator->blocks_size * sizeof(*validator->blocks));
        validator->blocks[validator->blocks_size - 1].block_start = block_start;
        validator->blocks[validator->blocks_size - 1].size_in_bytes = size_in_bytes;
        validator->blocks[validator->blocks_size - 1].freed = false;
        validator->blocks[validator->blocks_size - 1].allocated_in_function_name = (char *)malloc(
                strnlen(validator->reporter->state->function_names[validator->reporter->state->function_names_size - 1],
                        50) +
                1); // TODO: 50

        strncpy(validator->blocks[validator->blocks_size - 1].allocated_in_function_name,
                validator->reporter->state->function_names[validator->reporter->state->function_names_size - 1],
                strnlen(validator->reporter->state->function_names[validator->reporter->state->function_names_size - 1],
                        50) +
                        1); // TODO: 50
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
check_use_after_free(struct heap_use_validator *validator, doctorptr_t address,
                     uint32_t bit_size) // TODO: check address - it is in bit form, will it fit?
{
        bool found_on_heap = false;
        bool found_allocated = false;

        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start * 8 <= address + bit_size &&
                    address <= validator->blocks[i].block_start * 8 + validator->blocks[i].size_in_bytes * 8) {
                        found_on_heap = true;

                        if (validator->blocks[i].freed == false) {
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

static bool
is_read_write_valid(struct heap_use_validator *validator, doctorptr_t address, uint32_t bit_size)
{
        if (validator->shadow_stack_validator->shadow_stack_pointer * 8 <= address &&
            address + bit_size <= validator->shadow_stack_validator->shadow_stack_pointer_base * 8) {
                return true;
        }

        for (uint32_t i = 0; i < validator->global_blocks_size; ++i) {
                if (validator->global_blocks[i].block_start * 8 <= address + bit_size &&
                    address <= validator->global_blocks[i].block_start * 8 +
                                       validator->global_blocks[i].size_in_bytes * 8) {
                        return true;
                }
        }

        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start * 8 <= address + bit_size &&
                    address <= validator->blocks[i].block_start * 8 + validator->blocks[i].size_in_bytes * 8) {
                        return true;
                }
        }

        return false;
}

void
check_read_validity(struct heap_use_validator *validator, doctorptr_t address,
                    uint32_t bit_size) // TODO: check address - it is in bit form, will it fit?
{
        if (!is_read_write_valid(validator, address, bit_size)) {
                add_invalid_read(validator->reporter, address, bit_size,
                                 validator->reporter->state
                                         ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
check_write_validity(struct heap_use_validator *validator, doctorptr_t address,
                     uint32_t bit_size) // TODO: check address - it is in bit form, will it fit?
{
        if (!is_read_write_valid(validator, address, bit_size)) {
                add_invalid_write(validator->reporter, address, bit_size,
                                  validator->reporter->state
                                          ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
heap_use_validator_init(struct heap_use_validator *validator, struct shadow_stack_validator *shadow_stack_validator,
                        struct error_reporter *reporter)
{
        validator->blocks_size = 0;
        validator->blocks = NULL;

        validator->global_blocks_size = 0;
        validator->global_blocks = NULL;

        validator->shadow_stack_validator = shadow_stack_validator;
        validator->reporter = reporter;
}

void
heap_use_validator_exit(struct heap_use_validator *validator)
{
        for (uint32_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].freed == false) {

                        add_memory_leak(validator->reporter, validator->blocks[i].block_start,
                                        validator->blocks[i].size_in_bytes,
                                        validator->blocks[i].allocated_in_function_name);
                }

                free(validator->blocks[i].allocated_in_function_name);
        }

        free(validator->blocks);

        free(validator->global_blocks);
}
