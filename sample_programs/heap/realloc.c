#include <stdlib.h>

int
main(void)
{
        int *x = NULL;
        x = (int *)malloc(42);
        x = (int *)realloc(x, 81);
        free(x);
        return 0;
}
