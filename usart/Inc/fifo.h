#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stddef.h>

typedef enum fifo_res { FIFO_SUCCSESS, FIFO_EMPTY, FIFO_FULL } fifo_res_t;

typedef struct fifo {
    uint8_t *data;
    uint16_t cap;
    int32_t roff;
    int32_t woff;
} fifo_t;

#define FIFO_INITIAL_DATA                                                      \
    {                                                                          \
        .data = NULL, .cap = 0, .roff = -1, .woff = 0                          \
    }

inline void fifo_init(fifo_t *fifo, uint8_t *data, uint16_t sz)
{
    *fifo = (fifo_t){ .data = data, .cap = sz, .roff = -1, .woff = 0 };
}

inline int8_t fifo_is_full(fifo_t const *fifo)
{
    if (fifo->woff <= fifo->roff)
        return fifo->woff == fifo->roff;
    return fifo->woff == fifo->cap - 1 && fifo->roff == -1;
}

inline uint8_t fifo_is_empty(fifo_t const *fifo)
{
    if (fifo->woff == 0)
        return fifo->roff + 1 == fifo->cap;
    else
        return fifo->woff == fifo->roff + 1;
}

inline fifo_res_t fifo_push_back(fifo_t *fifo, uint8_t val)
{
    if (fifo_is_full(fifo))
        return FIFO_FULL;

    fifo->data[fifo->woff] = val;
    fifo->woff++;
    if (fifo->woff == fifo->cap)
        fifo->woff = 0; // prevent buffer overflow
    return FIFO_SUCCSESS;
}

inline fifo_res_t fifo_pop_front(fifo_t *fifo, uint8_t *val)
{
    if (fifo_is_empty(fifo))
        return FIFO_EMPTY;

    fifo->roff++;
    if (fifo->roff == fifo->cap)
        fifo->roff = 0; // prevent buffer overflow
    *val = fifo->data[fifo->roff];
    return FIFO_SUCCSESS;
}

#endif