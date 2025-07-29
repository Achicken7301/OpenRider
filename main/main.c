#include "main.h"


void app_main(void)
{
  TIM_Init();

  while (1)
  {
    SCH_Dispatch();
    IntercomFSM();
  }
}

bool timer_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
  SCH_Update();
  return true;
}

void TIM_Init()
{
  gptimer_handle_t timer;
  gptimer_config_t cfg = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000 * 1000, // 1MHz → 1 us per tick
  };
  ESP_ERROR_CHECK(gptimer_new_timer(&cfg, &timer));

  gptimer_alarm_config_t alarm = {
      .alarm_count = 1000, // every 1000 ticks (1ms) call function_cb
      .reload_count = 0,
      .flags.auto_reload_on_alarm = true,
  };
  ESP_ERROR_CHECK(gptimer_set_alarm_action(timer, &alarm));

  gptimer_event_callbacks_t cbs = {
      .on_alarm = timer_cb,
  };
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer, &cbs, NULL));
  ESP_ERROR_CHECK(gptimer_enable(timer));
  ESP_ERROR_CHECK(gptimer_start(timer));
}