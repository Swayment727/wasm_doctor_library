#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"

void
test_no_use_after_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.use_after_free_errors_size == 0);

        size_t address = 42;
        uint8_t bytes = 4;

        doctor_register_malloc(address, 100);

        assert(doctor.reporter.use_after_free_errors_size == 0);

        doctor_store(address, bytes);
        doctor_load(address, bytes);

        assert(doctor.reporter.use_after_free_errors_size == 0);

        doctor_register_free(address);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no use after free test\n");
}

void
test_use_after_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.use_after_free_errors_size == 0);

        size_t address = 42;
        doctor_register_malloc(address, 12);
        doctor_register_free(address);

        uint8_t bytes = 4;
        doctor_store(address, bytes);

        assert(doctor.reporter.use_after_free_errors_size == 1);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].address ==
               address);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].size == bytes);

        doctor_load(address, bytes);

        assert(doctor.reporter.use_after_free_errors_size == 2);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].address ==
               address);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].size == bytes);

        doctor_register_malloc(address, 24);

        doctor_store(address, bytes);
        doctor_load(address, bytes);

        assert(doctor.reporter.use_after_free_errors_size == 2);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].address ==
               address);
        assert(doctor.reporter.use_after_free_errors[doctor.reporter.use_after_free_errors_size - 1].size == bytes);

        doctor_register_free(address);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] use after free test\n");
}

void
run_use_after_free_test(void)
{
        test_no_use_after_free();
        test_use_after_free();
}
