#include "espnow_mesh.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t ESPNOW_MESH_PEERS_LIST[MAX_PEERS] = {0};
espnow_mesh_fsm_t current_fsm_state = ESPNOW_MESH_INIT;

bool send_flag = false;

char *get_fsm_state_name(espnow_mesh_fsm_t state)
{
  switch (state)
  {
  case ESPNOW_MESH_INIT:
    return "ESPNOW_MESH_INIT";
  case ESPNOW_MESH_ADD_PEER:
    return "ESPNOW_MESH_ADD_PEER";
  case ESPNOW_MESH_BROADCAST:
    return "ESPNOW_MESH_BROADCAST";
  case ESPNOW_MESH_SEND_BROADCAST:
    return "ESPNOW_MESH_SEND_BROADCAST";
  case ESPNOW_MESH_SEND:
    return "ESPNOW_MESH_SEND";
  case ESPNOW_MESH_RECEIVE:
    return "ESPNOW_MESH_RECEIVE";
  case ESPNOW_MESH_DO_NOTHING:
    return "ESPNOW_MESH_DO_NOTHING";
  case ESPNOW_MESH_ERROR:
    return "ESPNOW_MESH_ERROR";
  default:
    return "UNKNOWN_STATE";
  }
}

void fsm_espnow_mesh()
{
  switch (current_fsm_state)
  {
  case ESPNOW_MESH_INIT:
  {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(ESP_NOW_CHANNEL, WIFI_SECOND_CHAN_NONE));
#if LONGRANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_mesh_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_mesh_recv_cb));
    set_fsm_state(ESPNOW_MESH_DO_NOTHING);
    break;
  }

  case ESPNOW_MESH_BROADCAST:
  {
    ESP_LOGI(ESPNOW_MESH_TAG, "Broadcasting to add peer");
    esp_now_peer_info_t peerInfo = {
        .channel = ESP_NOW_CHANNEL, // Set the channel to the same as Wi-Fi channel
        .ifidx = ESP_IF_WIFI_STA,
        .encrypt = false,
    };

    memcpy(peerInfo.peer_addr, broadcastAddress, ESP_NOW_ETH_ALEN);

    esp_err_t r = esp_now_get_peer(broadcastAddress, &peerInfo);
    if (r == ESP_OK)
    {
      ESP_LOGI(ESPNOW_MESH_TAG, "Broadcast peer already exists");
    }
    else
    {
      ESP_LOGI(ESPNOW_MESH_TAG, "Adding broadcast peer");
      esp_now_add_peer(&peerInfo);
    }

    set_fsm_state(ESPNOW_MESH_SEND_BROADCAST);
    break;
  }

  case ESPNOW_MESH_SEND_BROADCAST:
  {
    esp_now_code message = C_BROADCAST_FOR_ADD_PEER;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    if (result == ESP_OK)
    {
      ESP_LOGI(ESPNOW_MESH_TAG, "Send broadcast with add peer code");
    }
    else
    {
      ESP_LOGE(ESPNOW_MESH_TAG, "Failed to send broadcast message: %s", esp_err_to_name(result));
    }
  }
    set_fsm_state(ESPNOW_MESH_DO_NOTHING);
    break;

  case ESPNOW_MESH_SEND:
    break;

  case ESPNOW_MESH_RECEIVE:
    /* code */
    break;
  case ESPNOW_MESH_DO_NOTHING:
    /* code */
    break;
  default:
    break;
  }
}

/**
 * @brief Set the current FSM state.
 * @param new_state The new FSM state to set.
 */
void set_fsm_state(espnow_mesh_fsm_t new_state)
{
  ESP_LOGI(ESPNOW_MESH_TAG, "Changing FSM state from %s to %s", get_fsm_state_name(current_fsm_state), get_fsm_state_name(new_state));
  current_fsm_state = new_state;
}

espnow_mesh_fsm_t get_fsm_state(void)
{
  return current_fsm_state;
}

void espnow_mesh_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  send_flag = 1;
  // ESP_LOGI(ESPNOW_MESH_TAG, "Send callback from: %02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2],
  //          mac_addr[3], mac_addr[4], mac_addr[5]);
  // ESP_LOGI(ESPNOW_MESH_TAG, "Send status: %s\n", (status == ESP_NOW_SEND_SUCCESS) ? "Success" : "Fail");
}

/**
 * @brief Callback function for receiving ESPNOW data.
 * @param recv_info Information about the received data, including source address.
 * @param data Pointer to the received data.
 * @param len Length of the received data.
 */
void espnow_mesh_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
  if (len == sizeof(esp_now_code))
  {
    // Get data value and store into message
    esp_now_code message = *(esp_now_code *)data;
    switch (message)
    {
    case C_BROADCAST_FOR_ADD_PEER:
    {
      // check if peer already exists
      esp_now_peer_info_t peerInfo = {};
      esp_err_t result = esp_now_get_peer(recv_info->src_addr, &peerInfo);
      switch (result)
      {
      case ESP_OK:
        ESP_LOGI(ESPNOW_MESH_TAG, "Peer already exists");
        break;

      case ESP_ERR_ESPNOW_NOT_FOUND:
        espnow_mesh_add_peer(recv_info->src_addr);
        break;
      default:
        ESP_LOGE(ESPNOW_MESH_TAG, "Failed to get peer: %s", esp_err_to_name(result));
        break;
      }
    }
    break;
    default:
      break;
    }
  }
}

void espnow_mesh_add_peer(const uint8_t *peer_addr)
{
  if (peer_addr == NULL)
  {
    ESP_LOGE(ESPNOW_MESH_TAG, "Peer address is NULL");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
  peerInfo.channel = ESP_NOW_CHANNEL;
  peerInfo.ifidx = ESP_IF_WIFI_STA;
  peerInfo.encrypt = false;

  esp_err_t result = esp_now_add_peer(&peerInfo);
  switch (result)
  {
  case ESP_OK:
    ESP_LOGI(ESPNOW_MESH_TAG, "Added peer %02x:%02x:%02x:%02x:%02x:%02x",
             peer_addr[0], peer_addr[1], peer_addr[2],
             peer_addr[3], peer_addr[4], peer_addr[5]);
    break;

  case ESP_ERR_ESPNOW_FULL:
  {
    ESP_LOGE(ESPNOW_MESH_TAG, "Peer list is full");
    break;
  }

  default:
  {
    ESP_LOGE(ESPNOW_MESH_TAG, "Failed to add peer: %s", esp_err_to_name(result));
    break;
  }
  }

  // Add new peer to ESPNOW_MESH_PEERS_LIST
  for (int i = 0; i < MAX_PEERS; i++)
  {
    if (memcmp(ESPNOW_MESH_PEERS_LIST[i].peer_addr, peer_addr, ESP_NOW_ETH_ALEN) == 0)
    {
      ESP_LOGI(ESPNOW_MESH_TAG, "Peer already exists in ESPNOW_MESH_PEERS_LIST");
      return; // Peer already exists
    }
    if (ESPNOW_MESH_PEERS_LIST[i].peer_addr[0] == 0)
    { // Empty slot found
      memcpy(ESPNOW_MESH_PEERS_LIST[i].peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
      ESPNOW_MESH_PEERS_LIST[i].channel = ESP_NOW_CHANNEL;
      ESPNOW_MESH_PEERS_LIST[i].ifidx = ESP_IF_WIFI_STA;
      ESPNOW_MESH_PEERS_LIST[i].encrypt = false;
      ESP_LOGI(ESPNOW_MESH_TAG, "Added peer to ESPNOW_MESH_PEERS_LIST at index %d", i);
      return;
    }
  }
}

void espnow_mesh_list_peer()
{
  ESP_LOGI(ESPNOW_MESH_TAG, "Listing all peers in CARDO_ESPNOW_PEERS_LIST");
  for (int i = 0; i < MAX_PEERS; i++)
  {
    if (ESPNOW_MESH_PEERS_LIST[i].peer_addr[0] != 0)
    { // Check if peer exists
      ESP_LOGI(ESPNOW_MESH_TAG, "Peer %d: %02x:%02x:%02x:%02x:%02x:%02x",
               i,
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[0],
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[1],
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[2],
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[3],
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[4],
               ESPNOW_MESH_PEERS_LIST[i].peer_addr[5]);
    }
  }
}