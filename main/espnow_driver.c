#include "espnow_driver.h"

int receive_audio_data_flag = 0;
const uint8_t BROADCAST_ADDR[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

wireless_packet_t rcv_pkg = {0};

void espnow_set_receive_packet(wireless_packet_t *pkg)
{
}

wireless_packet_t espnow_get_receive_packet()
{
  return rcv_pkg;
}

wireless_Err_t espnow_send_cb(wireless_packet_t *pkg)
{
  return WL_ERR_OK;
}

wireless_Err_t espnow_receive_cb(wireless_packet_t *pkg)
{
  // Should add to task and let the main() handle it
  // I'll implement this later
  return WL_ERR_OK;
}

wireless_Err_t espnow_send(wireless_packet_t *pkg)
{
  ESP_ERROR_CHECK(esp_now_send(pkg->src_mac, (uint8_t *)pkg, sizeof(wireless_packet_t)));

  return WL_ERR_OK;
}

wireless_Err_t espnow_receive(wireless_packet_t *pkg)
{

  return WL_ERR_OK;
}

void _espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  LOGI(WIRELESS_TAG, "Sent pkg %s MAC: %02x:%02x:%02x:%02x:%02x:%02x", (status == ESP_NOW_SEND_SUCCESS) ? "Successfully" : "Fail", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void _espnow_rcv_cb(const esp_now_recv_info_t *esp_now_info,
                    const uint8_t *data, int data_len)
{
  memset(&rcv_pkg, 0, sizeof(rcv_pkg));
  memcpy(&rcv_pkg, data, sizeof(wireless_packet_t));
  rcv_pkg.rssi = esp_now_info->rx_ctrl->rssi;
  memcpy(&rcv_pkg.src_mac, esp_now_info->src_addr, 6);

  if (rcv_pkg.cmd != CMD_NONE)
  {
    // LOGI(WIRELESS_TAG, "Receive cmd from " MAC_STR, rcv_pkg.src_mac[0], rcv_pkg.src_mac[1], rcv_pkg.src_mac[2], rcv_pkg.src_mac[3], rcv_pkg.src_mac[4], rcv_pkg.src_mac[5]);
    switch (rcv_pkg.cmd)
    {
    case CMD_ADD_PEER:
    {
      // LOGI(WIRELESS_TAG, "The cmd: add peer");
      esp_now_peer_info_t p = PEER_BRCST_INFO_DEFAULT();
      memcpy(p.peer_addr, rcv_pkg.src_mac, 6);
      esp_err_t r = esp_now_add_peer(&p);
      if (r == ESP_OK)
      {
        cmd_add_peer_flag = 1; // this flag for "other modules" (peer_manager) to communicate
      }
      break;
    }

    default:
      LOGE(WIRELESS_TAG, "Unknown cmd %d", rcv_pkg.cmd);
      break;
    }
    return;
  }

  // Receive audio packet

  receive_audio_data_flag = 1;
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
      .receive = &espnow_receive,
      .receive_cb = &espnow_receive_cb,
      .send = &espnow_send,
      .send_cb = &espnow_send_cb,
      .get_rcv_pkg = &espnow_get_receive_packet,
      .set_rcv_pkg = &espnow_set_receive_packet,
  };

  wireless_init(&wl_cfg);

  esp_now_peer_info_t brster = PEER_BRCST_INFO_DEFAULT();
  memcpy(brster.peer_addr, BROADCAST_ADDR, 6);
  esp_err_t r = esp_now_add_peer(&brster);
  LOGI(WIRELESS_TAG, "Add broadcast peer %s", (r == WL_ERR_OK) ? "Successfull" : "Fail");
}
