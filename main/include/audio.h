#ifndef AUDIO_H
#define AUDIO_H

#include "stdint.h"
#include "simple_log.h"
#include "config.h"
#define AUDIO_PACKET_DEFAULT \
  {                          \
  }

extern int new_audio_flag;
uint8_t *get_audio_example();

#endif // AUDIO_H