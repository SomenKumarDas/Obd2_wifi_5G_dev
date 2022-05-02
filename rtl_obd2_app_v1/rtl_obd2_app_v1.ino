#include "app_soc.hpp"

void setup()
{
    dev_init();

    if(xTaskCreate(app_srv_task, "app_srv_task", 1024*10, NULL, OS_PRIORITY_REALTIME, &AppSoc_TH) != pdTRUE)
    {
        UART_LOG.println("[ERROR] [OS TASK CREATE FAILED]");
    }
}

void loop()
{
    vTaskDelete(NULL);
}
