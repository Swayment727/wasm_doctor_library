#include <stdint.h>

uint64_t *
get_local_ptr()
{
        uint64_t result = 42;
        return &result;
}

int
main()
{
        *get_local_ptr() = 0;
        return 0;
}
