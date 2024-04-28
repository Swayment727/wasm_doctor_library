#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "local_validator.h"
#include "mem_addr_validator.h"
#include "wasm_state.h"
#include "wasm_types.h"

struct wasm_doctor {
        struct error_reporter reporter;
        struct wasm_state state;

        struct mem_addr_validator mem_validator;
        struct heap_use_validator heap_validator;
        struct local_validator local_validator;
};

void move_shadow_stack_pointer(wasmptr_t address);
void doctor_store(wasmptr_t address, uint32_t bit_size);
void doctor_load(wasmptr_t address, uint32_t bit_size);
void doctor_register_malloc(wasmptr_t block_start, uint32_t size_in_bytes);
void doctor_register_free(wasmptr_t block_start);
void doctor_local_set(uint32_t idx);
void doctor_local_get(uint32_t idx);
void doctor_frame_enter(uint32_t locals_size, char *function_name);
void doctor_frame_exit(void);
void doctor_init(uint32_t size_in_pages);
void doctor_exit(void);

#endif /* WASM_DOCTOR */
