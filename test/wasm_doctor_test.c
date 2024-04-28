#include <stdio.h>

#include "undefined_value_use_errors_test.c"

int
main(void)
{
        printf("Running tests\n");
        run_undefined_value_use_errors_test();
        printf("Tests successfully finished\n");
        return 0;
}
