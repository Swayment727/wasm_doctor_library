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
        if (address > shadow_stack_pointer) {
                invalidate_region(&mem, shadow_stack_pointer * 8, address * 8);
                printf("move ssp - invalidate region from %lu to %lu\n",
                       shadow_stack_pointer, address);
        }

        shadow_stack_pointer = address;
}

void
register_store(uintptr_t address, uint64_t size)
{
        if (!initialized) {
                shadow_memory_init(&mem, 65536 * 10);
                initialized = true;
        }

        validate_region(&mem, address * 8, (address + size) * 8 - 1);
}

uint8_t
validate_load(uintptr_t address)
{
        return is_valid_region(&mem, address * 8, (address + 1) * 8 - 1);
}
