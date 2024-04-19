#include <stdint.h>
#include <stdlib.h>

void
foo()
{
        uint32_t *ptr = (uint32_t *)malloc(42);
        free(ptr);
}

int
main()
{
        foo();
        return 0;
}
