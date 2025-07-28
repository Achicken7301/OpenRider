#include "network_espnow_mesh.h"

peer_recv_info_t temp_recv = {0};
void _network_espnow_mesh_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
void _network_espnow_mesh_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len);



void network_espnow_mesh_init(void)
{
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(NETWORK_CHANNEL, WIFI_SECOND_CHAN_NONE));
#if NETWORK_LONGRANGE
  ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_send_cb(_network_espnow_mesh_send_cb));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(_network_espnow_mesh_recv_cb));

  set_brcst_fsm_state(MSH_BROADCAST_INIT);
}

void network_espnow_mesh_deinit(void);

void _network_espnow_mesh_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // ESP_LOGI("Network ESPNOW", "Send callback from: %02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2],
  //          mac_addr[3], mac_addr[4], mac_addr[5]);
  // ESP_LOGI("Network ESPNOW", "Send status: %s\n", (status == ESP_NOW_SEND_SUCCESS) ? "Success" : "Fail");
}

void _network_espnow_mesh_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
  // Check if the received data is a command
  if (len == sizeof(MshBroadcastCommand_t))
  {
    // memcpy(temp_recv.p.peerInfo.peer_addr, recv_info->src_addr, ESP_NOW_ETH_ALEN);
    // memcpy(&temp_recv.p.rssi, recv_info->rx_ctrl->rssi, sizeof(recv_info->rx_ctrl->rssi));
    // memcpy(&temp_recv.data, data, sizeof(MshBroadcastCommand_t));
    // set_brcst_fsm_state(MSH_BROADCAST_RECEIVE);
  }

  // Handle other types of received data here if needed
}

void network_add_peer(peer_broadcast_info_t *peerInfo)
{
  esp_err_t r = esp_now_add_peer(&peerInfo->peerInfo);
  switch (r)
  {
  case ESP_ERR_ESPNOW_EXIST:
    ESP_LOGI("Network ESPNOW", "Node already exists");
    break;

  case ESP_OK:
  {
    // if (memcmp(peerInfo->peerInfo.peer_addr, BROADCAST_ADDR, ESP_NOW_ETH_ALEN) == 0)
    if (PEERS[BROADCAST_INDEX].isRegister == 0)
    {
      ESP_LOGI("Network ESPNOW", "Broadcast peer added");
      PEERS[BROADCAST_INDEX].isActive = 1;
      PEERS[BROADCAST_INDEX].isRegister = 1;
      memcpy(PEERS[BROADCAST_INDEX].peerInfo.peer_addr, BROADCAST_ADDR, ESP_NOW_ETH_ALEN);
      return;
    }

    uint8_t i = PEER_INDEX;
    while (PEERS[i].isRegister)
    {
      i++;
      if (i >= MAX_PEERS)
      {
        ESP_LOGE("Network ESPNOW", "No space for new peer");
        return;
      }
    }

    PEERS[i].isRegister = 1;
    memcpy(PEERS[i].peerInfo.peer_addr, peerInfo->peerInfo.peer_addr, ESP_NOW_ETH_ALEN);
    ESP_LOGI("Network ESPNOW", "Node added at index %d with MAC: %02x:%02x:%02x:%02x:%02x:%02x",
             i,
             PEERS[i].peerInfo.peer_addr[0], PEERS[i].peerInfo.peer_addr[1], PEERS[i].peerInfo.peer_addr[2],
             PEERS[i].peerInfo.peer_addr[3], PEERS[i].peerInfo.peer_addr[4], PEERS[i].peerInfo.peer_addr[5]);
  }
  break;

  default:
    ESP_LOGE("Network ESPNOW", "Failed to add node: %s", esp_err_to_name(r));
    break;
  }
}