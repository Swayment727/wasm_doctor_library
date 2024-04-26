#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
foo()
{
        uint32_t *ptr = (uint32_t *)malloc(42);
        printf("start %p\n", (void *)ptr);
        printf("end %p\n", (void *)((uint8_t *)ptr + 42));
        free(ptr);
}

int
main()
{
        foo();
        return 0;
}
