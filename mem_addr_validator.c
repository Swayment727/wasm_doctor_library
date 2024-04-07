#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem_addr_validator.h"

#define BYTES_PER_WORD sizeof(word_t)

void
validate(struct shadow_memory *mem, uint32_t bit_idx)
{
        mem->words[bit_idx / BYTES_PER_WORD] |= 1 << bit_idx % BYTES_PER_WORD;
}

void
invalidate(struct shadow_memory *mem, uint32_t bit_idx)
{
        mem->words[bit_idx / BYTES_PER_WORD] &=
                ~(1 << bit_idx % BYTES_PER_WORD);
}

bool
is_valid(struct shadow_memory *mem, uint32_t bit_idx)
{
        return (mem->words[bit_idx / BYTES_PER_WORD] &
                (1 << bit_idx % BYTES_PER_WORD)) != 0;
}

void
validate_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                validate(mem, i);
        }
}

void
invalidate_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                  uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                invalidate(mem, i);
        }
}

bool
is_valid_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                uint32_t bit_idx_end)
{
        for (uint32_t i = bit_idx_start; i <= bit_idx_end; ++i) {
                if (!is_valid(mem, i)) {
                        return false;
                }
        }

        return true;
}

void
shadow_memory_init(struct shadow_memory *mem, uint32_t mem_size)
{
        mem->words = (word_t *)malloc(mem_size / sizeof(*mem->words) *
                                      sizeof(word_t));
        invalidate_region(mem, 0, mem_size / sizeof(*mem->words));
}
