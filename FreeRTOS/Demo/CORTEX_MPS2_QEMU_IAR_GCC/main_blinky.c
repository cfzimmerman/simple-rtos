/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include <limits.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "timers.h"

#define mainRECV_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainSEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define mainTASK_SEND_FREQUENCY_MS pdMS_TO_TICKS(500UL)
#define mainTIMER_SEND_FREQUENCY_MS pdMS_TO_TICKS(2000UL)

#define mainVALUE_SENT_FROM_TASK (100UL)
#define mainVALUE_SENT_FROM_TIMER (200UL)

static void prvRecvNotifTask(void *pvParameters);
static void prvSendNotifTask(void *pvParameters);
static void prvSendNotifTimer(TimerHandle_t xTimerHandle);

static TimerHandle_t xTimer = NULL;
static TaskHandle_t xReceiverTask = NULL;

/// Starts a receiver task, a sender task, and a timer task. The
/// sender and timer pass notifications to the receiver, which prints
/// different colors based on who sent the message.
void main_blinky(void) {
  const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

  BaseType_t res = xTaskCreate(prvRecvNotifTask, "Rx", configMINIMAL_STACK_SIZE,
                               NULL, mainRECV_TASK_PRIORITY, &xReceiverTask);
  configASSERT(res == pdPASS);

  res = xTaskCreate(prvSendNotifTask, "Tx", configMINIMAL_STACK_SIZE, NULL,
                    mainSEND_TASK_PRIORITY, NULL);
  configASSERT(res == pdPASS);

  xTimer = xTimerCreate("Timer", xTimerPeriod, pdTRUE, NULL, prvSendNotifTimer);
  xTimerStart(xTimer, 0);

  vTaskStartScheduler();

  // If this point is reached, assume there was insufficient FreeRTOS
  // heap memory available.
  for (;;)
    ;
}

/// Performs a similar blinky function as the queue handler but using
/// notifications instead
static void prvRecvNotifTask(void *pvParameters) {
  (void)pvParameters;

  uint32_t ulNotifValue;
  for (;;) {
    xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifValue, portMAX_DELAY);
    if (ulNotifValue == mainVALUE_SENT_FROM_TASK) {
      printf("\033[33mNotification from task\033[0m\r\n");
    } else if (ulNotifValue == mainVALUE_SENT_FROM_TIMER) {
      printf("\033[35mNotification from timer\033[0m\r\n");
    } else {
      printf("Unexpected message\r\n");
    }
  }
}

/// Sends a timer notification to the receiver task
static void prvSendNotifTimer(TimerHandle_t xTimerHandle) {
  (void)xTimerHandle;
  configASSERT(xReceiverTask != NULL);
  xTaskNotify(xReceiverTask, mainVALUE_SENT_FROM_TIMER, eSetValueWithOverwrite);
}

/// Sends a normal task-to-task notification to the receiver on a fixed interval
static void prvSendNotifTask(void *pvParameters) {
  (void)pvParameters;

  TickType_t xNextWakeTime = xTaskGetTickCount();
  const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;

  for (;;) {
    vTaskDelayUntil(&xNextWakeTime, xBlockTime);
    configASSERT(xReceiverTask != NULL);
    xTaskNotify(xReceiverTask, mainVALUE_SENT_FROM_TASK,
                eSetValueWithOverwrite);
  }
}
