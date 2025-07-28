#include "peer_manager.h"
char *PM_TAG = "PEER_MANAGER";

const uint8_t BROADCAST_ADDR[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

PeerEntry peer_table[MAX_PEERS] = {0};

void PM_update_peer_list()
{
  static uint8_t broadcast_add_peer_count = 0; // 30 times
  if (broadcast_add_peer_count >= BROADCAST_MAX_COUNTER)
  {
    LOGI(PM_TAG, "Last update peer list time");
    broadcast_add_peer_count = BROADCAST_MAX_COUNTER;
    SCH_Delete(SCH_Get(PM_update_peer_list));
    return;
  }
  broadcast_add_peer_count++;
  wireless_packet_t pkt = {
      .cmd = CMD_ADD_PEER};

  // wireless_send(&pkt);
}

void PM_add_peer(uint8_t *mac, int rssi)
{
  int i = PEER_INDEX;
  while (peer_table[i].isActive)
  {
    i++;
    if (i > MAX_PEERS)
    {
      LOGI(PM_TAG, "Peer table is full");
      return;
    }
  }

  peer_table[i].isActive = 1;
  memcpy(peer_table[i].mac, mac, sizeof(mac));
  peer_table[i].rssi = rssi;
}