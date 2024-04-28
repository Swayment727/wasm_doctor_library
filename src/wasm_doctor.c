#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "local_validator.h"
#include "mem_addr_validator.h"
#include "wasm_doctor.h"
#include "wasm_state.h"
#include "wasm_types.h"

#define WASM_PAGE_SIZE 65536

struct wasm_doctor *doctor;

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
                invalidate_region(&doctor->mem_validator, shadow_stack_pointer * 8, address * 8);
                printf("move ssp - invalidate region from %u to %u\n", shadow_stack_pointer, address);
        }

        shadow_stack_pointer = address;
}

/**
 * @param[in] address Address of the store.
 * @param[in] bit_size Size of the store in bits.
 */
void
doctor_store(wasmptr_t address, uint32_t bit_size)
{
        set_bit_size(doctor->reporter.state, bit_size);
        validate_region(&doctor->mem_validator, address * 8, address * 8 + bit_size - 1);
        check_use_after_free(&doctor->heap_validator, address * 8, bit_size);
}

/**
 * @param[in] address Address of the load.
 * @param[in] bit_size Size of the load in bits.
 */
void
doctor_load(wasmptr_t address, uint32_t bit_size)
{
        set_bit_size(doctor->reporter.state, bit_size);
        check_region_access(&doctor->mem_validator, address * 8, address * 8 + bit_size - 1);
        check_use_after_free(&doctor->heap_validator, address * 8, bit_size);
}

void
doctor_register_malloc(wasmptr_t block_start, uint32_t size_in_bytes)
{
        register_malloc(&doctor->heap_validator, block_start, size_in_bytes);
}

void
doctor_register_free(wasmptr_t block_start)
{
        register_free(&doctor->heap_validator, block_start);
}

void
doctor_local_set(uint32_t idx)
{
        register_set(&doctor->local_validator, idx);
}

void
doctor_local_get(uint32_t idx)
{
        validate_get(&doctor->local_validator, idx);
}

void
doctor_frame_enter(uint32_t locals_size, char *function_name)
{
        enter_function(doctor->reporter.state, function_name);
        local_validator_frame_enter(&doctor->local_validator, locals_size);
}

void
doctor_frame_exit(void)
{
        exit_function(doctor->reporter.state);
        local_validator_frame_exit(&doctor->local_validator);
}

/**
 * @param[in] size_in_pages Size of WebAssembly memory in pages.
 */
void
doctor_init(struct wasm_doctor *wasm_doctor, uint32_t size_in_pages)
{
        doctor = wasm_doctor;
        wasm_state_init(&doctor->state);
        reporter_init(&doctor->reporter, &doctor->state);
        local_validator_init(&doctor->local_validator, &doctor->reporter);
        heap_use_validator_init(&doctor->heap_validator, &doctor->reporter);
        mem_addr_validator_init(&doctor->mem_validator, WASM_PAGE_SIZE * size_in_pages * 8, &doctor->reporter);
}

void
doctor_report(void)
{
        report(&doctor->reporter);
}

void
doctor_reporter_exit(void)
{
        reporter_exit(&doctor->reporter);
        wasm_state_exit(&doctor->state);
}

void
doctor_exit(bool exit_reporter)
{
        mem_addr_validator_exit(&doctor->mem_validator);
        heap_use_validator_exit(&doctor->heap_validator);
        local_validator_exit(&doctor->local_validator);

        if (exit_reporter) {
                doctor_reporter_exit();
        }
}
