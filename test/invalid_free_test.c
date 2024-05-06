#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_invalid_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_free_errors_size == 0);

        wasmptr_t address = 42;

        doctor_register_malloc(address, 100);
        doctor_register_free(address);

        assert(doctor.reporter.invalid_free_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid free test\n");
}

void
test_invalid_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_free_errors_size == 0);

        wasmptr_t address = 42;

        doctor_register_malloc(address, 100);

        wasmptr_t invalid_address = 180;
        doctor_register_free(invalid_address);

        assert(doctor.reporter.invalid_free_errors_size == 1);
        assert(doctor.reporter.invalid_free_errors[doctor.reporter.invalid_free_errors_size - 1].address ==
               invalid_address);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid free test\n");
}

void
run_invalid_free_test(void)
{
        test_no_invalid_free();
        test_invalid_free();
}
