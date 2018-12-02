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

uint8_t to_lower(uint8_t c);
uint8_t to_upper(uint8_t c);

#endif