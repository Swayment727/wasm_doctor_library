#include <stdint.h>
#include <stdlib.h>

void
foo()
{
        uint32_t *ptr = malloc(42);
        free(ptr);
        free(ptr);
}

int
main()
{
        foo();
        return 0;
}
