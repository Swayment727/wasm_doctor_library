#include <stdlib.h>
#include <string.h>

int
main(void)
{
        char orange[] = "orange";
        // allocated memory is too small by 1 byte
        char *orange_copy = (char *)malloc(strlen(orange));
        // strcpy writes outside of the allocated block
        strcpy(orange_copy, orange);

        free(orange_copy);
        return 0;
}
