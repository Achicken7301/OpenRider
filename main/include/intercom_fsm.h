#ifndef _INTERCOM_FSM_H_
#define _INTERCOM_FSM_H_

#include "stdint.h"
#include "scheduler.h"
#include "wireless_comm.h"
#include "espnow_driver.h"
#include "peer_manager.h"


#define INTERCOM_FSM_ENUM 10
#define MAX_RECEIVE_CMD_TIME 30

// char * INTERCOM_TAG ="INTERCOM";

typedef enum
{
  I_INIT = INTERCOM_FSM_ENUM,
  I_DISCOVER,
  I_IDLE,
  I_SEND,
  I_RECEIVE,
  I_FORWARDER,
  I_AUDIO_PLAY
} intercom_fsm_t;


intercom_fsm_t get_intercom_fsm_state();
void intercom_set_fsm(intercom_fsm_t);
void intercom_set_comm(wireless_comm_type_t);


void IntercomFSM();

#endif