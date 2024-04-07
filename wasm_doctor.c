#include <stdint.h>
#include <stdio.h>

#include "mem_addr_validator.h"
#include "wasm_doctor.h"

static bool initialized = false;
static struct shadow_memory mem;

static uintptr_t shadow_stack_pointer = UINT32_MAX;

void
move_shadow_stack_pointer(uintptr_t address)
{
        /* printf("move ssp\n"); */

        if (address > shadow_stack_pointer) {
                invalidate_region(&mem, shadow_stack_pointer, address);
        }

        shadow_stack_pointer = address;
}

void
register_store(uintptr_t address, uint64_t size)
{
        /* printf("register store at %lu of size %lu\n", address, size); */

        if (!initialized) {
                shadow_memory_init(&mem, 65536 * 10);
                initialized = true;
        }

        validate_region(&mem, address * 8, (address + size) * 8 - 1);
}

uint8_t
validate_load(uintptr_t address)
{
        /* printf("validate load\n"); */

        return is_valid_region(&mem, address * 8, (address + 1) * 8 - 1);
}
