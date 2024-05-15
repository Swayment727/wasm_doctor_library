#include <stdlib.h>

int
main(void)
{
        int number_of_names = 4;

        char **names = (char **)malloc(number_of_names * sizeof(*names));

        names[0] = (char *)malloc(5 * sizeof(**names));
        names[1] = (char *)malloc(8 * sizeof(**names));
        names[2] = (char *)malloc(7 * sizeof(**names));
        names[3] = (char *)malloc(5 * sizeof(**names));

        for (int i = 0; i <= number_of_names; ++i) { // i is incremented upto 4
                free(names[i]);
        }

        return 0;
}
