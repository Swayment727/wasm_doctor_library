#include <stdint.h>

struct example_struct {
        uint8_t a;
        uint32_t b;
        uint8_t c;
        uint32_t d;
        uint8_t e;
        uint32_t f;
};

struct example_struct
duplicate_struct(struct example_struct *e)
{
        return *e;
}

int
main()
{
        uint8_t undef;
        struct example_struct e = {.a = 1, .b = 2, .c = 3, .d = 4, .e = undef};
        /* struct example_struct e_duplicate = duplicate_struct(&e); */
        return e.e;
}
