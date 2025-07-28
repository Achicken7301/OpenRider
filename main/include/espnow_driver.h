#ifndef ESPNOW_DRIVER_H
#define ESPNOW_DRIVER_H

#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "nvs_flash.h"



#include "wireless_comm.h"
#include "peer_manager.h"
#include "config.h"

#define CHANNEL_FOR_BRSTER 6

void espnow_driver_init();

wireless_Err_t espnow_send_cb(wireless_packet_t *);
wireless_Err_t espnow_receive_cb(wireless_packet_t *);
wireless_Err_t espnow_send(wireless_packet_t *);
wireless_Err_t espnow_receive(wireless_packet_t *);

#endif // ESPNOW_DRIVER_H