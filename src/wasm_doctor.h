#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "local_validator.h"
#include "mem_addr_validator.h"
#include "shadow_stack_validator.h"
#include "wasm_state.h"
#include "wasm_types.h"

struct wasm_doctor {
        struct error_reporter reporter;
        struct wasm_state state;

        struct shadow_stack_validator shadow_stack_validator;
        struct mem_addr_validator mem_validator;
        struct heap_use_validator heap_validator;
        struct local_validator local_validator;
};

void doctor_set_shadow_stack_pointer_base(wasmptr_t address);
void doctor_move_shadow_stack_pointer(wasmptr_t address);
void doctor_global_data_validate(wasmptr_t address, uint32_t bit_size);
void doctor_store(wasmptr_t address, uint32_t bit_size);
void doctor_load(wasmptr_t address, uint32_t bit_size);
void doctor_register_malloc(wasmptr_t block_start, uint32_t size_in_bytes);
void doctor_register_free(wasmptr_t block_start);
void doctor_local_set(uint32_t idx);
void doctor_local_get(uint32_t idx);
void doctor_frame_enter(uint32_t locals_size, char *function_name);
void doctor_frame_exit(void);
void doctor_init(struct wasm_doctor *wasm_doctor, uint32_t size_in_pages);
void doctor_report(void);
void doctor_reporter_exit(void);
void doctor_exit(bool exit_reporter);

#endif /* WASM_DOCTOR */
