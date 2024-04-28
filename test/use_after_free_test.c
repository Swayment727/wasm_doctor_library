#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_use_after_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.use_after_free_errors_size == 0);

        wasmptr_t address = 42;
        uint32_t bit_size = 32;

        doctor_register_malloc(address, 100);

        assert(doctor.reporter.use_after_free_errors_size == 0);

        doctor_store(address, bit_size);
        doctor_load(address, bit_size);

        assert(doctor.reporter.use_after_free_errors_size == 0);

        doctor_register_free(address);

        doctor_frame_exit();

        doctor_exit();

        printf("[OK] no use after free test\n");
}

void
test_use_after_free(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.use_after_free_errors_size == 0);

        wasmptr_t address = 42;
        doctor_register_malloc(address, 100);
        doctor_register_free(address);

        uint32_t bit_size = 32;
        doctor_store(address, bit_size);

        assert(doctor.reporter.use_after_free_errors_size == 1);

        doctor_load(address, bit_size);

        assert(doctor.reporter.use_after_free_errors_size == 2);

        doctor_register_malloc(address, 200);

        doctor_store(address, bit_size);
        doctor_load(address, bit_size);

        assert(doctor.reporter.use_after_free_errors_size == 2);

        doctor_register_free(address);

        doctor_frame_exit();

        doctor_exit();

        printf("[OK] use after free test\n");
}

void
run_use_after_free_test(void)
{
        test_no_use_after_free();
        test_use_after_free();
}
