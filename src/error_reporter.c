#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"

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
        }

        for (uint32_t i = 0; i < reporter->undefined_local_use_errors_size; ++i) {
                printf("Undefined value of size %u bytes read from local %u. "
                       "(%s)\n",
                       reporter->undefined_local_use_errors[i].size, reporter->undefined_local_use_errors[i].idx,
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
add_undefined_memory_use(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        struct undefined_memory_use *errors = reporter->undefined_memory_use_errors;
        uint32_t *errors_size = &reporter->undefined_memory_use_errors_size;

        errors = realloc(errors, ++(*errors_size) * sizeof(*errors));
        errors[*errors_size - 1].address = address;
        errors[*errors_size - 1].size = size;

        errors[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50));
        strncpy(errors[*errors_size - 1].location.function_name, function_name,
                strnlen(function_name, 50)); // TODO: decide strncpy vs strcpy
}

void
add_undefined_local_use(struct error_reporter *reporter, uint32_t idx, uint32_t size, char *function_name)
{
        struct undefined_local_use *errors = reporter->undefined_local_use_errors;
        uint32_t *errors_size = &reporter->undefined_local_use_errors_size;

        errors = realloc(errors, ++(*errors_size) * sizeof(*errors));
        errors[*errors_size - 1].idx = idx;
        errors[*errors_size - 1].size = size;

        errors[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50));
        strncpy(errors[*errors_size - 1].location.function_name, function_name,
                strnlen(function_name, 50)); // TODO: decide strncpy vs strcpy
}

void
add_use_after_free(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        struct use_after_free *errors = reporter->use_after_free_errors;
        uint32_t *errors_size = &reporter->use_after_free_errors_size;

        errors = realloc(errors, ++(*errors_size) * sizeof(*errors));
        errors[*errors_size - 1].address = address;
        errors[*errors_size - 1].size = size;

        errors[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50));
        strncpy(errors[*errors_size - 1].location.function_name, function_name,
                strnlen(function_name, 50)); // TODO: decide strncpy vs strcpy
}

void
add_memory_leak(struct error_reporter *reporter, uint32_t address, uint32_t size, char *function_name)
{
        struct memory_leak *errors = reporter->memory_leak_errors;
        uint32_t *errors_size = &reporter->memory_leak_errors_size;

        errors = realloc(errors, ++(*errors_size) * sizeof(*errors));
        errors[*errors_size - 1].address = address;
        errors[*errors_size - 1].size = size;

        errors[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50));
        strncpy(errors[*errors_size - 1].location.function_name, function_name,
                strnlen(function_name, 50)); // TODO: decide strncpy vs strcpy
}

void
add_doubled_free(struct error_reporter *reporter, uint32_t address, char *function_name)
{
        struct double_free *errors = reporter->double_free_errors;
        uint32_t *errors_size = &reporter->double_free_errors_size;

        errors = realloc(errors, ++(*errors_size) * sizeof(*errors));
        errors[*errors_size - 1].address = address;

        errors[*errors_size - 1].location.function_name = malloc(strnlen(function_name, 50));
        strncpy(errors[*errors_size - 1].location.function_name, function_name,
                strnlen(function_name, 50)); // TODO: decide strncpy vs strcpy
}

void
reporter_exit(struct error_reporter *reporter)
{
        for (uint32_t i = 0; i < reporter->undefined_memory_use_errors_size; ++i) {
                free(reporter->undefined_memory_use_errors[i].location.function_name);
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
}