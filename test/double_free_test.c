#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_double_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.double_free_errors_size == 0);

        wasmptr_t address = 42;

        doctor_register_malloc(address, 100);
        doctor_register_free(address);

        assert(doctor.reporter.double_free_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no double free test\n");
}

void
test_double_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.double_free_errors_size == 0);

        wasmptr_t address = 42;

        doctor_register_malloc(address, 100);
        doctor_register_free(address);

        assert(doctor.reporter.double_free_errors_size == 0);

        doctor_register_free(address);

        assert(doctor.reporter.double_free_errors_size == 1);
        assert(doctor.reporter.double_free_errors[doctor.reporter.double_free_errors_size - 1].address == address);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] double free test\n");
}

void
run_double_free_test(void)
{
        test_no_double_free();
        test_double_free();
}
