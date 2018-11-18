#ifndef UTILS_H
#define UTILS_h

struct leds_list;

void delay(int i);
void test_leds_formation(struct leds_list *leds);

inline int bit_at(unsigned char bitset, unsigned int i)
{
    return (bitset >> i) & 0x01;
}

inline void soft_delay(unsigned long i)
{
    for (; i--;) {
    }
}

#endif