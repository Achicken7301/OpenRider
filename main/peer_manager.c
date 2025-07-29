#include "peer_manager.h"
char *PM_TAG = "PEER_MANAGER";

PeerEntry peer_table[MAX_PEERS] = {0};

void PM_update_peer_list()
{
  if (cmd_add_peer_flag == 1)
  {
    wireless_packet_t p = wireless_get_receive_packet();
    LOGI(PM_TAG, "Receive package from %02x:%02x:%02x:%02x:%02x:%02x, with cmd add peer", p.src_mac[0], p.src_mac[1], p.src_mac[2], p.src_mac[3], p.src_mac[4], p.src_mac[5]);

    PM_add_peer(p.src_mac, p.rssi);
    cmd_add_peer_flag = 0;
  }
}

void PM_add_peer(uint8_t *mac, int rssi)
{

  int empty_slot = -1;

  for (int i = 0; i < MAX_PEERS; i++)
  {
    if (peer_table[i].isActive)
    {
      if (memcmp(peer_table[i].mac, mac, 6) == 0)
      {
        return; // đã tồn tại
      }
    }
    else if (empty_slot == -1)
    {
      empty_slot = i; // lưu lại vị trí trống đầu tiên
    }
  }

  if (empty_slot != -1)
  {
    memcpy(peer_table[empty_slot].mac, mac, 6);
    peer_table[empty_slot].rssi = rssi;
    peer_table[empty_slot].isActive = 1;
    LOGI(PM_TAG, "Added MAC: "MAC_STR" to pos No.%d", MAC_SRC(peer_table[empty_slot].mac), empty_slot);
  }
}

void PM_list_peer()
{
  for (int i = 0; i < MAX_PEERS; i++)
  {
    if (peer_table[i].isActive == 0)
    {
      return;
    }
    LOGI(PM_TAG, "Peer No.%d, MAC: " MAC_STR ",Rssi: %d", i, MAC_SRC(peer_table[i].mac), peer_table[i].rssi);
  }
}