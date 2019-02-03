#include "stdint.h"

#define SBUS_DATAFRAME_COUNT 16
#define SBUS_DATAFRAME_SIZE_BYTES 22


struct sbus_frame {
    uint16_t channels[SBUS_DATAFRAME_COUNT];
};

// src - 22 byte array with 16 compressed 11 bit integers
void sbus_unpack_frame(uint8_t const* src,
                       struct sbus_frame* frame);

// dest - 22 byte array with 16 compressed 11 bit integers
void sbus_pack_frame(struct sbus_frame const* frame,
                     uint8_t* dest);
