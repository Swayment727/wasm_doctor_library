#include <stdlib.h>

int
main(void)
{
        int *numbers = (int *)malloc(10 * sizeof(*numbers));

        free(numbers);
        free(numbers);

        return 0;
}
