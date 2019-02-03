#include "sbus/sbus.h"
#include "sbus/bitops.h"

#include <stdint.h>

// actual

#define SBUS_START_BYTE 0x0F
#define SBUS_DATAFRAME_BITS 11
#define SBUS_TOTAL_DATA_BITS (SBUS_CHANNEL_BITS * SBUS_CHANNELS_COUNT)

static void unpack_dataframes_from_input(uint8_t const *input,
                                         uint16_t *dataframes)
{
    uint16_t dataframe = 0;

    for (uint8_t j = 0; j < SBUS_DATAFRAME_COUNT; j++) {
        for (uint8_t i = 0; i < SBUS_DATAFRAME_BITS; i++) {
            uint8_t bit = bitset_get_bit(input, j * SBUS_DATAFRAME_BITS + i);
            word_set_bit(&dataframe, i, bit);
        }

        dataframes[j] = word_reversed_bits(dataframe, SBUS_DATAFRAME_BITS);
        dataframe = 0;
    }
}

void sbus_unpack_frame(uint8_t const *input_data, struct sbus_frame *frame)
{
    // Extract input_data in dataframes
    uint8_t startbyte = input_data[0];
    unpack_dataframes_from_input(input_data + 1, frame->channels);
}

static void pack_output_from_dataframes(uint16_t const *dataframes,
                                        uint8_t *output)
{
    for (uint8_t j = 0; j < SBUS_DATAFRAME_COUNT; j++) {
        uint16_t reversed_frame =
            word_reversed_bits(dataframes[j], SBUS_DATAFRAME_BITS);
        for (uint8_t i = 0; i < SBUS_DATAFRAME_BITS; i++) {
            uint8_t bit = word_get_bit(reversed_frame, i);
            bitset_insert_bit(output, j * SBUS_DATAFRAME_BITS + i, bit);
        }
    }
}

void sbus_pack_frame(struct sbus_frame const *frame, uint8_t *dest)
{
    uint16_t const *channels = frame->channels;
    dest[0] = SBUS_START_BYTE;
    pack_output_from_dataframes(channels, dest + 1);
}
