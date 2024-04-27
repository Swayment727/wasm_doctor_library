#ifndef WASM_STATE
#define WASM_STATE

#include <stdint.h>

struct wasm_state {
        uint32_t function_names_size;
        char **function_names;
        uint32_t bit_size;
};

void enter_function(struct wasm_state *state, char *function_name);
void exit_function(struct wasm_state *state);
void set_bit_size(struct wasm_state *state, uint32_t bit_size);
void wasm_state_exit(struct wasm_state *state);

#endif /* WASM_STATE */
