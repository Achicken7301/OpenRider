#include "main.h"

// Create a task with every 1s
void periodic_task(void *pvParameter)
{
  vTaskDelay(pdMS_TO_TICKS(5 * 1000));
  int count = 5;

  while (1)
  {
    if (count > 0)
    {
      set_brcst_fsm_state(MSH_BROADCAST_SEND);
      vTaskDelay(pdMS_TO_TICKS(5000));
      count--;
    }
  }
}

void app_main(void)
{
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  network_espnow_mesh_init();

  xTaskCreate(periodic_task, "periodic_task", 2048, NULL, 5, NULL);

  while (1)
  {
    MSH_Broadcast_FSM();
  }
}
