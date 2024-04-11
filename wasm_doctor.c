#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "mem_addr_validator.h"
#include "wasm_doctor.h"

#define WASM_PAGE_SIZE 65536

static struct shadow_memory mem;

static wasmptr_t shadow_stack_pointer = UINT32_MAX;

/**
 * @param[in] address New address of the shadow stack pointer.
 */
void
move_shadow_stack_pointer(wasmptr_t address)
{
        if (address > shadow_stack_pointer) {
                invalidate_region(&mem, shadow_stack_pointer * 8, address * 8);
                printf("move ssp - invalidate region from %u to %u\n",
                       shadow_stack_pointer, address);
        }

        shadow_stack_pointer = address;
}

/**
 * @param[in] address Address of the store.
 * @param[in] size Size of the store in bits.
 */
void
shadow_store(wasmptr_t address, uint32_t size)
{
        printf("store from %u to %u\n", address * 8, address * 8 + size - 1);
        validate_region(&mem, address * 8, address * 8 + size - 1);
}

/**
 * @param[in] address Address of the load.
 * @param[in] size Size of the load in bits.
 */
bool
shadow_load(wasmptr_t address, uint32_t size)
{
        printf("load from %u to %u\n", address * 8, address * 8 + size - 1);
        return is_valid_region(&mem, address * 8, address * 8 + size - 1);
}

/**
 * @param[in] num_of_pages Size of WebAssembly memory in pages.
 */
void
doctor_init(uint32_t num_of_pages)
{
        shadow_memory_init(&mem, WASM_PAGE_SIZE * num_of_pages * 8);
}
