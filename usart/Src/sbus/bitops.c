#include "sbus/bitops.h"

#define BYTE_BITS_COUNT 8

uint8_t byte_get_bit(uint8_t byte, uint8_t num)
{
    uint8_t mask = 0x01 << num;
    uint8_t masked = byte & mask;
    uint8_t bit = masked >> num;
    return bit;
}

void byte_set_bit(uint8_t *byte, uint8_t pos, uint8_t bit)
{
    // Clear bit
    *byte &= ~(1 << pos);

    // Set bit
    uint8_t ordered = bit << pos;
    *byte |= ordered;
}

uint16_t word_reversed_bits(uint16_t word, uint8_t count)
{
    uint16_t new_word = 0;
    for (uint8_t i = 0; i < count; i++) {
        uint8_t bit = word_get_bit(word, i);
        word_set_bit(&new_word, count - i - 1, bit);
    }
    return new_word;
}

uint8_t word_get_bit(uint16_t word, uint8_t pos)
{
    uint16_t mask = 0x01 << pos;
    uint16_t masked = word & mask;
    uint8_t bit = masked >> pos;
    return bit;
}

void word_set_bit(uint16_t *word, uint8_t pos, uint8_t bit)
{
    // Clear bit
    *word &= ~(1 << pos);

    // Set bit
    uint16_t ordered = bit << pos;
    *word |= ordered;
}

uint8_t bitset_get_bit(uint8_t const *bitset, uint8_t bitpos)
{
    uint8_t byte_num = bitpos / BYTE_BITS_COUNT;
    uint8_t bitpos_in_byte = bitpos % BYTE_BITS_COUNT;
    return byte_get_bit(bitset[byte_num], BYTE_BITS_COUNT - bitpos_in_byte - 1);
}

void bitset_insert_bit(uint8_t *bitset, uint8_t bitpos, uint8_t bit)
{
    uint8_t byte_num = bitpos / BYTE_BITS_COUNT;
    uint8_t bitpos_in_byte = bitpos % BYTE_BITS_COUNT;
    byte_set_bit(&bitset[byte_num], BYTE_BITS_COUNT - bitpos_in_byte - 1, bit);
}

#ifdef TEST_COMPILE
#include "stdio.h"

#define TEST_FAILED 1
#define TEST_PASSED 0

#define ASSERT(condition, testname)                                            \
    if (!(condition)) {                                                        \
        printf("TEST: %s: failed\n", testname);                                \
        return TEST_FAILED;                                                    \
    } else {                                                                   \
        printf("TEST: %s: passed\n", testname);                                \
        return TEST_PASSED;                                                    \
    }

uint8_t test__byte_get_bit()
{
    uint8_t byte = 0x04;
    uint8_t bit = byte_get_bit(byte, 2);
    ASSERT(bit == 1, "byte_get_bit")
}

uint8_t test__byte_set_bit()
{
    uint8_t byte = 0x04;
    byte_set_bit(&byte, 2, 0);
    ASSERT(byte == 0, "byte_set_bit");
}

uint8_t test__word_get_bit()
{
    uint16_t word = 0x04;
    uint16_t bit = word_get_bit(word, 2);
    ASSERT(bit == 1, "word_get_bit")
}

uint8_t test__word_set_bit()
{
    uint16_t word = 0x04;
    word_set_bit(&word, 2, 0);
    ASSERT(word == 0, "word_set_bit");
}

uint8_t test__word_reversed_bits()
{
    uint16_t word = 0x0300;
    uint16_t reversed = word_reversed_bits(word, 11);
    ASSERT(reversed == 0x0006, "word_reversed_bits")
}

uint8_t test__bitset_get_bit()
{
    uint8_t bitset[3] = { 0x00, 0x01, 0x00 };
    uint8_t bit = bitset_get_bit(bitset, 15);
    ASSERT(bit == 1, "bitset_get_bit");
}

uint8_t test__bitset_set_bit()
{
    uint8_t bitset[3] = { 0x00, 0x01, 0x00 };
    bitset_insert_bit(bitset, 15, 0);
    ASSERT(bitset[1] == 0, "bitset_set_bit");
}

uint8_t bitops_run_tests()
{
    test__byte_get_bit();
    test__byte_set_bit();
    test__word_get_bit();
    test__word_set_bit();
    test__word_reversed_bits();
    test__bitset_get_bit();
    test__bitset_set_bit();
}

#endif