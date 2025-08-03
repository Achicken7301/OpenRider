#ifndef CONFIG_H
#define CONFIG_H

// CHANGE IF YOU WANT TO CHANGE HOW NODE COMMUNICATE
#define USE_ESPNOW_COMM
// #define USE_NRF24_COMM
// #define USE_LORA_COMM

#define PACKET_STR "\t%02x %02x %02x %02x %02x %02x %02x %02x"
#define PACKET_SRC(pkg) (pkg).payload[i], (pkg).payload[i + 1],                           \
                        (pkg).payload[i + 2], (pkg).payload[i + 3], (pkg).payload[i + 4], \
                        (pkg).payload[i + 5], (pkg).payload[i + 6], (pkg).payload[i + 7]

typedef enum
{
  ESPNOW_COMM,
  LORA,
  NRF24LO1,
  UNKNOWN_COMM,
} wireless_comm_type_t;

#endif // CONFIG_H
