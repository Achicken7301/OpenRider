#include "audio.h"

#define PAYLOAD_SIZE 256

static uint8_t payload[PAYLOAD_SIZE];

int new_audio_flag = 0;
static int counter_example = 0;

void audio_gen_example()
{
  for (int i = 0; i < PAYLOAD_SIZE; i++)
  {
    payload[i] = i;
  }
}

uint8_t *get_audio_example()
{
  audio_gen_example();
  return payload;
}