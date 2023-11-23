#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

//Preemptive without time-slicing and configIDLE_SHOULD_YIELD is set to 0
//#define configUSE_PREEMPTION                            1
//#define configUSE_TIME_SLICING                          0
//#define configIDLE_SHOULD_YIELD                         0

const char *LOG_TAG_MAIN = "MAIN";

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

uint32_t start_point;

void vTask1(void *pvParameters) {
	vTaskDelay(5);

    printf("(%lu) TASK1 (high, event) start\n", xTaskGetTickCount());

	printf("(%lu) TASK1: running\n", xTaskGetTickCount());
	printf("(%lu) TASK1 end\n", xTaskGetTickCount());

	vTaskDelay(2);

    printf("(%lu) TASK1: running again\n", xTaskGetTickCount());
    printf("(%lu) TASK1 end\n", xTaskGetTickCount());
    vTaskDelete(TaskHandle_1);
}

void vTask2(void *pvParameters) {
    uint32_t prev_tick1 = xTaskGetTickCount();
    uint32_t prev_tick2 = xTaskGetTickCount();
    uint32_t prev_tick3 = xTaskGetTickCount();
    uint32_t curr_tick = xTaskGetTickCount();

    printf("(%lu) TASK2 (idle priority, continuous) start\n", xTaskGetTickCount());

    while (xTaskGetTickCount() - curr_tick < 5) {
    	if (prev_tick3 != xTaskGetTickCount() || prev_tick2 != xTaskGetTickCount() || prev_tick1 != xTaskGetTickCount()) {
    		printf("(%lu) TASK2: running\n", xTaskGetTickCount());
    	    prev_tick1 = prev_tick2;
    	    prev_tick2 = prev_tick3;
    	    prev_tick3 = xTaskGetTickCount();
    	}
    }

    printf("(%lu) TASK2 end\n", xTaskGetTickCount());
    vTaskDelete(TaskHandle_2);
}

void vTask3(void *pvParameters) {
    uint32_t prev_tick1 = xTaskGetTickCount();
    uint32_t prev_tick2 = xTaskGetTickCount();
    uint32_t prev_tick3 = xTaskGetTickCount();
    uint32_t curr_tick = xTaskGetTickCount();
    start_point = xTaskGetTickCount();

    printf("(%lu): TASK3 (continuous) start\n", xTaskGetTickCount());

    while (xTaskGetTickCount() - curr_tick < 10) {
    	if (prev_tick3 != xTaskGetTickCount() || prev_tick2 != xTaskGetTickCount() || prev_tick1 != xTaskGetTickCount()) {
    		printf("(%lu) TASK3: running\n", xTaskGetTickCount());
    	    prev_tick1 = prev_tick2;
    	    prev_tick2 = prev_tick3;
    	    prev_tick3 = xTaskGetTickCount();
    	}
    }

    printf("(%lu) TASK3: finish!\n", xTaskGetTickCount());
    vTaskDelete(TaskHandle_3);
}

void app_main(void)
{
	//set priority for app_main to be highest among other tasks to avoid others task block app_main after initializaion
    vTaskPrioritySet(NULL, 15);
    if (xTaskCreatePinnedToCore(&vTask3, "vTask3", 2048, NULL, 2, &TaskHandle_3,0) == pdPASS) ESP_LOGI(LOG_TAG_MAIN, "TASK3 created successfully");
    if (xTaskCreatePinnedToCore(&vTask2, "vTask2", 2048, NULL, 2, &TaskHandle_2,0) == pdPASS) ESP_LOGI(LOG_TAG_MAIN, "TASK2 created successfully");
    if (xTaskCreatePinnedToCore(&vTask1, "vTask1", 2048, NULL, 5, &TaskHandle_1,0) == pdPASS) ESP_LOGI(LOG_TAG_MAIN, "TASK1 created successfully");

    //set back to original priority for app_main task for not blocking other running tasks
    printf("FINISH APP_MAIN\n");
    vTaskPrioritySet(NULL, 1);
}