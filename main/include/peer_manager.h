#ifndef _PEER_MANAGER_H_
#define _PEER_MANAGER_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "wireless_comm.h"
#include "scheduler.h"
#include "simple_log.h"

#define BROADCAST_MAX_COUNTER 30
#define MAX_PEERS 10

extern char *PM_TAG;

typedef enum
{
  PM_OK,
  PM_PEER_NOT_FOUND,
  PM_TABLE_NOT_EMPTY,
} PM_Err_t;

typedef struct
{
  uint8_t mac[6];
  signed rssi : 8; /**< Received Signal Strength Indicator(RSSI) of packet. unit: dBm */
  bool isActive;
} PeerEntry;

extern PeerEntry peer_table[MAX_PEERS];

void PM_init();
void PM_add_peer(uint8_t *mac, int rssi);
PM_Err_t PM_get_peer(uint8_t *, PeerEntry *);
PM_Err_t PM_isEmpty();
void PM_update_peer_list();
void PM_list_peer();
PeerEntry *PM_get_active_peers();

#endif