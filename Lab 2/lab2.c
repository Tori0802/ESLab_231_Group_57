// ------------------------------------------------------------------------------------------
// Include libraries
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


// ------------------------------------------------------------------------------------------
// Definition
   // Define GPIO
#define     BTN_NUM                         GPIO_NUM_34
   // Define ID students
#define     STUDENT_ID_NUM_1                2011610
#define     STUDENT_ID_NUM_2                2012286
   // Define mode button states
#define     NORMAL_STATE                    1
#define     PRESSED_STATE                   0
   // Define time for long press button events
#define     TIME_LONG_PRESS                 5000
#define     TIME_LONG_PRESS_NEXT_EVENT      2000


// ------------------------------------------------------------------------------------------
// Variables
   // Anti-interference processing variables for button presses
int         KeyReg0                         = NORMAL_STATE;
int         KeyReg1                         = NORMAL_STATE;
int         KeyReg2                         = NORMAL_STATE;
int         KeyReg3                         = NORMAL_STATE;
   // Flag check long press event
int         long_press_button_flag          = 0;
   // Tick timer counter for button long press events
int         TimeOutForKeyPress              = 0;


// ------------------------------------------------------------------------------------------
// Tasks implementation

// Task: Check button
void checkButton(void *pvParameter)
{
    for ( ; ; )
    {
    	// Anti-interference processing
        KeyReg0 = KeyReg1;
    	KeyReg1 = KeyReg2;
    	// Get GPIO button level
    	KeyReg2 = gpio_get_level(BTN_NUM);
		if ((KeyReg1 == KeyReg0) && (KeyReg1 == KeyReg2)){
			if (KeyReg3 != KeyReg2){
				KeyReg3 = KeyReg2;
				// If a button is detected and the state changes from 0 to 1, print "ESP32"
				if (KeyReg2 == PRESSED_STATE){
					printf("ESP32\n");
					// Set timer for long press events if flag is 0
					if (long_press_button_flag == 0) {
						// Long press detection event
						TimeOutForKeyPress = TIME_LONG_PRESS / portTICK_PERIOD_MS;
					}
					else {
						// Periodic printing events while still holding the button pressed
						TimeOutForKeyPress = TIME_LONG_PRESS_NEXT_EVENT / portTICK_PERIOD_MS;
					}
				}
			} else {
				// Handle timer when holding the button pressed
				TimeOutForKeyPress--;
				// When timer up, set long_press_button_flag to 1, and need to set KeyReg3 to NORMAL_STATE
				// (reason to prepare signal detection from 0 to 1 to trigger the print event)
				if (TimeOutForKeyPress == 0){
					KeyReg3 = NORMAL_STATE;
					long_press_button_flag = 1;
				}
			}
		}
		else {
			long_press_button_flag = 0;
		}
		// Delay task: 10ms
        vTaskDelay (10 / portTICK_PERIOD_MS) ;
    }
    vTaskDelete(NULL);
}

// Task: Print ID students
void printID(void *pvParameter)
{
    for ( ; ; )
    {
    	// Print ID students
        printf("Group 57: %d and %d\n", STUDENT_ID_NUM_1, STUDENT_ID_NUM_2);
        // Delay task: 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS) ;
    }
    vTaskDelete(NULL);
}


// ------------------------------------------------------------------------------------------
// Setup GPIO
void gpio_init()
{
	// Reset default configuration
	gpio_reset_pin(BTN_NUM);
	// Set direction
    gpio_set_direction(BTN_NUM, GPIO_MODE_INPUT);
    // Set pull mode
    gpio_set_pull_mode(BTN_NUM, GPIO_PULLUP_ONLY);
}


// ------------------------------------------------------------------------------------------
// Main
void app_main()
{
	gpio_init();
	// Create tasks
    xTaskCreate(&checkButton, "buttons", 8192, NULL, 1, NULL);
    xTaskCreate(&printID, "ID", 8192, NULL, 0, NULL);
}
