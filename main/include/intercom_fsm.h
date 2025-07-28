#ifndef _INTERCOM_FSM_H_
#define _INTERCOM_FSM_H_

#include "stdint.h"
#include "scheduler.h"
#include "wireless_comm.h"


#define INTERCOM_FSM_ENUM 10
typedef enum
{
  I_INIT = INTERCOM_FSM_ENUM,
  I_DISCOVER,
  I_IDLE,
  I_SENDER,
  I_RECEIVER,
  I_FORWARDER,
  I_AUDIO_PLAY
} intercom_fsm_t;


intercom_fsm_t get_intercom_fsm_state();
void intercom_set_fsm(intercom_fsm_t);
void intercom_set_comm(wireless_comm_type_t);


void IntercomFSM();

#endif