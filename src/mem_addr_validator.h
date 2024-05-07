#ifndef MEM_ADDR_VALIDATOR
#define MEM_ADDR_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"

typedef uint32_t word_t;

struct mem_addr_validator {
        struct error_reporter *reporter;
        word_t *words;
};

void validate(struct mem_addr_validator *validator, uint64_t bit_idx);
void invalidate(struct mem_addr_validator *validator, uint64_t bit_idx);
void check_access(struct mem_addr_validator *validator, uint64_t bit_idx);
void validate_region(struct mem_addr_validator *validator, size_t address, size_t size_in_bytes);
void invalidate_region(struct mem_addr_validator *validator, size_t address, size_t size_in_bytes);
void check_region_access(struct mem_addr_validator *validator, size_t bit_idx_start, size_t size_in_bytes);
void mem_addr_validator_init(struct mem_addr_validator *validator, size_t mem_size, struct error_reporter *reporter);
void mem_addr_validator_exit(struct mem_addr_validator *validator);

#endif /* MEM_ADDR_VALIDATOR */
