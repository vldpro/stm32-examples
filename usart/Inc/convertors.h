#ifndef CONVERTORS_H
#define CONVERTORS_H

#include <stdint.h>

typedef struct res_buf {
    uint8_t const *data;
    uint32_t const sz;
} res_buf_t;

int8_t is_cyrillic(char c);
int8_t is_latin(char c);
res_buf_t to_triple(char c);

char to_lower(char c);
char to_upper(char c);

#endif