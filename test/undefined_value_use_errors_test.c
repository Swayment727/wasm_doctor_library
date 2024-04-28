#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_correct_memory_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(0, "test_function");

        wasmptr_t address = 42;
        uint32_t bit_size = 32;
        doctor_store(address, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_load(address, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] correct memory use test\n");
}

void
test_incorrect_memory_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(0, "test_function");

        wasmptr_t address = 42;
        uint32_t bit_size = 32;
        doctor_store(address, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_load(address - 1, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 1);

        doctor_load(address - 32, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 2);

        doctor_load(address, bit_size + 1);

        assert(doctor.reporter.undefined_memory_use_errors_size == 3);

        wasmptr_t invalid_address = 60;
        doctor_load(invalid_address, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 4);

        doctor_load(address, bit_size);

        assert(doctor.reporter.undefined_memory_use_errors_size == 4);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] incorrect memory use test\n");
}

void
run_undefined_value_use_errors_test(void)
{
        test_correct_memory_use();
        test_incorrect_memory_use();
}
