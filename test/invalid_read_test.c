#include <assert.h>
#include <stdio.h>

#include "wasm_doctor.h"

#define RED_ZONE_SIZE 128

void
test_no_invalid_read(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_register_malloc(address, 100);

        doctor_store(address, bytes);
        doctor_load(address, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid read test\n");
}

void
test_no_invalid_read_linear_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_set_linear_stack_pointer_base(address);
        doctor_move_linear_stack_pointer(address - 4);

        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_store(address + 4 - RED_ZONE_SIZE, bytes);
        doctor_load(address + 4 - RED_ZONE_SIZE, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_move_linear_stack_pointer(address);

        doctor_store(address - 4 - RED_ZONE_SIZE, bytes);
        doctor_load(address - 4 - RED_ZONE_SIZE, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] no invalid read linear stack test\n");
}

void
test_invalid_read(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 142;
        uint8_t bytes = 4;

        doctor_load(address, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_store(address, bytes);
        doctor_load(address, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid read test\n");
}

void
test_invalid_read_linear_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_set_linear_stack_pointer_base(address);
        doctor_move_linear_stack_pointer(address - 4);

        doctor_load(address - 2, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_store(address - 2, bytes);
        doctor_load(address - 2, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_store(address - 4, bytes);
        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid read linear stack test\n");
}

void
test_invalid_read_bounds_linear_stack(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_set_linear_stack_pointer_base(address);
        doctor_move_linear_stack_pointer(address - 4);

        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_load(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_load(address - 3, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid read bounds linear stack test\n");
}

void
test_invalid_read_bounds_global_data(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_global_data_validate(address - 4, bytes);

        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_load(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_load(address - 3, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid read bounds global data test\n");
}

void
test_invalid_read_bounds_malloc(void)
{
        struct wasm_doctor doctor;
        doctor_init(&doctor, 2, false);

        doctor_frame_enter(10, "test_function");

        assert(doctor.reporter.invalid_read_errors_size == 0);

        size_t address = 164;
        uint8_t bytes = 4;

        doctor_register_malloc(address - 4, bytes);

        doctor_load(address - 4, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 0);

        doctor_load(address - 4, bytes + 1);

        assert(doctor.reporter.invalid_read_errors_size == 1);

        doctor_load(address - 3, bytes);

        assert(doctor.reporter.invalid_read_errors_size == 2);

        doctor_frame_exit();

        doctor_exit(false);
        doctor_reporter_exit();

        printf("[OK] invalid read bounds malloc test\n");
}

void
run_invalid_read_test(void)
{
        test_no_invalid_read();
        test_invalid_read();
        test_no_invalid_read_linear_stack();
        test_invalid_read_linear_stack();
        test_invalid_read_bounds_linear_stack();
        test_invalid_read_bounds_global_data();
        test_invalid_read_bounds_malloc();
}
