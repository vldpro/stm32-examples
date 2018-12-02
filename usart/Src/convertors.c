#include "convertors.h"

#define RES_BUF_SZ 128

static struct mod_scope {
    uint8_t res_buf[RES_BUF_SZ];
} mod_scope = { 0 };

//
// Private functions
//

static int8_t is_upper(uint8_t c)
{
    return is_cyrillic(c) ? c >= 192 && c <= 223 : c >= 65 && c <= 90;
}

//
// Public functions
//

int8_t is_cyrillic(uint8_t c)
{
    return c >= 192;
}

int8_t is_latin(uint8_t c)
{
    return (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

uint8_t to_lower(uint8_t c)
{
    if (!is_upper(c))
        return c;
    if (is_cyrillic(c) || is_latin(c))
        return c + 32;
}

uint8_t to_upper(uint8_t c)
{
    if (is_upper(c))
        return c;
    if (is_cyrillic(c) || is_latin(c))
        return c - 32;
}

res_buf_t to_triple(uint8_t c)
{
    uint8_t i;
    for (i = 0; i < 3; i++)
        mod_scope.res_buf[i] = c;
    return (res_buf_t){ .data = mod_scope.res_buf, .sz = RES_BUF_SZ };
}