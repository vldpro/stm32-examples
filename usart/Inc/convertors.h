#ifndef CONVERTORS_H
#define CONVERTORS_H

#include <stdint.h>

typedef struct res_buf {
    uint8_t const *data;
    uint32_t const sz;
} res_buf_t;

int8_t is_cyrillic(uint8_t c);
int8_t is_latin(uint8_t c);
res_buf_t to_triple(uint8_t c);
res_buf_t to_binary(uint8_t num);
int64_t to_num(uint8_t const *buf, uint32_t num);

uint8_t to_lower(uint8_t c);
uint8_t to_upper(uint8_t c);

inline uint16_t cyrrillic_to_upper(uint16_t c)
{
    if (c >= 0xD0B0 && c <= 0xD0BF)
        return c - 0xD0B0 + 0xD090;
    else if (c >= 0xD180 && c <= 0xD18f)
        return (c - 0xD180) + (0xD0BF - 0xD0B0) + 0xD090 + 1;
    return c;
}

#endif