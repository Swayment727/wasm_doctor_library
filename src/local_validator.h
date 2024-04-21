#ifndef LOCAL_VALIDATOR
#define LOCAL_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

struct local_validator {
        uint32_t frames_size;
        bool **locals;
};

void register_set(struct local_validator *validator, uint32_t local_idx);
bool validate_get(struct local_validator *validator, uint32_t local_idx);
void local_validator_frame_enter(struct local_validator *validator,
                                 uint32_t locals_size);
void local_validator_frame_exit(struct local_validator *validator);

#endif /* LOCAL_VALIDATOR */
