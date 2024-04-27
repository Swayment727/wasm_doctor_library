#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdbool.h>
#include <stdint.h>

#include "wasm_types.h"

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
