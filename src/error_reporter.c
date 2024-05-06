#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"
#include "wasm_state.h"
#include "wasm_types.h"

#define SET_FUNCTION_NAME(errors, errors_size, function_name)                                                          \
        {                                                                                                              \
                (*errors)[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50) + 1);           \
                strncpy((*errors)[*errors_size - 1].location.function_name, function_name,                             \
                        strnlen(function_name, 50) + 1);                                                               \
        }

static bool
is_dlmalloc(char *function_name)
{
        if (!strcmp(function_name, "dlmalloc")) {
                return true;
        }

        return false;
}

static bool
is_dlfree(char *function_name)
{
        if (!strcmp(function_name, "dlfree")) {
                return true;
        }

        return false;
}

static bool
is_start(char *function_name)
{
        if (!strcmp(function_name, "_start")) {
                return true;
        }

        return false;
}

static bool
is_stdio_exit(char *function_name)
{
        if (!strcmp(function_name, "__stdio_exit")) {
                return true;
        }

        return false;
}

static bool
is_isatty(char *function_name)
{
        if (!strcmp(function_name, "__isatty")) {
                return true;
        }

        return false;
}

static bool
is_writev(char *function_name)
{
        if (!strcmp(function_name, "writev")) {
                return true;
        }

        return false;
}

static bool
is_result_of_fwritex(struct error_reporter *reporter)
{
        for (uint32_t i = 1; i <= reporter->state->function_names_size; i++) {
                char *function_name = reporter->state->function_names[reporter->state->function_names_size - i];
                if (!strcmp(function_name, "__fwritex")) {
                        return true;
                }
        }

        return false;
}

static bool
is_blacklisted(char *function_name)
{
        return is_dlmalloc(function_name) || is_dlfree(function_name) || is_start(function_name) ||
               is_stdio_exit(function_name) || is_isatty(function_name) || is_writev(function_name);
}

static void
print_stack_trace(struct error_reporter *reporter)
{
        if (reporter->state->function_names_size >= 1) {
                printf("== Wasm Doctor == ");
                for (uint32_t i = reporter->state->function_names_size - 1; i > 0; --i) {
                        printf("%s <- ", reporter->state->function_names[i]);
                }
                printf("%s\n", reporter->state->function_names[0]);
                printf("== Wasm Doctor == \n");
        }
}

void
report(struct error_reporter *reporter)
{
        /* printf("============================= Wasm Doctor Report " */
        /*        "=============================\n\n"); */

        /* for (uint32_t i = 0; i < reporter->undefined_local_use_errors_size; ++i) { */
        /*         printf("Undefined local with index %u read. " */
        /*                "(%s)\n", */
        /*                reporter->undefined_local_use_errors[i].idx, */
        /*                reporter->undefined_local_use_errors[i].location.function_name); */
        /* } */

        /* if (reporter->undefined_local_use_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->undefined_memory_use_errors_size; ++i) { */
        /*         char *function_name = reporter->undefined_memory_use_errors[i].location.function_name; */

        /*         if (!is_dlmalloc(function_name) && !is_dlfree(function_name) && !is_start(function_name) && */
        /*             !is_stdio_exit(function_name)) { */
        /*                 printf("Undefined value of size %u bits read from address " */
        /*                        "%u. " */
        /*                        "(%s)\n", */
        /*                        reporter->undefined_memory_use_errors[i].size, */
        /*                        reporter->undefined_memory_use_errors[i].address, */
        /*                        reporter->undefined_memory_use_errors[i].location.function_name); */

        /*                 printf("validity: "); */
        /*                 for (uint32_t j = 0; j < reporter->undefined_memory_use_errors[i].size; ++j) { */
        /*                         printf("%u", reporter->undefined_memory_use_errors[i].validity[j]); */
        /*                 } */

        /*                 printf("\n"); */
        /*         } */
        /* } */

        /* if (reporter->undefined_memory_use_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->invalid_read_errors_size; ++i) { */
        /*         char *function_name = reporter->invalid_read_errors[i].location.function_name; */

        /*         if (!is_dlmalloc(function_name) && !is_dlfree(function_name) && !is_start(function_name) && */
        /*             !is_stdio_exit(function_name)) { */
        /*                 printf("Invalid read of size %u bits detected at address %u. (%s)\n", */
        /*                        reporter->invalid_read_errors[i].size, reporter->invalid_read_errors[i].address, */
        /*                        reporter->invalid_read_errors[i].location.function_name); */
        /*         } */
        /* } */

        /* if (reporter->invalid_read_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->invalid_write_errors_size; ++i) { */
        /*         char *function_name = reporter->invalid_write_errors[i].location.function_name; */

        /*         if (!is_dlmalloc(function_name) && !is_dlfree(function_name) && !is_start(function_name) && */
        /*             !is_stdio_exit(function_name)) { */
        /*                 printf("Invalid write of size %u bits detected at address %u. (%s)\n", */
        /*                        reporter->invalid_write_errors[i].size, reporter->invalid_write_errors[i].address, */
        /*                        reporter->invalid_write_errors[i].location.function_name); */
        /*         } */
        /* } */

        /* if (reporter->invalid_write_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->use_after_free_errors_size; ++i) { */
        /*         char *function_name = reporter->use_after_free_errors[i].location.function_name; */

        /*         if (!is_dlfree(function_name)) { */
        /*                 printf("Use after free of size %u bytes detected at address " */
        /*                        "%u. (%s)\n", */
        /*                        reporter->use_after_free_errors[i].size, reporter->use_after_free_errors[i].address *
         * 8, */
        /*                        reporter->use_after_free_errors[i].location.function_name); */
        /*         } */
        /* } */

        /* if (reporter->use_after_free_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->double_free_errors_size; ++i) { */
        /*         printf("Double free detected at address %u. (%s)\n", reporter->double_free_errors[i].address * 8, */
        /*                reporter->double_free_errors[i].location.function_name); */
        /* } */

        /* if (reporter->double_free_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->invalid_free_errors_size; ++i) { */
        /*         printf("Invalid free detected at address %u. (%s)\n", reporter->invalid_free_errors[i].address * 8,
         */
        /*                reporter->invalid_free_errors[i].location.function_name); */
        /* } */

        /* if (reporter->invalid_free_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* for (uint32_t i = 0; i < reporter->memory_leak_errors_size; ++i) { */
        /*         printf("Memory leak of size %u bytes detected at address " */
        /*                "%u. (%s)\n", */
        /*                reporter->memory_leak_errors[i].size, reporter->memory_leak_errors[i].address * 8, */
        /*                reporter->memory_leak_errors[i].location.function_name); */
        /* } */

        /* if (reporter->memory_leak_errors_size > 0) { */
        /*         printf("\n"); */
        /* } */

        /* printf("============================================================================" */
        /*        "\n"); */
}

void
add_undefined_memory_use(struct error_reporter *reporter, wasmptr_t address, uint8_t size_in_bytes, bool *validity,
                         char *function_name)
{
        struct undefined_memory_use **errors = &reporter->undefined_memory_use_errors;
        uint32_t *errors_size = &reporter->undefined_memory_use_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size_in_bytes;
        (*errors)[*errors_size - 1].validity =
                malloc(size_in_bytes * 8 * sizeof(*((*errors)[*errors_size - 1].validity)));
        memcpy((*errors)[*errors_size - 1].validity, validity, size_in_bytes * 8);

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Undefined value of size %u bits read from address "
                       "%u.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);

                printf("== Wasm Doctor == validity: ");
                for (uint32_t j = 0; j < (*errors)[*errors_size - 1].size * 8; ++j) {
                        printf("%u", (*errors)[*errors_size - 1].validity[j]);
                }
                printf("\n");

                print_stack_trace(reporter);
        }
}

void
add_undefined_local_use(struct error_reporter *reporter, uint32_t idx, char *function_name)
{
        struct undefined_local_use **errors = &reporter->undefined_local_use_errors;
        uint32_t *errors_size = &reporter->undefined_local_use_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].idx = idx;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name) && false) { // TODO: ignore?
                printf("== Wasm Doctor == Undefined local with index %u read.\n", (*errors)[*errors_size - 1].idx);
                print_stack_trace(reporter);
        }
}

void
add_use_after_free(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        struct use_after_free **errors = &reporter->use_after_free_errors;
        uint32_t *errors_size = &reporter->use_after_free_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Use after free of size %u bytes detected at address "
                       "%u.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_memory_leak(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        struct memory_leak **errors = &reporter->memory_leak_errors;
        uint32_t *errors_size = &reporter->memory_leak_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Memory leak of size %u bytes detected at address "
                       "%u.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_double_free(struct error_reporter *reporter, uint32_t address, char *function_name)
{
        struct double_free **errors = &reporter->double_free_errors;
        uint32_t *errors_size = &reporter->double_free_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Double free detected at address %u.\n",
                       (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_invalid_free(struct error_reporter *reporter, uint32_t address, char *function_name)
{
        struct invalid_free **errors = &reporter->invalid_free_errors;
        uint32_t *errors_size = &reporter->invalid_free_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;

        SET_FUNCTION_NAME(errors, errors_size, function_name);

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Invalid free detected at address %u.\n",
                       (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_invalid_read(struct error_reporter *reporter, doctorptr_t address, uint32_t size, char *function_name)
{
        struct invalid_read **errors = &reporter->invalid_read_errors;
        uint32_t *errors_size = &reporter->invalid_read_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name)) {
                printf("== Wasm Doctor == Invalid read of size %u bits detected at address %u.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);
                print_stack_trace(reporter);
        }
}

void
add_invalid_write(struct error_reporter *reporter, doctorptr_t address, uint32_t size, char *function_name)
{
        struct invalid_write **errors = &reporter->invalid_write_errors;
        uint32_t *errors_size = &reporter->invalid_write_errors_size;

        *errors = realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size;

        SET_FUNCTION_NAME(errors, errors_size, function_name)

        if (!is_blacklisted(function_name) && !is_result_of_fwritex(reporter)) {
                printf("== Wasm Doctor == Invalid write of size %u bits detected at address %u.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);
                print_stack_trace(reporter);
        }
}

void
reporter_init(struct error_reporter *reporter, struct wasm_state *state)
{
        reporter->undefined_memory_use_errors_size = 0;
        reporter->undefined_memory_use_errors = NULL;

        reporter->undefined_local_use_errors_size = 0;
        reporter->undefined_local_use_errors = NULL;

        reporter->use_after_free_errors_size = 0;
        reporter->use_after_free_errors = NULL;

        reporter->memory_leak_errors_size = 0;
        reporter->memory_leak_errors = NULL;

        reporter->double_free_errors_size = 0;
        reporter->double_free_errors = NULL;

        reporter->invalid_free_errors_size = 0;
        reporter->invalid_free_errors = NULL;

        reporter->invalid_read_errors_size = 0;
        reporter->invalid_read_errors = NULL;

        reporter->invalid_write_errors_size = 0;
        reporter->invalid_write_errors = NULL;

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

        for (uint32_t i = 0; i < reporter->invalid_free_errors_size; ++i) {
                free(reporter->invalid_free_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->invalid_read_errors_size; ++i) {
                free(reporter->invalid_read_errors[i].location.function_name);
        }

        for (uint32_t i = 0; i < reporter->invalid_write_errors_size; ++i) {
                free(reporter->invalid_write_errors[i].location.function_name);
        }

        free(reporter->undefined_memory_use_errors);
        free(reporter->undefined_local_use_errors);
        free(reporter->use_after_free_errors);
        free(reporter->memory_leak_errors);
        free(reporter->double_free_errors);
        free(reporter->invalid_free_errors);
        free(reporter->invalid_read_errors);
        free(reporter->invalid_write_errors);
}
