#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t wasmptr_t;

void move_shadow_stack_pointer(wasmptr_t address);
void shadow_store(wasmptr_t address, uint32_t size);
bool shadow_load(wasmptr_t address, uint32_t size);
void doctor_init(uint32_t size_in_pages);
void doctor_exit(void);

#endif /* WASM_DOCTOR */
