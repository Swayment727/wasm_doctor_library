#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_memory_leak(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.memory_leak_errors_size == 0);

        wasmptr_t address = 42;

        doctor_register_malloc(address, 100);
        doctor_register_free(address);

        assert(doctor.reporter.memory_leak_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);

        assert(doctor.reporter.memory_leak_errors_size == 0);

        doctor_reporter_exit();

        printf("[OK] no memory leak test\n");
}

void
test_memory_leak(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        char *function_name = "test_function";
        doctor_frame_enter(10, function_name);

        assert(doctor.reporter.memory_leak_errors_size == 0);

        wasmptr_t address = 42;
        uint32_t size_in_bytes = 100;

        doctor_register_malloc(address, size_in_bytes);

        assert(doctor.reporter.memory_leak_errors_size == 0);
        doctor_frame_exit();

        doctor_exit(false);

        assert(doctor.reporter.memory_leak_errors_size == 1);
        assert(doctor.reporter.memory_leak_errors[doctor.reporter.memory_leak_errors_size - 1].address == address);
        assert(doctor.reporter.memory_leak_errors[doctor.reporter.memory_leak_errors_size - 1].size == size_in_bytes);
        assert(strncmp(doctor.reporter.memory_leak_errors[0].location.function_name, function_name,
                       strlen(function_name)) == 0);

        doctor_reporter_exit();

        printf("[OK] memory leak test\n");
}

void
run_memory_leak_test(void)
{
        test_no_memory_leak();
        test_memory_leak();
}
