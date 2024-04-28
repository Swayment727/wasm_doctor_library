#include <stdlib.h>
#include <string.h>

#include "wasm_state.h"

void
enter_function(struct wasm_state *state, char *function_name)
{
        state->function_names =
                realloc(state->function_names, ++state->function_names_size * sizeof(*state->function_names));
        state->function_names[state->function_names_size - 1] =
                (char *)malloc(strnlen(function_name, 50) + 1); // TODO: check strnlen
        strncpy(state->function_names[state->function_names_size - 1], function_name, strnlen(function_name, 50) + 1);
}

void
exit_function(struct wasm_state *state)
{
        free(state->function_names[--state->function_names_size]);
}

void
set_bit_size(struct wasm_state *state, uint32_t bit_size)
{
        state->bit_size = bit_size;
}

void
wasm_state_init(struct wasm_state *state)
{
        state->function_names_size = 0;
        state->function_names = NULL;
}

void
wasm_state_exit(struct wasm_state *state)
{
        while (state->function_names_size > 0) {
                free(state->function_names[--state->function_names_size]);
        }

        free(state->function_names);
}
