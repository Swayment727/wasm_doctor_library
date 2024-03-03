#include <stdint.h>
#include <stdio.h>

void
foo()
{
    printf("foo started\n");
    uint64_t *ptr;
    *ptr = 42;
    printf("%p\n", ptr);
    printf("%llu\n", *ptr);
}

int
main()
{
    printf("main started\n");
    foo();
    return 0;
}
