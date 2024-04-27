#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"
#include "wasm_state.h"

void
report(struct error_reporter *reporter)
{
        printf("==================== Wasm Doctor Report "
               "====================\n");

        for (uint32_t i = 0; i < reporter->undefined_memory_use_errors_size; ++i) {
                printf("Undefined value of size %u bytes read from address "
                       "%u. "
                       "(%s)\n",
                       reporter->undefined_memory_use_errors[i].size, reporter->undefined_memory_use_errors[i].address,
                       reporter->undefined_memory_use_errors[i].location.function_name);

                printf("validity: ");
                for (uint32_t j = 0; j < reporter->undefined_memory_use_errors[i].size; ++j) {
                        printf("%u", reporter->undefined_memory_use_errors[i].validity[j]);
                }

                printf("\n\n");
        }

        for (uint32_t i = 0; i < reporter->undefined_local_use_errors_size; ++i) {
                printf("Undefined local with index %u read. "
                       "(%s)\n",
                       reporter->undefined_local_use_errors[i].idx,
                       reporter->undefined_local_use_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->use_after_free_errors_size; ++i) {
                printf("Use after free of size %u bytes detected at address "
                       "%u. (%s)\n",
                       reporter->use_after_free_errors[i].size, reporter->use_after_free_errors[i].address,
                       reporter->use_after_free_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->memory_leak_errors_size; ++i) {
                printf("Memory leak of size %u bytes detected at address "
                       "%u. (%s)\n",
                       reporter->memory_leak_errors[i].size, reporter->memory_leak_errors[i].address,
                       reporter->memory_leak_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->double_free_errors_size; ++i) {
                printf("Double free detected at address %u. (%s)\n", reporter->double_free_errors[i].address,
                       reporter->double_free_errors[i].location.function_name);
        }

        printf("============================================================"
               "\n");
}

void
add_undefined_memory_use(struct error_reporter *reporter, uint32_t address, uint32_t size, bool *validity,
                         char *function_name)
{
        reporter->undefined_memory_use_errors =
                realloc(reporter->undefined_memory_use_errors, ++(reporter->undefined_memory_use_errors_size) *
                                                                       sizeof(*reporter->undefined_memory_use_errors));
        reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1].address = address;
        reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1].size = size;
        reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1].validity = malloc(
                size * sizeof(*(reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1]
                                        .validity)));
        memcpy(reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1].validity, validity,
               size);

        reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1].location.function_name =
                malloc(strnlen(function_name, 50) + 1);
        strncpy(reporter->undefined_memory_use_errors[reporter->undefined_memory_use_errors_size - 1]
                        .location.function_name,
                function_name, strnlen(function_name, 50) + 1); // TODO: decide strncpy vs strcpy
}

void
add_undefined_local_use(struct error_reporter *reporter, uint32_t idx, char *function_name)
{
        reporter->undefined_local_use_errors =
                realloc(reporter->undefined_local_use_errors,
                        ++(reporter->undefined_local_use_errors_size) * sizeof(*reporter->undefined_local_use_errors));
        reporter->undefined_local_use_errors[reporter->undefined_local_use_errors_size - 1].idx = idx;

        reporter->undefined_local_use_errors[reporter->undefined_local_use_errors_size - 1].location.function_name =
                malloc(strnlen(function_name, 50) + 1);
        strncpy(reporter->undefined_local_use_errors[reporter->undefined_local_use_errors_size - 1]
                        .location.function_name,
                function_name, strnlen(function_name, 50) + 1); // TODO: decide strncpy vs strcpy
}

void
add_use_after_free(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        reporter->use_after_free_errors =
                realloc(reporter->use_after_free_errors,
                        ++(reporter->use_after_free_errors_size) * sizeof(*reporter->use_after_free_errors));
        reporter->use_after_free_errors[reporter->use_after_free_errors_size - 1].address = address;
        reporter->use_after_free_errors[reporter->use_after_free_errors_size - 1].size = size;

        reporter->use_after_free_errors[reporter->use_after_free_errors_size - 1].location.function_name =
                malloc(strnlen(function_name, 50) + 1);
        strncpy(reporter->use_after_free_errors[reporter->use_after_free_errors_size - 1].location.function_name,
                function_name, strnlen(function_name, 50) + 1); // TODO: decide strncpy vs strcpy
}

void
add_memory_leak(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        reporter->memory_leak_errors =
                realloc(reporter->memory_leak_errors,
                        ++(reporter->memory_leak_errors_size) * sizeof(*reporter->memory_leak_errors));
        reporter->memory_leak_errors[reporter->memory_leak_errors_size - 1].address = address;
        reporter->memory_leak_errors[reporter->memory_leak_errors_size - 1].size = size;

        reporter->memory_leak_errors[reporter->memory_leak_errors_size - 1].location.function_name =
                malloc(strnlen(function_name, 50) + 1);
        strncpy(reporter->memory_leak_errors[reporter->memory_leak_errors_size - 1].location.function_name,
                function_name, strnlen(function_name, 50) + 1); // TODO: decide strncpy vs strcpy
}

void
add_doubled_free(struct error_reporter *reporter, uint32_t address, char *function_name)
{
        reporter->double_free_errors =
                realloc(reporter->double_free_errors,
                        ++(reporter->double_free_errors_size) * sizeof(*reporter->double_free_errors));
        reporter->double_free_errors[reporter->double_free_errors_size - 1].address = address;

        reporter->double_free_errors[reporter->double_free_errors_size - 1].location.function_name =
                malloc(strnlen(function_name, 50) + 1);
        strncpy(reporter->double_free_errors[reporter->double_free_errors_size - 1].location.function_name,
                function_name, strnlen(function_name, 50) + 1); // TODO: decide strncpy vs strcpy
}

void
reporter_init(struct error_reporter *reporter, struct wasm_state *state)
{
        reporter->state = state;
}

void
reporter_exit(struct error_reporter *reporter)
{
        for (uint32_t i = 0; i < reporter->undefined_memory_use_errors_size; ++i) {
                free(reporter->undefined_memory_use_errors[i].location.function_name);
                free(reporter->undefined_memory_use_errors[i].validity);
        }

        for (uint32_t i = 0; i < reporter->undefined_local_use_errors_size; ++i) {
                free(reporter->undefined_local_use_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->use_after_free_errors_size; ++i) {
                free(reporter->use_after_free_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->memory_leak_errors_size; ++i) {
                free(reporter->memory_leak_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->double_free_errors_size; ++i) {
                free(reporter->double_free_errors[i].location.function_name);
        }

        free(reporter->undefined_memory_use_errors);
        free(reporter->undefined_local_use_errors);
        free(reporter->use_after_free_errors);
        free(reporter->memory_leak_errors);
        free(reporter->double_free_errors);

        wasm_state_exit(reporter->state);
}
