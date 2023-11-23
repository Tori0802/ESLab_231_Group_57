// Including libraries
#include <stdio.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include "driver/gpio.h"
#include <time.h>
#include <string.h>

/*
In lab 4, we assume a scenario where the ESP32 acts as a gateway in a smart home IoT system.
The gateway is designed to collect data and, based on that, control the status (on/off) of devices inside the home
   such as air conditioning, lights, garden water pump, TV, and so on.
The task input type will be randomly created and randomly write into the queue for reception_task with no specific period of time.
Note that we will not specify the value for simple implementation.
*/


// ---------------------------------------------------------------------------------------------
// DECLERATION
// ---------------------------------------------------------------------------------------------

// Define parameters
#define       MAX_LENGTH              30                      // Max length of message
#define       QUEUE_SIZE              10                      // Max length of queue
#define       TIME_OUT                100                     // Time out
#define       NUM_TASKS               4                       // Number of handle tasks

// Define structure data that stored in queue
typedef struct {
    int       data_ID;
    bool      data_value;
    char      data_message[MAX_LENGTH];
    int       data_reject;
} QueueData;

// Define structure that input data type
typedef struct Task {
    int       task_ID;
    char      task_name[MAX_LENGTH];
} Task;

// Predefine queue
QueueHandle_t Queue;

// Predefine tasks
Task          Control_Air_Cond         = {.task_name = "airc", .task_ID = 0};
Task          Control_Light            = {.task_name = "light", .task_ID = 1};
Task          Control_Pump             = {.task_name = "pump", .task_ID = 2};
Task          Control_TV               = {.task_name = "tv", .task_ID = 3};


// ---------------------------------------------------------------------------------------------
// METHODS
// ---------------------------------------------------------------------------------------------

// Create queue
void init_queue(void *pvParameter) {
    Queue = xQueueCreate(QUEUE_SIZE, sizeof(QueueData *));
    // Notify if value of queue is NULL
    if (Queue == NULL) {
    	printf("Failed to create queue, out of memory!\n");
    }
}

// Generate data
// (Create new structure data according to type_ID and value inputs)
QueueData *generate_data(int type_ID, int value) {
	// Create new structure data
    QueueData *new_data = malloc(sizeof(QueueData));
    // Assign init data state
    new_data->data_reject = 0;
    // Check input type_ID before assign values
    switch (type_ID) {
    case 0:     // Air-conditioner
        new_data->data_ID = type_ID;
        strcpy(new_data->data_message, "Control_Air_Cond");
        new_data->data_value = value;
        break;
    case 1:     // Light
        new_data->data_ID = type_ID;
        strcpy(new_data->data_message, "Control_Light");
        new_data->data_value = value;
        break;
    case 2:     // Pump
        new_data->data_ID = type_ID;
        strcpy(new_data->data_message, "Control_Pump");
        new_data->data_value = value;
        break;
    case 3:     // Television
        new_data->data_ID = type_ID;
        strcpy(new_data->data_message, "Control_TV");
        new_data->data_value = value;
        break;
    default:	// Assign NULL with other type_ID value
        new_data = NULL;
        break;
    }
    return new_data;
}

// Reception task
// (Receiving the task and put it in a queue until the queue is full
//  The reception period for each set of data is randomized to simulate real-world conditions)
void reception_task(void *pvParameter) {
	// Create and random variables
    time_t t;
    srand((unsigned)time(&t));                              // Random time

    for (;;) {
		int rand_job = (rand() % 4);                            // 4 jobs
		int rand_value = (rand() % 2);                            // 2 type value: On (1) and Off (0)
		int rand_delay = ((rand() % 5) + 1);                      // Random delay
    	// Notify if no queue is created
        while (Queue == NULL) {
            printf("No queue is created!\n");
        }
        // Create new structure data
        QueueData *data_temp = generate_data(rand_job, rand_value);
        // If new data is created completely
        if (data_temp != NULL) {
        	// Send data to the queue and notify if import has failed
            if (xQueueSend(Queue, (void *)&data_temp, TIME_OUT) == errQUEUE_FULL) {
                printf("Failed to import job: %d!\n", data_temp->data_ID);
            }
        }
        // Notify if new data isn't created completely
        else {
            printf("Failed to allocate new queue data!");
        }
        vTaskDelay(pdMS_TO_TICKS(100 * rand_delay));
    }
    vTaskDelete(NULL);
}

// Task handle
// From the queue, task_handle will take out the data and handle it according to its type.
// If the task cannot resolve with this task_handle type, it will increase state and wait for another handle task to resolve.
void task_handle(void *pvParameter) {
	for (;;) {
        // Check the queue and start to read from it to handle the task
        if (Queue != NULL)
        {
        	// Call pointers to task and queue data structures
            Task *task_temp = (Task *)pvParameter;
            QueueData *data_temp;
            // If can take out the element form the queue successfully
            if (xQueueReceive(Queue, &data_temp, (TickType_t)10) == pdPASS)
            {
            	// If data ID is same as task ID, notify and release memory
                if (data_temp->data_ID == task_temp->task_ID)
                {
                    printf("Resolving for task: %s with ID: %d and value: %d!\n", data_temp->data_message, data_temp->data_ID, data_temp->data_value);
                    free(data_temp);
                }
                // If data ID is not same as task ID
                else
                {
                    printf("%s cannot resolve for task %d!\n", data_temp->data_message, task_temp->task_ID);
                    // If passing NUM_TASKS time rejection mean that no task_handle can resolve this task,
                    // At that time notify and release memory
                    if (data_temp->data_reject < (NUM_TASKS - 1))
                    {
                        data_temp->data_reject++;
                        xQueueSendToFront(Queue, (void *)&data_temp, (TickType_t)10);
                    }
                    else
                    {
                        printf("No handler for this task: %s!\n", data_temp->data_message);
                        free(data_temp);
                    }
                }
            }
        }
        // Notify no queue is created
        else
        {
            printf("No queue is created!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

// Main
void app_main(void) {
    init_queue(NULL);
    xTaskCreate(&reception_task, "reception_task", 2048, NULL                      , 10, NULL);
    xTaskCreate(&task_handle   , "airc"          , 2048, (void *)&Control_Air_Cond , 10, NULL);
    xTaskCreate(&task_handle   , "light"         , 2048, (void *)&Control_Light    , 10, NULL);
    xTaskCreate(&task_handle   , "pump"          , 2048, (void *)&Control_Pump     , 10, NULL);
    xTaskCreate(&task_handle   , "tv"            , 2048, (void *)&Control_TV       , 10, NULL);
}