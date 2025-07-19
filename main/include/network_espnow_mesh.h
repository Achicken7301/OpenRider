#ifndef __NK_ESPNOW_MESH_H__
#define __NK_ESPNOW_MESH_H__

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "esp_now.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_err.h"

#define MSH_BROADCAST_ENUM 10
#define MSH_BROADCAST_ERR_ENUM (MSH_BROADCAST_ENUM + 10)
#define MSH_BRCST_CMD_ENUM (MSH_BROADCAST_ERR_ENUM + 10)
#define MAX_PEERS 20
#define NETWORK_CHANNEL 6
#define NETWORK_LONGRANGE 0
#define BROADCAST_INDEX 0
#define PEER_INDEX 1
#define BROADCAST_ADDR "\xFF\xFF\xFF\xFF\xFF\xFF"
#define PEER_BRCST_INFO_DEFAULT() {      \
    .peerInfo.channel = NETWORK_CHANNEL, \
    .peerInfo.encrypt = false,           \
    .peerInfo.ifidx = ESP_IF_WIFI_STA,   \
}

typedef struct
{
  esp_now_peer_info_t peerInfo; // ESPNOW peer information
  signed rssi : 8;              // Received Signal Strength Indicator
  int isActive;                 // Indicates if the peer is active
  int isRegister;               // Indicates if the peer is registered
} peer_broadcast_info_t;
extern peer_broadcast_info_t PEERS[MAX_PEERS];

typedef struct
{
  peer_broadcast_info_t p;
  uint8_t *data;
  int len;
} peer_recv_info_t;
extern peer_recv_info_t temp_recv;

typedef enum
{
  MSH_BROADCAST_INIT = MSH_BROADCAST_ENUM,
  MSH_BROADCAST_IDLE,
  MSH_BROADCAST_SEND, // Send order to add peer
  MSH_BROADCAST_RECEIVE,
  MSH_BROADCAST_UPDATE_RSSI,
  MSH_BROADCAST_ERROR,
} MshBroadcastFSM_t;

typedef enum
{
  MSH_BRCST_CMD_ADD_PEER = MSH_BRCST_CMD_ENUM,
} MshBroadcastCommand_t;

typedef enum
{
  MSH_BROADCAST_ERR_OK = MSH_BROADCAST_ERR_ENUM,
  MSH_BROADCAST_ERR_NOT_OK,
  MSH_BROADCAST_ERR_TIMEOUT,
  MSH_BROADCAST_ERR_INVALID_STATE,
} espnow_mesh_err_t;

MshBroadcastFSM_t get_brcst_fsm_state(void);
void set_brcst_fsm_state(MshBroadcastFSM_t new_state);
void MSH_Broadcast_FSM(void);
void network_espnow_mesh_init(void);
void network_espnow_mesh_deinit(void);
void network_add_peer(peer_broadcast_info_t *);

#endif