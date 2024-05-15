#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
        char orange[] = "orange";
        // allocated memory has the correct size
        char *orange_copy = (char *)malloc(strlen(orange) + 1);
        // the content of the string is copied without the terminating 0
        for (int i = 0; i < strlen(orange); ++i) {
                orange_copy[i] = orange[i];
        }
        // string that is null terminated is printed
        printf("%s\n", orange_copy);

        free(orange_copy);
        return 0;
}
