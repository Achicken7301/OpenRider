#ifndef CONFIG_H
#define CONFIG_H

// CHANGE IF YOU WANT TO CHANGE HOW NODE COMMUNICATE
#define USE_ESPNOW_COMM
// #define USE_NRF24_COMM
// #define USE_LORA_COMM


typedef enum
{
  ESPNOW_COMM,
  LORA,
  NRF24LO1,
  UNKNOWN_COMM,
} wireless_comm_type_t;

#endif // CONFIG_H
