/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"

#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
#include "semphr.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
unsigned char systemState = 0;
TaskHandle_t short_handler = NULL;
TaskHandle_t long_handler = NULL;
SemaphoreHandle_t xMutex = NULL;
/* Task to be created. */

/**
	* this task controls led toggling time
  */
void long_task( void * pvParameters )
{
		unsigned int counter1,counter2;
		unsigned char ret;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 500;
	  // Initialise the xLastWakeTime variable with the current time.
	  xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
			
        /* Task code goes here. */
			//GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);
			counter1 = 0;
			if( xMutex != NULL )
			{
        /* See if we can obtain the semaphore. . */
        if( xSemaphoreTake( xMutex,portMAX_DELAY) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */

						while(counter1 < 10)
						{			
							ret = vSerialPutString("ahmed\n",6);
							if(ret == pdTRUE)
							{
								for(counter2 = 0;counter2 < 100000;counter2++)
								{
								}
								counter1++;
						  }
			      }
            /* We have finished accessing the shared resource.  Release the
            semaphore. */
            xSemaphoreGive( xMutex );
						//GPIO_write(PORT_0,PIN0,PIN_IS_LOW);
						// Wait for the next cycle.
						vTaskDelayUntil( &xLastWakeTime, xFrequency );
						//vTaskDelay(500);
        }
        else
        {
            /* We could not obtain the semaphore and can therefore not access
            the shared resource safely. */
        }
			}

    }
}

/**
  * takes button input and measures time of press
	*/
void short_task( void * pvParameters )
{
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
		unsigned int counter1 = 0;
		unsigned char ret = pdFALSE;
		// Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
      /* Task code goes here. */
			//GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);
			counter1 = 0;
			if(xMutex != NULL)
			{
				if(xSemaphoreTake(xMutex,portMAX_DELAY) == pdTRUE)
				{
					while(counter1 < 10)
					{
						ret = vSerialPutString("juarju\n",7);
						if(ret == pdTRUE)
						{
							counter1++;
						}
					}
					xSemaphoreGive(xMutex);
					//GPIO_write(PORT_0,PIN0,PIN_IS_LOW);
					vTaskDelayUntil( &xLastWakeTime, xFrequency );
					//vTaskDelay(100);
				}
			}
			else
			{
				
			}
    }
}

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

    /* Create Tasks here */
		/* Create the task, storing the handle. */
      xTaskCreate(
                      long_task,       /* Function that implements the task. */
                      "long Task",          /* Text name for the task. */
                      100,      /* Stack size in words, not bytes. */
                      ( void * ) 0,    /* Parameter passed into the task. */
                      1,/* Priority at which the task is created. */
                      &long_handler );      /* Used to pass out the created task's handle. */
		  xTaskCreate(
                      short_task,       /* Function that implements the task. */
                      "short task",          /* Text name for the task. */
                      100,      /* Stack size in words, not bytes. */
                      ( void * ) 0,    /* Parameter passed into the task. */
                      2,/* Priority at which the task is created. */
                      &short_handler );      /* Used to pass out the created task's handle. */								

   /* Create a xMutex type semaphore. */
   xMutex = xSemaphoreCreateMutex();

   if( xMutex != NULL )
   {
       /* The semaphore was created successfully and
       can be used. */
   }
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


