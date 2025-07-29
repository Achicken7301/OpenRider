#ifndef MAIN_H
#define MAIN_H


#include "driver/gptimer.h"

#include "intercom_fsm.h"
#include "scheduler.h"
// #include "wireless_comm.h"
// #include "espnow_driver.h"
#include "simple_log.h"
// #include "peer_manager.h"
// #include "config.h"

char *MAIN_TAG = "MAIN";

#ifdef USE_ESPNOW_COMM
#include "espnow_driver.h"

#elif USE_NRF24_COMM
#include "nrf24_driver.h"

#elif USE_LORA_COMM
#include "lora_driver.h"

#endif

void TIM_Init();

#endif // MAIN_H