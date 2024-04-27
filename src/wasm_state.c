#include <stdlib.h>
#include <string.h>

#include "wasm_state.h"

void
enter_function(struct wasm_state *state, char *function_name)
{
        state->function_names =
                realloc(state->function_names, ++state->function_names_size * sizeof(*state->function_names));
        state->function_names[state->function_names_size - 1] =
                (char *)malloc(strlen(function_name) + 1); // TODO: check strnlen
        strcpy(state->function_names[state->function_names_size - 1], function_name);
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
wasm_state_exit(struct wasm_state *state)
{
        free(state->function_names);
}
