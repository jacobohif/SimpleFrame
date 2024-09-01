#ifndef SIMPLE_FRAME_H
#define SIMPLE_FRAME_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOF 0x01
#define HEADER_SIZE 8  // SOF(1) + ID(2) + LEN(2) + TYPE(2) + HEAD_CKSUM(1) + DATA(4) + DATA_CKSUM(1)

typedef struct {
    uint16_t id;          // Frame ID
    uint16_t len;         // Data frame length (excluding header)
    uint16_t type;        // Frame type
    uint8_t head_cksum;   // Header checksum
    uint8_t* data;        // Data (variable length)
    uint8_t data_cksum;   // Data checksum
} SimpleFrame;

// Create a new frame
SimpleFrame* create_frame(uint16_t id, uint16_t type, const uint8_t* data, uint16_t data_len);

// Destroy the frame
void destroy_frame(SimpleFrame* frame);

// Serialize the frame to a byte array
uint8_t* serialize_frame(SimpleFrame* frame, size_t* frame_size);

// Deserialize a byte array to a frame
SimpleFrame* deserialize_frame(const uint8_t* buffer, size_t buffer_len);

// Calculate the header checksum
uint8_t calculate_header_checksum(uint16_t id, uint16_t len, uint16_t type);

// Calculate the data checksum
//uint8_t calculate_data_checksum(const uint8_t* data, uint16_t data_len);


unsigned char calculate_data_checksum(unsigned char *data, unsigned char len);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_FRAME_H