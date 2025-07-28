#include "espnow_driver.h"

wireless_Err_t espnow_send_cb(wireless_packet_t *);
wireless_Err_t espnow_receive_cb(wireless_packet_t *);
wireless_Err_t espnow_send(wireless_packet_t *);
wireless_Err_t espnow_receive(wireless_packet_t *);

void _espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}
void _espnow_rcv_cb(const esp_now_recv_info_t *esp_now_info,
                         const uint8_t *data, int data_len)
{
  // // Receive with command
  // if (data_len == sizeof(wireless_cmd_t))
  // {
  //   wireless_cmd_t cmd = (wireless_cmd_t)(*data);
  //   switch (cmd)
  //   {
  //   case CMD_ADD_PEER:
  //   {
  //     PM_add_peer(esp_now_info->src_addr, esp_now_info->rx_ctrl->rssi);
  //     esp_now_peer_info_t new_p = {
  //         .channel = CHANNEL_FOR_BRSTER,
  //         .encrypt = false,
  //         .ifidx = WIFI_MODE_STA,
  //     };
  //     memcpy(new_p.peer_addr, esp_now_info->src_addr, sizeof(new_p.peer_addr));
  //     esp_now_add_peer(&new_p);
  //     break;
  //   }

  //   default:
  //     break;
  //   }
  // }

  // // Receive with audio
}

void espnow_driver_init()
{
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(CHANNEL_FOR_BRSTER, WIFI_SECOND_CHAN_NONE));
#if NETWORK_LONGRANGE
  ESP_ERROR_CHECK(esp_wifi_set_protocol(
      WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N |
                       WIFI_PROTOCOL_LR));
#endif
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(_espnow_send_cb));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(_espnow_rcv_cb));
  wireless_comm_config_t wl_cfg = {
      .type = ESPNOW_COMM,
      .receive = espnow_receive,
      .receive_cb = espnow_receive_cb,
      .send = espnow_send,
      .send_cb = espnow_send_cb};

  wireless_init(&wl_cfg);
}
