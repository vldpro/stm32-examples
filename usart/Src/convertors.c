#include "convertors.h"

#define RES_BUF_SZ 128

static struct mod_scope {
    uint8_t res_buf[RES_BUF_SZ];
} mod_scope = { 0 };

//
// Private functions
//

static int8_t is_upper(char c)
{
    return is_cyrillic(c) ? c >= 192 && c <= 223 : c >= 65 && c <= 90;
}

//
// Public functions
//

int8_t is_cyrillic(char c)
{
    return c >= 192;
}

int8_t is_latin(char c)
{
    return c >= 97 && c <= 122 || c >= 65 && c <= 90;
}

char to_lower(char c)
{
    if (!is_upper(c))
        return c;
    if (is_cyrillic(c) || is_latin(c))
        return c + 32;
}

char to_upper(char c)
{
    if (is_upper(c))
        return c;
    if (is_cyrillic(c) || is_latin(c))
        return c - 32;
}

res_buf_t to_triple(char c)
{
    uint8_t i;
    for (i = 0; i < 3; i++)
        mod_scope.res_buf[i] = c;
    return (res_buf_t){ .data = mod_scope.res_buf, .sz = RES_BUF_SZ };
}