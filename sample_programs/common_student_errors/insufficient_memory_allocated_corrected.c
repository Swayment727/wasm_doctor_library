#include <stdlib.h>
#include <string.h>

int
main(void)
{
        char orange[] = "orange";
        // allocated memory has the correct size
        char *orange_copy = (char *)malloc(strlen(orange) + 1);
        strcpy(orange_copy, orange);

        free(orange_copy);
        return 0;
}
