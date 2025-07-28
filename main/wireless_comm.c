#include "wireless_comm.h"
char *WIRELESS_TAG = "";

// Các con trỏ hàm public
wireless_Err_t (*wireless_send)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_receive)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_send_cb)(wireless_packet_t *) = 0;
wireless_Err_t (*wireless_receive_cb)(wireless_packet_t *) = 0;

wireless_Err_t wireless_init(wireless_comm_config_t *cfg)
{
  if (!cfg) return WL_ERR_NULL;

  wireless_send        = cfg->send;
  wireless_receive     = cfg->receive;
  wireless_send_cb     = cfg->send_cb;
  wireless_receive_cb  = cfg->receive_cb;

  switch (cfg->type)
  {
  case ESPNOW_COMM:
    WIRELESS_TAG  = "ESPNOW_COMM";
    return WL_ERR_OK;
    break;
  
  default:
    WIRELESS_TAG  = "UNKNOWN_COMM";
    return WL_ERR_NO_COMM_TYPE;
    break;
  }
}