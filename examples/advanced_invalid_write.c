#include <stdlib.h>
#include <string.h>

int
main(void)
{
        char *name = (char *)malloc(4 * sizeof(*name));

        strcpy(name, "John");

        free(name);

        return 0;
}
