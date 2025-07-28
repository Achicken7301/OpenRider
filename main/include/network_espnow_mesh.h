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
#include "broadcast_fsm.h"

extern peer_recv_info_t temp_recv;

void network_espnow_mesh_init(void);
void network_espnow_mesh_deinit(void);
void network_add_peer(peer_broadcast_info_t *);

#endif