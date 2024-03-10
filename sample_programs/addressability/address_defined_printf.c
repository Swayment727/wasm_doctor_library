#include <stdint.h>
#include <stdio.h>

uint64_t
foo()
{
    printf("foo started\n");
    uint64_t *ptr = (uint64_t*)39;
    *ptr = 42;
    return *ptr;
}

int
main()
{
    printf("main started\n");
    return foo();
}
