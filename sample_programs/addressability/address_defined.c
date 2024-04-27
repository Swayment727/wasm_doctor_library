#include <stdint.h>

uint64_t
foo(void)
{
        uint64_t *ptr = (uint64_t *)39;
        *ptr = 42;
        return *ptr;
}

int
main(void)
{
        return foo();
}
