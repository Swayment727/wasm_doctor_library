#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "local_validator.h"
#include "mem_addr_validator.h"
#include "shadow_stack_validator.h"
#include "wasm_doctor.h"
#include "wasm_state.h"
#include "wasm_types.h"

#define WASM_PAGE_SIZE 65536

struct wasm_doctor *doctor;

/**
 * Set the shadow stack pointer base to position defined in WebAssembly module.
 * This function is Clang/LLVM shadow stack pointer specific.
 * It is meant to be used with the $__stack_pointer global in WebAssembly.
 *
 * @param[in] address Address of the shadow stack pointer base.
 */
void
doctor_set_shadow_stack_pointer_base(wasmptr_t address)
{
        set_shadow_stack_pointer_base(&doctor->shadow_stack_validator, address);
}

/**
 * Move the shadow stack pointer to its new position.
 * If the stack size decreases invalidate values in the region that is no longer part of the shadow stack.
 * This function is Clang/LLVM shadow stack pointer specific.
 * It is meant to be used with the $__stack_pointer global in WebAssembly.
 *
 * @param[in] address New address of the shadow stack pointer.
 */
void
doctor_move_shadow_stack_pointer(wasmptr_t address)
{
        move_shadow_stack_pointer(&doctor->shadow_stack_validator, &doctor->mem_validator, address);
}

/**
 * @param[in] address Address of the global data to be validated.
 * @param[in] size_in_bytes Size of the global data to be validated in bytes.
 */
void
doctor_global_data_validate(wasmptr_t address, uint32_t size_in_bytes)
{
        validate_region(&doctor->mem_validator, address, size_in_bytes);
        register_global_data(&doctor->heap_validator, address, size_in_bytes);
}

/**
 * @param[in] address Address of the store.
 * @param[in] size_in_bytes Size of the store in bytes.
 */
void
doctor_store(wasmptr_t address, uint8_t size_in_bytes)
{
        set_byte_size(doctor->reporter.state, size_in_bytes);
        check_use_after_free(&doctor->heap_validator, address, size_in_bytes);
        check_write_validity(&doctor->heap_validator, address, size_in_bytes);
        validate_region(&doctor->mem_validator, address, size_in_bytes);
}

/**
 * @param[in] address Address of the load.
 * @param[in] size_in_bytes Size of the load in bytes.
 */
void
doctor_load(wasmptr_t address, uint8_t size_in_bytes)
{
        set_byte_size(doctor->reporter.state, size_in_bytes);
        check_use_after_free(&doctor->heap_validator, address, size_in_bytes);
        check_read_validity(&doctor->heap_validator, address, size_in_bytes);
        check_region_access(&doctor->mem_validator, address, size_in_bytes);
}

/**
 * @param[in] block_start Start address of the allocated block.
 * @param[in] size_in_bytes Size of the allocated block.
 */
void
doctor_register_malloc(wasmptr_t block_start, uint32_t size_in_bytes)
{
        register_malloc(&doctor->heap_validator, block_start, size_in_bytes);
}

/**
 * @param[in] block_start Start address of the allocated block.
 */
void
doctor_register_free(wasmptr_t block_start)
{
        register_free(&doctor->heap_validator, block_start);
}

/**
 * @param[in] idx Index of the local which is being set by the local.set WebAssembly instruction.
 */
void
doctor_local_set(uint32_t idx)
{
        register_set(&doctor->local_validator, idx);
}

/**
 * @param[in] idx Index of the local which is accessed by the local.get WebAssembly instruction.
 */
void
doctor_local_get(uint32_t idx)
{
        validate_get(&doctor->local_validator, idx);
}

/**
 * @param[in] locals_size The number of locals a WebAssembly function (frame) has.
 * @param[in] function_name The name of function that is being entered.
 */
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
 * @param[in, out] wasm_doctor Wasm Doctor struct containing all validators and reports.
 * @param[in] size_in_pages Size of WebAssembly memory in pages.
 */
void
doctor_init(struct wasm_doctor *wasm_doctor, uint32_t size_in_pages)
{
        doctor = wasm_doctor;
        wasm_state_init(&doctor->state);
        reporter_init(&doctor->reporter, &doctor->state);
        local_validator_init(&doctor->local_validator, &doctor->reporter);
        heap_use_validator_init(&doctor->heap_validator, &doctor->shadow_stack_validator, &doctor->reporter);
        mem_addr_validator_init(&doctor->mem_validator, WASM_PAGE_SIZE * size_in_pages, &doctor->reporter);
        shadow_stack_validator_init(&doctor->shadow_stack_validator);

        enter_function(doctor->reporter.state, "module");
}

/**
 * Print error report.
 */
void
doctor_report(void)
{
        report(&doctor->reporter);
}

/**
 * Exit reporter if it was not previously done by doctor_exit. This may happens in case the reports are needed after the
 * analysis has already stopped.
 */
void
doctor_reporter_exit(void)
{
        reporter_exit(&doctor->reporter);
        wasm_state_exit(&doctor->state);
}

/**
 * @param[in] exit_reporter Boolean value decidig whether to call reporter_exit or keep the reports for further use
 * (i.e. testing).
 */
void
doctor_exit(bool exit_reporter)
{
        mem_addr_validator_exit(&doctor->mem_validator);
        heap_use_validator_exit(&doctor->heap_validator);
        local_validator_exit(&doctor->local_validator);

        if (exit_reporter) {
                report(&doctor->reporter);
                doctor_reporter_exit();
        }
}
