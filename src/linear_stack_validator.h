#ifndef LINEAR_STACK_VALIDATOR
#define LINEAR_STACK_VALIDATOR

#include <stddef.h>

#include "shadow_memory_validator.h"

struct linear_stack_validator {
        size_t linear_stack_pointer_base;

        /**
         * Linear stack pointer is initially set to the highest address. Because of
         * this the first call of move_linear_stack_pointer function sets it to the
         * actual start of the linear stack.
         */
        size_t linear_stack_pointer;
};

void set_linear_stack_pointer_base(struct linear_stack_validator *validator, size_t address);
void move_linear_stack_pointer(struct linear_stack_validator *validator,
                               struct shadow_memory_validator *shadow_memory_validator, size_t address);
void linear_stack_validator_init(struct linear_stack_validator *validator);

#endif /* LINEAR_STACK_VALIDATOR */
