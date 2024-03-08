#ifndef WASM_DOCTOR
#define WASM_DOCTOR

// 0003cce 0004a6c

#include <limits.h> /* for CHAR_BIT */
#include <stdint.h> /* for uint32_t */

typedef uint32_t word_t;
enum { BITS_PER_WORD = sizeof(word_t) * CHAR_BIT };
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b) ((b) % BITS_PER_WORD)

void
set_bit(word_t *words, int n)
{
        words[WORD_OFFSET(n)] |= ((word_t)1 << BIT_OFFSET(n));
}

void
clear_bit(word_t *words, int n)
{
        words[WORD_OFFSET(n)] &= ~((word_t)1 << BIT_OFFSET(n));
}

int
get_bit(word_t *words, int n)
{
        word_t bit = words[WORD_OFFSET(n)] & ((word_t)1 << BIT_OFFSET(n));
        return bit != 0;
}

#include <stdint.h>

uint8_t initialized = 0;
word_t words[100000000];

void
init(void)
{
        if (!initialized) {
                for (uint32_t i = 0x0003cce; i <= 0x0004a6c; ++i) {
                        set_bit(words, i);
                }

                initialized = 1;
        }
}

void
register_store(uint32_t address)
{
        init();

        for (uint32_t i = 0; i < 32; ++i) {
                set_bit(words, address + i);
        }
}

uint8_t
validate_load(uint32_t address)
{
        init();

        return get_bit(words, address);
}

#endif /* WASM_DOCTOR */
