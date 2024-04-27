#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "error_reporter.h"
#include "mem_addr_validator.h"

#define BYTES_PER_WORD sizeof(word_t)

void
validate(struct mem_addr_validator *validator, uint32_t bit_idx)
{
        /* printf("%u %lu %lu\n", bit_idx, BYTES_PER_WORD, */
        /* bit_idx / BYTES_PER_WORD); */
        validator->words[bit_idx / BYTES_PER_WORD] |= 1 << bit_idx % BYTES_PER_WORD;
}

void
invalidate(struct mem_addr_validator *validator, uint32_t bit_idx)
{
        validator->words[bit_idx / BYTES_PER_WORD] &= ~(1 << bit_idx % BYTES_PER_WORD);
}

void
check_access(struct mem_addr_validator *validator, uint32_t bit_idx)
{
        if ((validator->words[bit_idx / BYTES_PER_WORD] & (1 << bit_idx % BYTES_PER_WORD)) == 0) {
                add_undefined_memory_use(validator->reporter, bit_idx, validator->reporter->state->bit_size,
                                         validator->reporter->state
                                                 ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
validate_region(struct mem_addr_validator *validator, uint32_t bit_idx_start, uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                validate(validator, i);
        }
}

void
invalidate_region(struct mem_addr_validator *validator, uint32_t bit_idx_start, uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                invalidate(validator, i);
        }
}

void
check_region_access(struct mem_addr_validator *validator, uint32_t bit_idx_start, uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                check_access(validator, i);
        }
}

void
mem_addr_validator_init(struct mem_addr_validator *validator, uint32_t mem_size, struct error_reporter *reporter)
{
        validator->words = (word_t *)malloc(mem_size);
        invalidate_region(validator, 0, mem_size - 1);

        validator->reporter = reporter;
}

void
mem_addr_validator_exit(struct mem_addr_validator *validator)
{
        free(validator->words);
}
