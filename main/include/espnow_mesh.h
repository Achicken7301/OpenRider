#ifndef ESPNOW_MESH_H
#define ESPNOW_MESH_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "esp_now.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "driver/gpio.h"

#include "global.h"

#define ESP_NOW_CHANNEL 6
#define ESP_NOW_LONGRANGE 0
#define MAX_PEERS 20

#define ESPNOW_MESH_FSM_T_INIT 0
typedef enum
{
  ESPNOW_MESH_INIT = ESPNOW_MESH_FSM_T_INIT,
  ESPNOW_MESH_ADD_PEER,
  ESPNOW_MESH_BROADCAST,
  ESPNOW_MESH_SEND_BROADCAST,
  ESPNOW_MESH_SEND,
  ESPNOW_MESH_RECEIVE,
  ESPNOW_MESH_DO_NOTHING,
  ESPNOW_MESH_ERROR,
  ESPNOW_MESH_MAX
} espnow_mesh_fsm_t;

#define ESP_NOW_CODE_T_INIT 10
typedef enum
{
  C_BROADCAST_FOR_ADD_PEER = ESP_NOW_CODE_T_INIT, /**< Broadcast for adding a peer */

} esp_now_code;


void fsm_espnow_mesh(void);
void set_fsm_state(espnow_mesh_fsm_t new_state);
espnow_mesh_fsm_t get_fsm_state(void);

void espnow_mesh_add_peer(const uint8_t *peer_addr);
void espnow_mesh_list_peer(void);

void espnow_mesh_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
void espnow_mesh_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len);
#endif // ESPNOW_MESH_H