#include "intercom_fsm.h"
char *INTERCOM_TAG = "INTERCOM";
intercom_fsm_t intercom_state = I_INIT;

void intercom_set_fsm(intercom_fsm_t next_state)
{
  intercom_state = next_state;
}

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
  LOGI(INTERCOM_TAG, "brst counter: %d", brst_update_peer_counter);

  wireless_packet_t pkg = {.cmd = CMD_ADD_PEER};
  memcpy(pkg.src_mac, BROADCAST_ADDR, 6);
  wireless_send(&pkg);
}

void send_audio_example_periodic()
{
  // intercom_set_fsm(I_SEND);
  new_audio_flag = 1;
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
    // SCH_Add(_broadcast_update_peer, 5000, 1000);
    // SCH_Add(PM_list_peer, 30000, 5000);
    SCH_Add(send_audio_example_periodic, 10000, 5000);
    // intercom_set_fsm(I_DISCOVER);
    intercom_set_fsm(I_SEND);
    break;

  case I_DISCOVER:
  {
    PM_update_peer_list();

    // Update Rssi of every peers
    break;
  }

  case I_IDLE:
    break;

  case I_SEND:
  {
    if (new_audio_flag == 1)
    {
      LOGI(INTERCOM_TAG, "Send example audio");
      uint8_t *audio_example = get_audio_example();
      wireless_packet_t audio_packet_example = {.cmd = CMD_NONE, .ttl = 5, .payload_len = sizeof(audio_example)};
      memcpy(&audio_packet_example.payload, audio_example, 256);

      // Get peers which is active
      // loop through each peers, send data
      memcpy(audio_packet_example.src_mac, BROADCAST_ADDR, 6);
      wireless_send(&audio_packet_example);
      new_audio_flag = 0;
      // free(audio_example);
    }
    intercom_set_fsm(I_RECEIVE);

    break;
  }

  case I_RECEIVE:
  {
    intercom_set_fsm(I_SEND);
    if (receive_audio_data_flag == 1)
    {
      wireless_packet_t pkg = wireless_get_receive_packet();
      LOGI(INTERCOM_TAG, "Receive from: " MAC_STR, MAC_SRC(pkg.src_mac));
      for (int i = 0; i < 256; i += 8)
      {
        LOGI(INTERCOM_TAG, "\t%02x %02x %02x %02x %02x %02x %02x %02x", pkg.payload[i],
             pkg.payload[i + 1], pkg.payload[i + 2], pkg.payload[i + 3], pkg.payload[i + 4],
             pkg.payload[i + 5], pkg.payload[i + 6], pkg.payload[i + 7]);
      }

      receive_audio_data_flag = 0;
    }

    break;
  }

  default:
    break;
  }
}
