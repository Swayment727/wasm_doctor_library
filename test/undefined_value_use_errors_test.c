#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_correct_memory_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(0, "test_function");

        wasmptr_t address = 42;
        uint8_t bytes = 4;
        doctor_store(address, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_load(address, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] correct memory use test\n");
}

void
test_incorrect_memory_use(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(0, "test_function");

        wasmptr_t address = 42;
        uint8_t bytes = 4;
        doctor_store(address, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 0);

        doctor_load(address - 1, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 1);

        doctor_load(address - 32, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 2);

        doctor_load(address, bytes + 1);

        assert(doctor.reporter.undefined_memory_use_errors_size == 3);

        wasmptr_t invalid_address = address + 2;
        doctor_load(invalid_address, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 4);
        printf("%u %u\n",
               doctor.reporter.undefined_memory_use_errors[doctor.reporter.undefined_memory_use_errors_size - 1]
                       .address,
               invalid_address);

        assert(doctor.reporter.undefined_memory_use_errors[doctor.reporter.undefined_memory_use_errors_size - 1]
                       .address == invalid_address);
        bool validity[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        assert(memcmp(doctor.reporter.undefined_memory_use_errors[doctor.reporter.undefined_memory_use_errors_size - 1]
                              .validity,
                      validity, bytes * 8));

        doctor_load(address, bytes);

        assert(doctor.reporter.undefined_memory_use_errors_size == 4);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] incorrect memory use test\n");
}

void
run_undefined_value_use_errors_test(void)
{
        test_correct_memory_use();
        test_incorrect_memory_use();
}
