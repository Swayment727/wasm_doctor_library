#include "mem_addr_validator.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MEM_SIZE 10
#define VALID_IDX 2
#define INVALID_IDX 4

struct mem_addr_validator mem_validator;

void
test_shadow_memory_init(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(check_access(&mem_validator, i) == false);
        }
        assert(check_region_access(&mem_validator, 0, MEM_SIZE - 1) == false);

        mem_addr_validator_exit(&mem_validator);
}

void
test_is_valid_region(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(check_access(&mem_validator, i) == true);
        }
        assert(check_region_access(&mem_validator, 0, MEM_SIZE - 1) == true);

        mem_addr_validator_exit(&mem_validator);

        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);
        invalidate_region(&mem_validator, 0, MEM_SIZE - 1);

        for (uint32_t i = 0; i < MEM_SIZE; ++i) {
                assert(check_access(&mem_validator, i) == false);
        }
        assert(check_region_access(&mem_validator, 0, MEM_SIZE - 1) == false);

        mem_addr_validator_exit(&mem_validator);
}

void
test_validate_region(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);
        assert(check_region_access(&mem_validator, 0, MEM_SIZE - 1) == true);

        mem_addr_validator_exit(&mem_validator);
}

void
test_invalidate_region(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);
        invalidate_region(&mem_validator, 0, MEM_SIZE - 1);
        assert(check_region_access(&mem_validator, 0, MEM_SIZE - 1) == false);

        mem_addr_validator_exit(&mem_validator);
}

void
test_validate(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        validate(&mem_validator, 0);
        assert(check_access(&mem_validator, 0) == true);

        assert(check_access(&mem_validator, 1) == false);
        assert(check_access(&mem_validator, 2) == false);

        validate(&mem_validator, 3);
        assert(check_access(&mem_validator, 3) == true);

        assert(check_access(&mem_validator, 4) == false);

        mem_addr_validator_exit(&mem_validator);
}

void
test_invalidate(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);
        invalidate(&mem_validator, 0);
        assert(check_access(&mem_validator, 0) == false);
        assert(check_access(&mem_validator, 1) == true);

        mem_addr_validator_exit(&mem_validator);
}

void
test_partial_validation(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE / 2 - 1);
        assert(check_region_access(&mem_validator, 0, MEM_SIZE / 2 - 1) == true);
        assert(check_region_access(&mem_validator, MEM_SIZE / 2, MEM_SIZE - 1) == false);

        mem_addr_validator_exit(&mem_validator);
}

void
test_partial_invalidation(void)
{
        mem_addr_validator_init(&mem_validator, MEM_SIZE);

        check_region(&mem_validator, 0, MEM_SIZE - 1);
        invalidate_region(&mem_validator, MEM_SIZE / 2, MEM_SIZE - 1);
        assert(check_region_access(&mem_validator, 0, MEM_SIZE / 2 - 1) == true);
        assert(check_region_access(&mem_validator, MEM_SIZE / 2, MEM_SIZE - 1) == false);

        mem_addr_validator_exit(&mem_validator);
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
