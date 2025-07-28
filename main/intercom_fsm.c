#include "intercom_fsm.h"

intercom_fsm_t intercom_state = I_INIT;

void IntercomFSM()
{
  switch (intercom_state)
  {
  case I_INIT:
    // SCH_Add(PM_update_peer_list, 1000, 1000);
    intercom_set_fsm(I_IDLE);
    break;

  case I_IDLE:

    break;

  case I_SENDER:
    break;

  default:
    break;
  }
}

void intercom_set_fsm(intercom_fsm_t next_state)
{
  intercom_state = next_state;
}
