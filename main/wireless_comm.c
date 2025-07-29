#include "wireless_comm.h"
char *WIRELESS_TAG = "";
bool cmd_add_peer_flag = 0;

// Các con trỏ hàm public
wireless_Err_t (*wireless_send)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_receive)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_send_cb)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_receive_cb)(wireless_packet_t *) = 0;
wireless_packet_t (*wireless_get_receive_packet)() = 0;
void (*wireless_set_receive_packet)(wireless_packet_t *) = 0;

wireless_Err_t wireless_init(wireless_comm_config_t *cfg)
{
  if (!cfg)
    return WL_ERR_NULL;

  wireless_send = cfg->send;
  wireless_receive = cfg->receive;
  wireless_send_cb = cfg->send_cb;
  wireless_receive_cb = cfg->receive_cb;
  wireless_get_receive_packet = cfg->get_rcv_pkg;
  wireless_set_receive_packet = cfg->set_rcv_pkg;

  switch (cfg->type)
  {
  case ESPNOW_COMM:
  {
    WIRELESS_TAG = "ESPNOW_COMM";
    LOGI(WIRELESS_TAG, "Wireless driver: ESPNOW");
    return WL_ERR_OK;
    break;
  }

  default:
    WIRELESS_TAG = "UNKNOWN_COMM";
    LOGE(WIRELESS_TAG, "Unknown wireless driver %d", cfg->type);
    return WL_ERR_NO_COMM_TYPE;
    break;
  }
}