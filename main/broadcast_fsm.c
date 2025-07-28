#include "network_espnow_mesh.h"

peer_broadcast_info_t PEERS[MAX_PEERS] = {0};
static MshBroadcastFSM_t current_fsm_state = MSH_BROADCAST_IDLE;

MshBroadcastFSM_t get_brcst_fsm_state(void)
{
  return current_fsm_state;
}

void set_brcst_fsm_state(MshBroadcastFSM_t new_state)
{
  current_fsm_state = new_state;
}

void MSH_Broadcast_FSM(void)
{
  switch (get_brcst_fsm_state())
  {
  case MSH_BROADCAST_INIT:
    // Create peer with broadcast address
    peer_broadcast_info_t temp = {
        .peerInfo = {
            .peer_addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // Broadcast address
            .channel = NETWORK_CHANNEL,
            .ifidx = ESP_IF_WIFI_STA,
            .encrypt = false},
        .rssi = 0,
        .isActive = 1,
        .isRegister = 1};

    network_add_peer(&temp);
    set_brcst_fsm_state(MSH_BROADCAST_IDLE);
    break;

  case MSH_BROADCAST_IDLE:
    // Idle logic
    break;

  case MSH_BROADCAST_SEND_CMD_ADD_PEER:
    MshBroadcastCommand_t msg = MSH_BRCST_CMD_ADD_PEER;
    ESP_ERROR_CHECK(esp_now_send(PEERS[BROADCAST_INDEX].peerInfo.peer_addr, (uint8_t *)&msg, sizeof(msg)));
    set_brcst_fsm_state(MSH_BROADCAST_IDLE);
    break;

  case MSH_BROADCAST_RECEIVE:
  {
    MshBroadcastCommand_t cmd = (MshBroadcastCommand_t)temp_recv.data;
    switch (cmd)
    {
    case MSH_BRCST_CMD_ADD_PEER:
      set_brcst_fsm_state(MSH_BROADCAST_ADD_PEER);
      break;
    case MSH_BRCST_CMD_GET_RSSI:
      set_brcst_fsm_state(MSH_BRCST_CMD_GET_RSSI);
      break;

    default:
      ESP_LOGW("Network ESPNOW", "Unknown command received: %d", cmd);
      set_brcst_fsm_state(MSH_BROADCAST_IDLE);
      break;
    }
  }

  case MSH_BROADCAST_ADD_PEER:
  {
    ESP_LOGI("Network ESPNOW", "Received command to add peer");
    esp_now_peer_info_t peerInfo;
    esp_err_t result = esp_now_get_peer(temp_recv.p.peerInfo.peer_addr, &peerInfo);
    switch (result)
    {
    case ESP_OK:
      ESP_LOGI("Network ESPNOW", "Peer already exists");
      break;

    case ESP_ERR_ESPNOW_NOT_FOUND:
    {
      peer_broadcast_info_t temp = PEER_BRCST_INFO_DEFAULT();
      memcpy(temp.peerInfo.peer_addr, temp_recv.p.peerInfo.peer_addr, ESP_NOW_ETH_ALEN);
      network_add_peer(&temp);
    }
    break;

    default:
      ESP_LOGE("Network ESPNOW", "Failed to get peer: %s", esp_err_to_name(result));
      break;
    }
    break;

    set_brcst_fsm_state(MSH_BROADCAST_IDLE);
    break;
  }

  case MSH_BROADCAST_UPDATE_RSSI:
  {
    MshBroadcastCommand_t cmd = MSH_BRCST_CMD_GET_RSSI;
    esp_now_send(BROADCAST_ADDR, &cmd, sizeof(MshBroadcastCommand_t));
    set_brcst_fsm_state(MSH_BROADCAST_IDLE);
    break;
  }

  case MSH_BROADCAST_ERROR:
    // Error handling logic
    set_brcst_fsm_state(MSH_BROADCAST_INIT);
    break;

  default:
    set_brcst_fsm_state(MSH_BROADCAST_ERROR);
    break;
  }
}