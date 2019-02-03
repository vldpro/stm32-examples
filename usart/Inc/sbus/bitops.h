#include <stdint.h>

uint16_t word_reversed_bits(uint16_t word, uint8_t count);

uint8_t word_get_bit(uint16_t word, uint8_t pos);
void word_set_bit(uint16_t* word, uint8_t pos, uint8_t bit);

uint8_t bitset_get_bit(uint8_t const* bitset, uint8_t bitpos);
void bitset_insert_bit(uint8_t* bitset, uint8_t bitpos, uint8_t bit);

#ifdef TEST_COMPILE
uint8_t bitops_run_tests();
#endif
