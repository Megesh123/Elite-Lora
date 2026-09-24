/**********************
 * TASK MANAGEMENT HEADER
 * Defines interface for FreeRTOS task creation and management
 * Handles multi-threading for system components and real-time operations
 **********************/

#ifndef __TASKS_H__
#define __TASKS_H__

/**********************
 * TASKFUNCTION CLASS
 * Manages FreeRTOS task creation and system component parallel execution
 **********************/
class TaskFunction
{
public:
    void create(); // Create and initialize all FreeRTOS tasks

private:
    static void VoltTask(void *pvParameters);        // System voltage monitoring and low voltage protection
    static void CurrentTask(void *pvParameters);     // System current monitoring and overload protection
    static void AlertTask(void *pvParameters);       // System alert processing and notification handling
    static void GreaseOutTask(void *pvParameters);   // Main lubrication process control
    static void SettingTask(void *pvParameters);     // LED status indicator control and patterns
    static void LCDDisplayTask(void *pvParameters);  // Dashboard interface and status updates
    static void SupervisoryTask(void *pvParameters); // Supervisory mode time tracking and alerts

    static void LedTask(void *pvParameters);          // LED status indicator task
    static void LoraPublishTask(void *pvParameters);  // LoRa communication and cloud connectivity
    static void LoraReceiveTask(void *pvParameters);  // Continuous LoRa downlink listener
    static void TimerServiceTask(void *pvParameters); // Per-second work moved out of the onTimer3 ISR
};

extern TaskFunction Task; // Global instance of TaskFunction

#endif /* __TASKS_H__ */