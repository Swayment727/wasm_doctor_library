#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"
#include "wasm_types.h"

void
test_no_invalid_write(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 42;
        uint8_t bytes = 4;

        doctor_register_malloc(address, 100);

        doctor_store(address, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid write test\n");
}

void
test_no_invalid_write_shadow_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint8_t bytes = 4;

        doctor_set_shadow_stack_pointer_base(address);
        doctor_move_shadow_stack_pointer(address - 4);

        doctor_store(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_store(address - 4, bytes);
        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_move_shadow_stack_pointer(address);

        doctor_store(address - 4, bytes);
        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid write shadow stack test\n");
}

void
test_invalid_write(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 42;
        uint8_t bytes = 4;

        doctor_store(address, bytes);
        doctor_load(address, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_load(address, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid write test\n");
}

void
test_invalid_write_shadow_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint8_t bytes = 4;

        doctor_set_shadow_stack_pointer_base(address);
        doctor_move_shadow_stack_pointer(address - 4);

        doctor_store(address - 2, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address - 2, bytes);
        doctor_load(address - 2, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_store(address - 4, bytes);
        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid write shadow stack test\n");
}

void
test_invalid_write_bounds_shadow_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        wasmptr_t address = 64;
        uint8_t bytes = 4;

        doctor_set_shadow_stack_pointer_base(address);
        doctor_move_shadow_stack_pointer(address - 4);

        doctor_store(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_store(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address - 3, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid write bounds shadow stack test\n");
}

void
test_invalid_write_bounds_global_data(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint8_t bytes = 4;

        doctor_global_data_validate(address - 4, bytes);

        doctor_store(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_store(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address - 3, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid write bounds global data test\n");
}

void
test_invalid_write_bounds_malloc(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_write_errors_size == 0);

        wasmptr_t address = 64;
        uint8_t bytes = 4;

        doctor_register_malloc(address - 4, bytes);

        doctor_store(address - 4, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 0);

        doctor_store(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_write_errors_size == 1);

        doctor_store(address - 3, bytes);

        assert(doctor.reporter.invalid_write_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid write bounds malloc test\n");
}

void
run_invalid_write_test(void)
{
        test_no_invalid_write();
        test_invalid_write();
        test_no_invalid_write_shadow_stack();
        test_invalid_write_shadow_stack();
        test_invalid_write_bounds_shadow_stack();
        test_invalid_write_bounds_global_data();
        test_invalid_write_bounds_malloc();
}
