#include "intercom_fsm.h"

intercom_fsm_t intercom_state = I_INIT;

void _broadcast_update_peer()
{
  static uint8_t brst_update_peer_counter = 0;
  if (brst_update_peer_counter >= MAX_RECEIVE_CMD_TIME)
  {
    SCH_Delete(SCH_Get(_broadcast_update_peer));
    intercom_set_fsm(I_IDLE);
    return;
  }
  brst_update_peer_counter += 1;
  LOGI("INTERCOM", "brst counter: %d", brst_update_peer_counter);

  wireless_packet_t pkg = {
    .payload = "12345",
    .last_hop = "130301",
  };
  pkg.cmd = CMD_ADD_PEER;
  memcpy(pkg.src_mac, BROADCAST_ADDR, 6);
  wireless_send(&pkg);
}

void IntercomFSM()
{
  switch (intercom_state)
  {
  case I_INIT:

    // ONLY use driver in here
    // nrf24_driver_init();
    // lora_driver_init();
    espnow_driver_init();
    SCH_Add(_broadcast_update_peer, 5000, 1000);
    SCH_Add(PM_list_peer, 30000, 5000);
    intercom_set_fsm(I_DISCOVER);
    break;

  case I_DISCOVER:
  {
    PM_update_peer_list();
    break;
  }

  case I_IDLE:
    break;

  case I_SEND:
    break;

  default:
    break;
  }
}

void intercom_set_fsm(intercom_fsm_t next_state)
{
  intercom_state = next_state;
}
