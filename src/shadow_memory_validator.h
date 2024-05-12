#ifndef SHADOW_MEMORY_VALIDATOR
#define SHADOW_MEMORY_VALIDATOR

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "error_reporter.h"

typedef uint32_t word_t;

struct shadow_memory_validator {
        struct error_reporter *reporter;
        word_t *words;
};

void validate(struct shadow_memory_validator *validator, uint64_t bit_idx);
void invalidate(struct shadow_memory_validator *validator, uint64_t bit_idx);
void check_access(struct shadow_memory_validator *validator, uint64_t bit_idx);
void validate_region(struct shadow_memory_validator *validator, size_t address, size_t size_in_bytes);
void invalidate_region(struct shadow_memory_validator *validator, size_t address, size_t size_in_bytes);
void check_region_access(struct shadow_memory_validator *validator, size_t bit_idx_start, size_t size_in_bytes);
void shadow_memory_validator_init(struct shadow_memory_validator *validator, size_t mem_size,
                                  struct error_reporter *reporter);
void shadow_memory_validator_exit(struct shadow_memory_validator *validator);

#endif /* SHADOW_MEMORY_VALIDATOR */
