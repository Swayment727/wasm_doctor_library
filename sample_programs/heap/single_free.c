#include <stdlib.h>

int
main(void)
{
        int *x = (int *)malloc(42);
        free(x);
        return 0;
}
