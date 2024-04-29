#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_invalid_write(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 42;
        uint32_t bit_size = 32;

        doctor_register_malloc(address, 100);

        doctor_store(address, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] no invalid write test\n");
}

void
test_no_invalid_write_shadow_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint32_t bit_size = 32;

        doctor_move_shadow_stack_pointer(address / 8);
        doctor_move_shadow_stack_pointer(address / 8 - 4);

        doctor_store(address / 8 - 4, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_store(address / 8 - 4, bit_size);
        doctor_load(address / 8 - 4, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_move_shadow_stack_pointer(address / 8);

        doctor_store(address / 8 - 4, bit_size);
        doctor_load(address / 8 - 4, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] no invalid write shadow stack test\n");
}

void
test_invalid_write(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 42;
        uint32_t bit_size = 32;

        doctor_store(address, bit_size);
        doctor_load(address, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_load(address, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] invalid write test\n");
}

void
test_invalid_write_shadow_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint32_t bit_size = 32;

        doctor_move_shadow_stack_pointer(address / 8);
        doctor_move_shadow_stack_pointer(address / 8 - 4);

        doctor_store(address / 8 - 2, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address / 8 - 2, bit_size);
        doctor_load(address / 8 - 2, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_store(address / 8 - 4, bit_size);
        doctor_load(address / 8 - 4, bit_size);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(true);

        printf("[OK] no invalid write shadow stack test\n");
}

void
run_invalid_write_test(void)
{
        test_no_invalid_write();
        test_invalid_write();
        test_no_invalid_write_shadow_stack();
        test_invalid_write_shadow_stack();
}
