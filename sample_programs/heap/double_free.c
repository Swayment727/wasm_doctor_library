#include <stdlib.h>

int
main(void)
{
        int *x = (int *)malloc(42);
        free(x);
        free(x);
        return 0;
}
