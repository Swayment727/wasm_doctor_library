#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "local_validator.h"

void
register_set(struct local_validator *validator, uint32_t local_idx)
{
        validator->locals[validator->frames_size - 1][local_idx] = true;
}

bool
validate_get(struct local_validator *validator, uint32_t local_idx)
{
        return validator->locals[validator->frames_size - 1][local_idx];
}

void
local_validator_frame_enter(struct local_validator *validator,
                            uint32_t locals_size)
{
        validator->locals = (bool **)realloc(
                validator->locals,
                ++validator->frames_size * sizeof(*validator->locals));
        validator->locals[validator->frames_size - 1] =
                (bool *)malloc(locals_size * sizeof(**validator->locals));

        for (uint32_t i = 0; i < locals_size; ++i) {
                validator->locals[validator->frames_size - 1][i] = false;
        }
}

void
local_validator_frame_exit(struct local_validator *validator)
{
        free(validator->locals[--validator->frames_size]);
}

void
local_validator_init(struct local_validator *validator,
                     struct error_reporter *reporter)
{
        validator->reporter = reporter;
}

void
local_validator_exit(struct local_validator *validator)
{
        free(validator->locals);
}
