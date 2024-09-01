#include "SimpleFrame.h"
#include <stdlib.h>
#include <string.h>

//创建包
SimpleFrame* create_frame(uint16_t id, uint16_t type, const uint8_t* data, uint16_t data_len) {
    SimpleFrame* frame = (SimpleFrame*)malloc(sizeof(SimpleFrame));
    if (!frame) return NULL;

    frame->id = id;
    frame->len = data_len;
    frame->type = type;
    frame->head_cksum = calculate_header_checksum(id, data_len, type);

    if (data_len > 0 && data) {
        frame->data = (uint8_t*)malloc(data_len);
        memcpy(frame->data, data, data_len);
        frame->data_cksum = calculate_data_checksum(data, data_len);
    } else {
        frame->data = NULL;
        frame->data_cksum = 0;
    }

    return frame;
}
//回收
void destroy_frame(SimpleFrame* frame) {
    if (frame) {
        if (frame->data) free(frame->data);
        free(frame);
    }
}
//序列化
uint8_t* serialize_frame(SimpleFrame* frame, size_t* frame_size) {
    *frame_size = HEADER_SIZE + frame->len + 1;
    uint8_t* buffer = (uint8_t*)malloc(*frame_size);

    buffer[0] = SOF;
    buffer[1] = (frame->id >> 8) & 0xFF;
    buffer[2] = frame->id & 0xFF;
    buffer[3] = (frame->len >> 8) & 0xFF;
    buffer[4] = frame->len & 0xFF;
    buffer[5] = (frame->type >> 8) & 0xFF;
    buffer[6] = frame->type & 0xFF;
    buffer[7] = frame->head_cksum;

    if (frame->data && frame->len > 0) {
        memcpy(&buffer[8], frame->data, frame->len);
    }

    buffer[8 + frame->len] = frame->data_cksum;

    return buffer;
}
//解析包
SimpleFrame* deserialize_frame(const uint8_t* buffer, size_t buffer_len) {
    if (buffer_len < HEADER_SIZE) return NULL;

    uint16_t id = (buffer[1] << 8) | buffer[2];
    uint16_t len = (buffer[3] << 8) | buffer[4];
    uint16_t type = (buffer[5] << 8) | buffer[6];
    uint8_t head_cksum = buffer[7];

    if (calculate_header_checksum(id, len, type) != head_cksum) {
        return NULL; // 头校验和错误
    }

    SimpleFrame* frame = create_frame(id, type, &buffer[8], len);
    if (!frame) return NULL;

    if (calculate_data_checksum(frame->data, len) != buffer[8 + len]) {
        destroy_frame(frame);
        return NULL; // 数据校验和错误
    }

    return frame;
}
//计算头检验和
uint8_t calculate_header_checksum(uint16_t id, uint16_t len, uint16_t type) {
    int8_t checksum = 0;

    // XOR SOF
    checksum ^= SOF;

    // ID
    checksum ^= (id >> 8) & 0xFF;  // ID 高字节
    checksum ^= id & 0xFF;          // ID 低字节

    // LEN
    checksum ^= (len >> 8) & 0xFF;  // LEN 高字节
    checksum ^= len & 0xFF;         // LEN 低字节

    // TYPE
    checksum ^= (type >> 8) & 0xFF; // TYPE 高字节
    checksum ^= type & 0xFF;        // TYPE 低字节

    // 按位取反
    checksum = ~checksum;

    return checksum;
}

//计算data校验和
unsigned char calculate_data_checksum(unsigned char *data, unsigned char len) {
    unsigned char ret = 0;

    for (int i = 0; i < len; i++) {
        ret ^= data[i];
    }

    ret = ~ret;
    return ret;
}