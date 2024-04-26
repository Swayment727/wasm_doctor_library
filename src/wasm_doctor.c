#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "local_validator.h"
#include "mem_addr_validator.h"
#include "wasm_doctor.h"

#define WASM_PAGE_SIZE 65536

struct error_reporter reporter;

struct mem_addr_validator mem_validator;
struct heap_use_validator heap_validator;
struct local_validator local_validator;

/**
 * Shadow stack pointer is initially set to the highest address. Because of
 * this the first call of move_shadow_stack_pointer function sets it to the
 * actual start of the shadow stack.
 */
wasmptr_t shadow_stack_pointer = UINT32_MAX;

/**
 * @brief Move the shadow stack pointer to its new position. If the stack size
 * decreases invalidate values in the region that is no longer part of the
 * shadow stack. This function is Clang/LLVM shadow stack pointer specific.
 * It is meant to be used with the $__stack_pointer global in WebAssembly.
 *
 * @param[in] address New address of the shadow stack pointer.
 */
void
move_shadow_stack_pointer(wasmptr_t address)
{
        if (address > shadow_stack_pointer) {
                invalidate_region(&mem_validator, shadow_stack_pointer * 8,
                                  address * 8);
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
doctor_store(wasmptr_t address, uint32_t size)
{
        printf("store from %u to %u\n", address * 8, address * 8 + size - 1);
        validate_region(&mem_validator, address * 8, address * 8 + size - 1);
}

/**
 * @param[in] address Address of the load.
 * @param[in] size Size of the load in bits.
 */
void
doctor_load(wasmptr_t address, uint32_t size)
{
        check_region_access(&mem_validator, address * 8,
                            address * 8 + size - 1);
}

void
doctor_register_malloc(wasmptr_t block_start, uint32_t size_in_bytes)
{
        register_malloc(&heap_validator, block_start, size_in_bytes);
}

void
doctor_register_free(wasmptr_t block_start)
{
        register_free(&heap_validator, block_start);
}

void
doctor_local_set(uint32_t idx)
{
        register_set(&local_validator, idx);
}

void
doctor_local_get(uint32_t idx)
{
        validate_get(&local_validator, idx);
}

void
doctor_frame_enter(uint32_t locals_size)
{
        local_validator_frame_enter(&local_validator, locals_size);
}

void
doctor_frame_exit(void)
{
        local_validator_frame_exit(&local_validator);
}

/**
 * @param[in] size_in_pages Size of WebAssembly memory in pages.
 */
void
doctor_init(uint32_t size_in_pages)
{
        shadow_memory_init(&mem_validator, WASM_PAGE_SIZE * size_in_pages * 8);
}

void
doctor_exit(void)
{
        shadow_memory_exit(&mem_validator);
        heap_use_validator_exit(&heap_validator);
        report(&reporter);
        reporter_exit(&reporter);
}
