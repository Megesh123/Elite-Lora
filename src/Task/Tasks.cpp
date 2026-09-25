/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note TASK MANAGEMENT IMPLEMENTATION
 */

/**********************
 * TASK MANAGEMENT IMPLEMENTATION
 * Handles FreeRTOS task creation and management for multi-threaded operation
 * Implements real-time system monitoring and control through dedicated tasks
 **********************/

#include "Headerfile.h" // Include main system header
#include "esp_task_wdt.h"

/**********************
 * CREATE TASKS
 * Initializes and creates FreeRTOS tasks for system operation
 **********************/
SemaphoreHandle_t SerialMutex = NULL;

// Task watchdog timeout. Every real-time task subscribes on entry and calls
// esp_task_wdt_reset() each loop; a task that hangs (stuck sensor loop, blocked
// I/O) is caught and the device resets instead of silently freezing.
#define TASK_WDT_TIMEOUT_S 10

// Cores
#define CORE_RT 0  // sensing / safety / control
#define CORE_APP 1 // UI, LoRa, OTA (shares with Arduino loopTask + WiFi/BT)

// Priorities (higher = more urgent). Keep <= configMAX_PRIORITIES-1 (24 on ESP32).
#define PRIO_SAFETY 4  // Volt, Current, Alert
#define PRIO_CONTROL 3 // GreaseOut, Supervisory
#define PRIO_UI 2      // LCD, Setting, LED
#define PRIO_NET 1     // LoraPublish, LoraReceive

void TaskFunction::create()
{
  SerialMutex = xSemaphoreCreateMutex();
  if (SerialMutex == NULL)
  {
    Serial.println("Failed to create Serial Mutex");
    return;
  }

  LoRaSerialMutex =
      xSemaphoreCreateMutex();

  if (LoRaSerialMutex == NULL)
  {
    Serial.println(
        "ERROR: LoRaSerialMutex creation failed");
  }
  // LoRaSerialMutex = xSemaphoreCreateMutex();
  // if (LoRaSerialMutex == NULL)
  // {
  //   Serial.println("Failed to create LoRaSerial Mutex");
  //   return;
  // }

  // (Re)configure the task watchdog. The Arduino core initialises it for the
  // loopTask already; this just widens the timeout for our slower RT tasks.
  esp_task_wdt_init(TASK_WDT_TIMEOUT_S, true); // true = panic + reset on timeout

  //                         fn              name           stack  arg  prio          handle core
  xTaskCreatePinnedToCore(VoltTask, "Volt", 3072, NULL, PRIO_SAFETY, NULL, CORE_RT);
  xTaskCreatePinnedToCore(CurrentTask, "Current", 3072, NULL, PRIO_SAFETY, NULL, CORE_RT);
  xTaskCreatePinnedToCore(AlertTask, "Alert", 4096, NULL, PRIO_SAFETY, NULL, CORE_RT);
  xTaskCreatePinnedToCore(GreaseOutTask, "GreaseOut", 4096, NULL, PRIO_CONTROL, NULL, CORE_RT);
  xTaskCreatePinnedToCore(SupervisoryTask, "Supervisory", 4096, NULL, PRIO_CONTROL, NULL, CORE_RT);
  xTaskCreatePinnedToCore(TimerServiceTask, "TimerSvc", 4096, NULL, PRIO_CONTROL, NULL, CORE_RT);

  xTaskCreatePinnedToCore(LCDDisplayTask, "LCDDisplay", 6144, NULL, PRIO_UI, NULL, CORE_APP);
  xTaskCreatePinnedToCore(SettingTask, "Setting", 6144, NULL, PRIO_UI, NULL, CORE_APP);
  xTaskCreatePinnedToCore(LedTask, "Led", 2048, NULL, PRIO_UI, NULL, CORE_APP);
  // Task 8 sends LoRa data. It may temporarily own LoRaSerial while waiting
  // for OK/TX_DONE; sendString() also dispatches +EVT:RX_ downlinks.
  xTaskCreatePinnedToCore(LoraPublishTask, "LoraPublish", 12288, NULL, PRIO_NET, NULL, CORE_APP);

  // Task 10 receives unsolicited LoRa events when the UART is available.
  xTaskCreatePinnedToCore(LoraReceiveTask, "LoraReceive", 4096, NULL, PRIO_NET, NULL, CORE_APP);
}

/**********************
 * TIMER SERVICE TASK
 * Consumes the 1 Hz tick counter set by the onTimer3 ISR and runs the real
 * per-second work in task context. Catches up if several ticks were missed.
 **********************/
void TaskFunction::TimerServiceTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task - Timer Service Task Started");
  xSemaphoreGive(SerialMutex);

  esp_task_wdt_add(NULL);
  uint32_t handled = g_timerTicks;

  for (;;)
  {
    uint32_t now = g_timerTicks; // volatile snapshot
    while ((uint32_t)(now - handled) != 0)
    {
      handled++;
      Timer.service();
    }
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

/**********************
 * LCD DISPLAY TASK
 * Manages LCD display updates and interface rendering
 **********************/
void TaskFunction::LCDDisplayTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 1 - LCD Display Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    Lcd.DisplayFunction(); // Update LCD display
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * SETTING TASK
 * Manages settings interface and user configuration
 **********************/
void TaskFunction::SettingTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 2 - Setting Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    MenuSetting.SettingFunction(); // Update settings interface
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * Voltage TASK
 * Monitors system voltage with low voltage protection
 **********************/
void TaskFunction::VoltTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 3 - Voltage Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    Voltage.read(); // Read voltage value
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * Current TASK
 * Monitors electrical current with overload protection
 **********************/
void TaskFunction::CurrentTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 4 - Current Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    Current.read(); // Read current value
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

void TaskFunction::GreaseOutTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 5 - GreaseOut Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    if (!bluetooth.ModeCheck && !TestingModeEnable && !Alert.Enable && start_init == 1) // If operational and no alerts
    {
      Grease.lubrication(); // Run lubrication process
      Grease.pauseTime();   // Run pause time process
      Grease.testRun();     // Run test run process
    }
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * SUPERVISORY TASK
 * Monitors supervisory mode time tracking and alerts
 **********************/
void TaskFunction::SupervisoryTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 6 - Supervisory Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    if (!VoltageMonitor.PowerDownDetected)
    {
      if (!bluetooth.ModeCheck && !TestingModeEnable)
      {
        Supervisory.Supervisory1();            // Monitor supervisory function for outlet 1
        Supervisory.Supervisory2();            // Monitor supervisory function for outlet 2
        Supervisory.Supervisory3();            // Monitor supervisory function for outlet 3
        Supervisory.Check_Supervisory_Alert(); // Check and trigger supervisory alerts
        Supervisory.Supervisory();             //  Main supervisory monitoring function
      }
    }
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * ALERT TASK
 * Monitors and processes system alerts and notifications
 **********************/
void TaskFunction::AlertTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 7 - Alert Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {
    if (!VoltageMonitor.PowerDownDetected)
    {
      if (!bluetooth.ModeCheck && !TestingModeEnable && start_init == 1) // If not in Bluetooth mode
      {
        AlertFun.occured(); // Process any active alerts
        AlertFun.Buzzer();
      }
    }
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * LoRa PUBLISH TASK
 * Handles LoRa message publishing to cloudN
 **********************/
void TaskFunction::LoraPublishTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 8 - LoRa Publish Task Started");
  xSemaphoreGive(SerialMutex);

  if (start_init == 1 && !bluetooth.ModeCheck)
  {
    if (!loraBegin())
    {
      Serial.println();
      Serial.println("LoRa initialization FAILED!");
    }
    else
    {
      Serial.println();
      Serial.println("LoRa initialization SUCCESS!");
    }
    if (!loraJoin())
    {
      Serial.println("LoRaWAN JOIN FAILED!");
    }
  }
  else
  {
    if (start_init != 1)
    {
      Serial.println("Basic Config not completed. Skipping LoRa initialization.");
    }
    else if (bluetooth.ModeCheck)
    {
      Serial.println("Bluetooth mode is active. Skipping LoRa initialization.");
    }
  }
  long previousMillis = 0;
  // Deliberately NOT subscribed to the task watchdog: loraJoin() blocks for up
  // to its own 20s timeout while the LoRaWAN network processes the join. It
  // carries no safety-critical work.
  for (;;) // Infinite task loop
  {

    if (!bluetooth.ModeCheck && !TestingModeEnable && start_init == 1) // If not in Bluetooth mode and LoRa is connected
    {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 5000) // Check if 5 seconds have passed
      {
        previousMillis = currentMillis;
        if (!LoraInitialized)
        {
          if (!loraBegin())
          {
            Serial.println();
            Serial.println("LoRa initialization FAILED!");
          }
          else
          {
            Serial.println();
            Serial.println("LoRa initialization SUCCESS!");
          }
        }
        if (!LoraJoined)
        {
          if (!loraJoin())
          {
            Serial.println("LoRaWAN JOIN FAILED!");
          }
        }
      }

      if (!VoltageMonitor.PowerDownDetected && LoraJoined)
      {
        AlertFun.LoraPublishAlert(); // Publish alert data via LoRa
        Grease.LoraLiveProcess();    // Publish live process data via LoRa
                                     // LoRa.NEXUS_ONLINE_UPDATE();  // Update Nexus online status

        LoRa.OfflineSettingPageData();
        // LoRa.offlineDataStart(); // Handle offline data transmission

        Twoway.Process();
        LoRa.controllerOnline();     // Send controller online status via LoRa
        AlertFun.LoraPublishAlert(); // Publish alert data via LoRa

        Receive(); // Process received messages - Part 1
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

/**********************
 * LoRa RECEIVE TASK
 * Continuously drains LoRaSerial and dispatches downlink lines so that
 * unsolicited events (Class C downlinks) are never dropped just because
 * LoraPublishTask isn't inside a blocking AT/join/send call at the time.
 **********************/
void TaskFunction::LoraReceiveTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 10 - LoRa Receive Task Started");
  xSemaphoreGive(SerialMutex);

  esp_task_wdt_add(NULL);

  for (;;)
  {
    // Do not read the UART before loraJoin() has completed.
    // This prevents Task 10 from consuming +EVT:JOINED.
    if (LoraInitialized && LoraJoined && !bluetooth.ModeCheck && !TestingModeEnable && start_init == 1)
    {
      loraReceiveProcess();
    }

    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void TaskFunction::LedTask(void *pvParameters)
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("Task 9 - LED Task Started");
  xSemaphoreGive(SerialMutex);
  esp_task_wdt_add(NULL);
  for (;;) // Infinite task loop
  {

    switch (LedFun.state)
    {
    case _on:
      Led._on();
      break;

    case _off:
      Led._off();
      break;

    case _overload:
      Led._overload();
      break;

    case _supervisory:
      Led._supervisory();
      break;

    case _reservoir:
      Led._reservoir();
      break;

    case _proximity:
      Led._proximity();
      break;

    case _testrun:
      Led._testrun();
      break;

    default:
      Led._off();
      break;
    }

    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
