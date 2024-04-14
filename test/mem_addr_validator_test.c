#include "mem_addr_validator.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MEM_SIZE 10
#define VALID_IDX 2
#define INVALID_IDX 4

struct shadow_memory mem;

void
test_shadow_memory_init(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == false);
        }
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);

        shadow_memory_exit(&mem);
}

void
test_is_valid_region(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == true);
        }
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == true);

        shadow_memory_exit(&mem);

        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate_region(&mem, 0, MEM_SIZE - 1);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(is_valid(&mem, i) == false);
        }
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);

        shadow_memory_exit(&mem);
}

void
test_validate_region(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == true);

        shadow_memory_exit(&mem);
}

void
test_invalidate_region(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate_region(&mem, 0, MEM_SIZE - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE - 1) == false);

        shadow_memory_exit(&mem);
}

void
test_validate(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate(&mem, 0);
        assert(is_valid(&mem, 0) == true);

        assert(is_valid(&mem, 1) == false);
        assert(is_valid(&mem, 2) == false);

        validate(&mem, 3);
        assert(is_valid(&mem, 3) == true);

        assert(is_valid(&mem, 4) == false);

        shadow_memory_exit(&mem);
}

void
test_invalidate(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate(&mem, 0);
        assert(is_valid(&mem, 0) == false);
        assert(is_valid(&mem, 1) == true);

        shadow_memory_exit(&mem);
}

void
test_partial_validation(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE / 2 - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE / 2 - 1) == true);
        assert(is_valid_region(&mem, MEM_SIZE / 2, MEM_SIZE - 1) == false);

        shadow_memory_exit(&mem);
}

void
test_partial_invalidation(void)
{
        shadow_memory_init(&mem, MEM_SIZE);

        validate_region(&mem, 0, MEM_SIZE - 1);
        invalidate_region(&mem, MEM_SIZE / 2, MEM_SIZE - 1);
        assert(is_valid_region(&mem, 0, MEM_SIZE / 2 - 1) == true);
        assert(is_valid_region(&mem, MEM_SIZE / 2, MEM_SIZE - 1) == false);

        shadow_memory_exit(&mem);
}

int
main(void)
{
        test_shadow_memory_init();
        test_is_valid_region();
        test_validate_region();
        test_invalidate_region();
        test_validate();
        test_invalidate();
        test_partial_validation();
        test_partial_invalidation();

        printf("mem_addr_validator tests successfull\n");
        return 0;
}
