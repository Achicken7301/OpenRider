/**
 * @file wireless_comm.h
 * @author Khang (buiankhang130301@gmail.com)
 * @brief This is an interface for other wireless communication (NRF24 or LORA, you implement it on your own)
 * @version 0.1
 * @date 2025-07-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef _WIRELESS_COMM_H_
#define _WIRELESS_COMM_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "simple_log.h"
#include "config.h"

extern char *WIRELESS_TAG;

// You can modify this size which
#define PAYLOAD_SIZE 256
#define WL_CMD_ENUM 10
#define MAC_STR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_SRC(m) (m)[0], (m)[1], (m)[2], \
                   (m)[3], (m)[4], (m)[5]

typedef enum
{
  WL_ERR_OK,
  WL_ERR_NO_COMM_TYPE, // Can not find communication
  WL_ERR_NULL,
} wireless_Err_t;

typedef enum
{
  CMD_ADD_PEER = WL_CMD_ENUM,
  CMD_UPDATE_RSSI,
} wireless_cmd_t;

typedef struct
{
  wireless_cmd_t cmd;
  uint8_t payload[PAYLOAD_SIZE];
  uint8_t src_mac[6];
  signed rssi : 8;
  uint8_t last_hop[6];
  uint8_t ttl;
  uint16_t seq_num;
} wireless_packet_t;

extern bool cmd_add_peer_flag;

typedef struct
{
  wireless_comm_type_t type;
  wireless_Err_t (*send_cb)(wireless_packet_t *);
  wireless_Err_t (*send)(wireless_packet_t *);
  wireless_Err_t (*receive_cb)(wireless_packet_t *);
  wireless_Err_t (*receive)(wireless_packet_t *);
  wireless_packet_t (*get_rcv_pkg)();
  void (*set_rcv_pkg)(wireless_packet_t *);
} wireless_comm_config_t;

// This is function pointer - still variable so we use "extern" like public api
extern wireless_Err_t (*wireless_send)(wireless_packet_t *);
extern wireless_Err_t (*wireless_receive)(wireless_packet_t *);
extern wireless_Err_t (*wireless_send_cb)(wireless_packet_t *);
extern wireless_Err_t (*wireless_receive_cb)(wireless_packet_t *);
extern wireless_packet_t (*wireless_get_receive_packet)();
extern void (*wireless_set_receive_packet)(wireless_packet_t *);

wireless_Err_t wireless_init(wireless_comm_config_t *);

#endif