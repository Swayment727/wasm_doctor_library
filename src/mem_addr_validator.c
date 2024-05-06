#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_reporter.h"
#include "mem_addr_validator.h"
#include "wasm_types.h"

#define BYTES_PER_WORD sizeof(word_t)

void
validate(struct mem_addr_validator *validator, doctorptr_t bit_idx)
{
        /* printf("%u %lu %lu\n", bit_idx, BYTES_PER_WORD, */
        /* bit_idx / BYTES_PER_WORD); */
        validator->words[bit_idx / BYTES_PER_WORD] |= 1 << bit_idx % BYTES_PER_WORD;
}

void
invalidate(struct mem_addr_validator *validator, doctorptr_t bit_idx)
{
        validator->words[bit_idx / BYTES_PER_WORD] &= ~(1 << bit_idx % BYTES_PER_WORD);
}

void
check_access(struct mem_addr_validator *validator, doctorptr_t bit_idx)
{
        if ((validator->words[bit_idx / BYTES_PER_WORD] & (1 << bit_idx % BYTES_PER_WORD)) == 0) {
                bool validity[1] = {false};
                add_undefined_memory_use(validator->reporter, bit_idx, 1, validity,
                                         validator->reporter->state
                                                 ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
validate_region(struct mem_addr_validator *validator, wasmptr_t address, uint32_t size_in_bytes)
{
        for (uint32_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                validate(validator, i);
        }
}

void
invalidate_region(struct mem_addr_validator *validator, wasmptr_t address, uint32_t size_in_bytes)
{

        for (uint32_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                invalidate(validator, i);
        }
}

void
check_region_access(struct mem_addr_validator *validator, wasmptr_t address, uint32_t size_in_bytes)
{
        bool validity[size_in_bytes * 8];

        bool is_valid = true;
        for (uint32_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                if ((validator->words[i / BYTES_PER_WORD] & (1 << i % BYTES_PER_WORD)) == 0) {
                        is_valid = false;
                        validity[i - address * 8] = false;
                } else {
                        validity[i - address * 8] = true;
                }
        }

        if (!is_valid) {
                add_undefined_memory_use(validator->reporter, address, validator->reporter->state->size_in_bytes,
                                         validity,
                                         validator->reporter->state
                                                 ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
mem_addr_validator_init(struct mem_addr_validator *validator, uint32_t mem_size, struct error_reporter *reporter)
{
        validator->words = (word_t *)malloc(mem_size * 8);
        invalidate_region(validator, 0, mem_size);

        validator->reporter = reporter;
}

void
mem_addr_validator_exit(struct mem_addr_validator *validator)
{
        free(validator->words);
}
