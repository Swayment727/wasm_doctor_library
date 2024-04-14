#ifndef MEM_ADDR_VALIDATOR
#define MEM_ADDR_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t word_t;

struct shadow_memory {
        word_t *words;
};

void validate(struct shadow_memory *mem, uint32_t bit_idx);
void invalidate(struct shadow_memory *mem, uint32_t bit_idx);
bool is_valid(struct shadow_memory *mem, uint32_t bit_idx);
void validate_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                     uint32_t bit_idx_end);
void invalidate_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                       uint32_t bit_idx_end);
bool is_valid_region(struct shadow_memory *mem, uint32_t bit_idx_start,
                     uint32_t bit_idx_end);
void shadow_memory_init(struct shadow_memory *mem, uint32_t mem_size);
void shadow_memory_exit(struct shadow_memory *mem);

#endif /* MEM_ADDR_VALIDATOR */
