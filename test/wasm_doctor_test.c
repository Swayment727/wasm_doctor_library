#include <stdio.h>

#include "double_free_test.c"
#include "invalid_free_test.c"
#include "invalid_read_test.c"
#include "invalid_write_test.c"
#include "memory_leak_test.c"
#include "undefined_local_use_errors_test.c"
#include "undefined_value_use_errors_test.c"
#include "use_after_free_test.c"
#include "zero_address_access_test.c"

int
main(void)
{
        printf("Running tests\n");
        run_undefined_value_use_errors_test();
        run_undefined_local_use_errors_test();
        run_use_after_free_test();
        run_memory_leak_test();
        run_double_free_test();
        run_invalid_free_test();
        run_invalid_read_test();
        run_invalid_write_test();
        run_zero_address_access_errors_test();
        printf("Tests successfully finished\n");
        return 0;
}
