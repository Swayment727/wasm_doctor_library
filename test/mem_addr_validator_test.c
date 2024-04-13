#include "mem_addr_validator.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MEM_SIZE 10
#define VALID_IDX 2
#define INVALID_IDX 4

struct shadow_memory mem;

void
is_valid_after_init()
{
        shadow_memory_init(&mem, MEM_SIZE);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == false);
        }
}

void
is_valid_region_after_init()
{
        shadow_memory_init(&mem, MEM_SIZE);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);
}

void
is_valid_after_validate()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate(&mem, VALID_IDX);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                if (i != VALID_IDX) {
                        assert(is_valid(&mem, i) == false);
                } else {
                        assert(is_valid(&mem, i) == true);
                }
        }
}

void
is_valid_region_after_validate()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate(&mem, VALID_IDX);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);
}

void
is_valid_after_all_validated()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate_region(&mem, 0, MEM_SIZE - 1);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == true);
        }
}

void
is_valid_region_after_all_validated()
{
        shadow_memory_init(&mem, MEM_SIZE);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                validate(&mem, i);
        }
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == true);
}

void
is_valid_after_all_validated_by_validate_all()
{
        shadow_memory_init(&mem, MEM_SIZE);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                validate(&mem, i);
        }
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == true);
        }
}

void
is_valid_region_after_all_validated_by_validate_all()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate_region(&mem, 0, MEM_SIZE - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == true);
}

void
is_invalid_after_invalidate()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate(&mem, INVALID_IDX);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                if (i != INVALID_IDX) {
                        assert(is_valid(&mem, i) == true);
                } else {
                        assert(is_valid(&mem, i) == false);
                }
        }
}

void
is_invalid_after_invalidated_by_invalidate_all()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate_region(&mem, 0, MEM_SIZE - 1);
        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == false);
        }
}

void
is_invalid_region_after_invalidate()
{
        shadow_memory_init(&mem, MEM_SIZE);
        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate_region(&mem, 0, MEM_SIZE - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);
}

int
main()
{
        is_valid_after_init();
        is_valid_region_after_init();

        is_valid_after_validate();
        is_valid_region_after_validate();

        is_valid_after_all_validated();
        is_valid_region_after_all_validated();
        is_valid_after_all_validated_by_validate_all();
        is_valid_region_after_all_validated_by_validate_all();

        is_invalid_after_invalidate();
        is_invalid_region_after_invalidate();

        is_invalid_after_invalidated_by_invalidate_all();

        printf("tests successfull\n");
        return 0;
}
