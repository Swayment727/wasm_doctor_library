#include "zero_address_access_validator.h"
#include "error_reporter.h"

void
check_zero_address(struct zero_address_access_validator *validator, wasmptr_t address)
{
        if (address == 0) {
                add_zero_address_access(validator->reporter,
                                        validator->reporter->state
                                                ->function_names[validator->reporter->state->function_names_size - 1]);
        }
}

void
zero_address_access_validator_init(struct zero_address_access_validator *validator, struct error_reporter *reporter)
{
        validator->reporter = reporter;
}
