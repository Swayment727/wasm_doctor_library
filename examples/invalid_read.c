#include <stdlib.h>
#include <string.h>

int
main(void)
{
        int number_of_names = 2;

        char **names = (char **)malloc(number_of_names * sizeof(*names));
        names[0] = (char *)malloc(5 * sizeof(**names));
        names[1] = (char *)malloc(5 * sizeof(**names));

        // The string "John" is copied into memory that was not allocated
        strcpy(names[2], "John");

        free(names);
        free(names[0]);
        free(names[1]);

        return 0;
}
