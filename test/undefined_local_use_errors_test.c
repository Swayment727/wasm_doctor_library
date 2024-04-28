#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"

void
test_correct_local_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        uint32_t local_idx = 2;
        doctor_local_set(local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 0);

        doctor_local_get(local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] correct local use test\n");
}

void
test_incorrect_local_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        uint32_t local_idx = 2;
        doctor_local_set(local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 0);

        doctor_local_get(local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 0);

        uint32_t undefined_local_idx = 3;
        doctor_local_get(undefined_local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 1);

        doctor_local_get(undefined_local_idx);

        assert(doctor.reporter.undefined_local_use_errors_size == 2);

        uint32_t undefined_local_idx_2 = 4;
        doctor_local_get(undefined_local_idx_2);

        assert(doctor.reporter.undefined_local_use_errors_size == 3);

        doctor_local_set(undefined_local_idx_2);
        doctor_local_get(undefined_local_idx_2);

        assert(doctor.reporter.undefined_local_use_errors_size == 3);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] incorrect local use test\n");
}

void
run_undefined_local_use_errors_test(void)
{
        test_correct_local_use();
        test_incorrect_local_use();
}
