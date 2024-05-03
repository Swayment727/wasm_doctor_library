#ifndef SHADOW_STACK_VALIDATOR
#define SHADOW_STACK_VALIDATOR

#include "mem_addr_validator.h"
#include "wasm_types.h"

struct shadow_stack_validator {
        wasmptr_t shadow_stack_pointer_base;

        /**
         * Shadow stack pointer is initially set to the highest address. Because of
         * this the first call of move_shadow_stack_pointer function sets it to the
         * actual start of the shadow stack.
         */
        wasmptr_t shadow_stack_pointer;
};

void set_shadow_stack_pointer_base(struct shadow_stack_validator *validator, wasmptr_t address);
void move_shadow_stack_pointer(struct shadow_stack_validator *validator, struct mem_addr_validator *mem_validator,
                               wasmptr_t address);

#endif /* SHADOW_STACK_VALIDATOR */
