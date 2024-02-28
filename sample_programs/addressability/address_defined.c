#include <stdint.h>
#include <stdio.h>

void
foo()
{
    uint64_t *ptr = (uint64_t*)39;
    *ptr = 42;
    printf("%p\n", ptr);
    printf("%llu\n", *ptr);
}

int
main()
{
    foo();
    return 0;
}
