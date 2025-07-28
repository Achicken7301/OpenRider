#ifndef _PEER_MANAGER_H_
#define _PEER_MANAGER_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "wireless_comm.h"
#include "scheduler.h"
#include "simple_log.h"

#define PEER_INDEX 1
#define BROADCAST_INDEX 0
#define BROADCAST_MAX_COUNTER 30
extern const uint8_t BROADCAST_ADDR[6];

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

#define MAX_PEERS 20
extern PeerEntry peer_table[MAX_PEERS];

void PM_init();
void PM_add_peer(uint8_t *mac, int rssi);
PM_Err_t PM_get_peer(uint8_t *, PeerEntry *);
PM_Err_t PM_isEmpty();
void PM_update_peer_list();

#endif