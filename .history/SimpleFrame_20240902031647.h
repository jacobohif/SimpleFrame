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
    uint16_t id;          // 帧 ID
    uint16_t len;         // 数据帧长度（不包括头部）
    uint16_t type;        // 帧类型
    uint8_t head_cksum;   // 头校验和
    uint8_t* data;        // 数据（长度可变）
    uint8_t data_cksum;   // 数据校验和
} SimpleFrame;

// 创建一个新的帧
SimpleFrame* create_frame(uint16_t id, uint16_t type, const uint8_t* data, uint16_t data_len);

// 销毁帧
void destroy_frame(SimpleFrame* frame);

// 序列化帧到字节数组
uint8_t* serialize_frame(SimpleFrame* frame, size_t* frame_size);

// 反序列化字节数组到帧
SimpleFrame* deserialize_frame(const uint8_t* buffer, size_t buffer_len);

// 计算头校验和
uint8_t calculate_header_checksum(uint16_t id, uint16_t len, uint16_t type);

// 计算数据校验和
//uint8_t calculate_data_checksum(const uint8_t* data, uint16_t data_len);


unsigned char calculate_data_checksum(unsigned char *data, unsigned char len);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_FRAME_H
