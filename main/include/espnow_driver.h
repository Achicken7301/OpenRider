#ifndef ESPNOW_DRIVER_H
#define ESPNOW_DRIVER_H

#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "nvs_flash.h"

#include "wireless_comm.h"
#include "config.h"

#define DEFAULT_BROADCAST_CHANNEL 6
#define CHANNEL_FOR_BRSTER 6
extern const uint8_t BROADCAST_ADDR[6];

#define PEER_BRCST_INFO_DEFAULT() {       \
    .channel = DEFAULT_BROADCAST_CHANNEL, \
    .encrypt = false,                     \
    .ifidx = ESP_IF_WIFI_STA,             \
}

void espnow_driver_init();

/**
 * @brief Callback function should be small
 *
 * @return wireless_Err_t
 */
wireless_Err_t espnow_send_cb(wireless_packet_t *);
wireless_Err_t espnow_receive_cb(wireless_packet_t *);

wireless_Err_t espnow_send(wireless_packet_t *);
wireless_Err_t espnow_receive(wireless_packet_t *);

wireless_Err_t espnow_receive_cmd_add_peer(wireless_packet_t *);

/**
 * @brief This API is used by peer_manager
 *
 * @return wireless_packet_t
 */
wireless_packet_t espnow_get_receive_packet();

/**
 * @brief This API is used by peer_manager
 * 
 */
void espnow_set_receive_packet(wireless_packet_t *);

#endif // ESPNOW_DRIVER_H