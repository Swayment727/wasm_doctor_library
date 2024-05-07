#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"

void
test_correct_zero_address_access_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        size_t address = 42;
        uint8_t bytes = 4;

        doctor_register_malloc(address, 100);

        doctor_store(address, bytes);

        assert(doctor.reporter.zero_address_access_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no zero address access test\n");
}

void
test_incorrect_zero_address_access_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        size_t address = 0;
        uint8_t bytes = 4;

        doctor_register_malloc(address, 100);

        doctor_store(address, bytes);

        assert(doctor.reporter.zero_address_access_errors_size == 1);

        doctor_store(address, bytes);

        assert(doctor.reporter.zero_address_access_errors_size == 2);

        doctor_load(address, bytes);

        assert(doctor.reporter.zero_address_access_errors_size == 3);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] zero address access test\n");
}

void
run_zero_address_access_errors_test(void)
{
        test_correct_zero_address_access_use();
        test_incorrect_zero_address_access_use();
}
