#include "shadow_stack_validator.h"
#include "mem_addr_validator.h"

void
set_shadow_stack_pointer_base(struct shadow_stack_validator *validator, size_t address)
{
        validator->shadow_stack_pointer_base = address;
        validator->shadow_stack_pointer = address;
}

void
move_shadow_stack_pointer(struct shadow_stack_validator *validator, struct mem_addr_validator *mem_validator,
                          size_t address)
{
        if (validator->shadow_stack_pointer_base == UINT32_MAX) {
                validator->shadow_stack_pointer_base = address;
        }

        if (address > validator->shadow_stack_pointer) {
                invalidate_region(mem_validator, validator->shadow_stack_pointer,
                                  address - validator->shadow_stack_pointer);
        }

        validator->shadow_stack_pointer = address;
}

void
shadow_stack_validator_init(struct shadow_stack_validator *validator)
{
        validator->shadow_stack_pointer_base = UINT32_MAX;
        validator->shadow_stack_pointer = UINT32_MAX;
}
