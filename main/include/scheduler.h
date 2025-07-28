#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <stdio.h>

/**
 * @brief This Scheduler with O(n) complexity
 * @Todo: Make this to O(1)
 *
 */
#define CLOCK 1 //1us per tick
#define SCH_MAX_TASK 10

typedef struct
{
  void *pFn;
  uint16_t delay;
  uint16_t period;
  uint8_t flag;
} vTask;

typedef enum
{
  SCH_ERR_OK,
  SCH_ERR_NOT_OK,
  SCH_ERR_TASK_NOT_FOUND,
} SchedulerError_t;


void SCH_Update();
void SCH_Dispatch();
void SCH_Add(void *task, uint16_t delay, uint16_t period);
void SCH_Delete(int);
int SCH_Get(void *);

#endif