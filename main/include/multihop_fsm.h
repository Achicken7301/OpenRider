#ifndef __MULTIHOP_FSM_H__
#define __MULTIHOP_FSM_H__

#include "stdio.h"
#include "stdint.h"
#include "broadcast_fsm.h"

#define MAX_CACHE_SIZE 64
#define MAX_PAYLOAD 64

typedef enum{
  MH_INIT,
  MH_SEND,
  MH_RECV,
} multihop_fsm_t;

typedef struct
{
  uint8_t src_mac[6];           // Địa chỉ MAC nguồn (node khởi tạo gói)
  uint16_t seq_num;             // Số thứ tự gói từ nguồn
  uint8_t last_hop[6];          // Node gần nhất gửi gói này
  uint8_t ttl;                  // Time To Live để giới hạn hop
  uint8_t payload[MAX_PAYLOAD]; // Dữ liệu thực (audio, v.v.)
} packet_t;

typedef struct
{
  uint8_t src_mac[6];
  uint16_t seq_num;
  uint16_t timetstamp;
} packet_cache_entry_t;

packet_cache_entry_t packet_cache[MAX_CACHE_SIZE];

#endif