#include "convertors.h"

#define RES_BUF_SZ 128

static struct mod_scope {
    uint8_t res_buf[RES_BUF_SZ];
} mod_scope = { 0 };

//
// Private functions
//

static uint8_t is_digit(uint8_t c)
{
    return c >= '0' && c <= '9';
}

static uint8_t digit_to_num(uint8_t c)
{
    return c - '0';
}

static int bit_at(uint8_t bitset, uint8_t i)
{
    return (bitset >> i) & 0x01;
}

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
    uint8_t const res_buf_sz = 3;
    uint8_t i;
    for (i = 0; i < res_buf_sz; i++)
        mod_scope.res_buf[i] = c;
    return (res_buf_t){ .data = mod_scope.res_buf, .sz = res_buf_sz };
}

res_buf_t to_binary(uint8_t num)
{
    uint8_t const res_buf_sz = 8;
    uint8_t i;
    for (i = 0; i < res_buf_sz; i++)
        mod_scope.res_buf[i] = bit_at(num, i) ? '1' : '0';
    return (res_buf_t){ .data = mod_scope.res_buf, .sz = res_buf_sz };
}

int64_t to_num(uint8_t const *buf, uint32_t sz)
{
    int64_t res = 0;
    uint32_t i;
    for (i = 0; i < sz; i++) {
        if (!is_digit(buf[i]))
            return -1;
        int8_t num = digit_to_num(buf[i]);
        int64_t mult = i != (sz - 1) ? 10 * (sz - 1 - i) : 1;
        res += mult * num;
    }

    return res;
}