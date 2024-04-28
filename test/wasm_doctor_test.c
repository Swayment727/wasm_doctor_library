#include <stdio.h>

#include "undefined_local_use_errors_test.c"
#include "undefined_value_use_errors_test.c"
#include "use_after_free_test.c"

int
main(void)
{
        printf("Running tests\n");
        run_undefined_value_use_errors_test();
        run_undefined_local_use_errors_test();
        run_use_after_free_test();
        printf("Tests successfully finished\n");
        return 0;
}
