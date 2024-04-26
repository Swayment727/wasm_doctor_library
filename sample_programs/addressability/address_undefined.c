#include <stdint.h>

uint64_t
foo()
{
        uint64_t *ptr;
        *ptr = 42;
        return *ptr;
}

int
main()
{
        return foo();
}
