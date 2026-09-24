#include "Headerfile.h"

/**********************
 * TIMER DECLARATIONS
 **********************/
hw_timer_t *timer3 = nullptr;

volatile uint32_t g_timerTicks = 0;

/**********************
 * TIMER 3 ISR
 * Keep this minimal: only bump the tick counter. All real work (float math,
 * String access, flash-resident helpers) runs in TimerServiceTask instead, so
 * it cannot fault when the instruction cache is disabled during a flash write.
 **********************/
void IRAM_ATTR onTimer3()
{
    g_timerTicks++;
}

/**********************
 * TIMER SERVICE
 * Runs once per elapsed second, in task context (see TimerServiceTask).
 **********************/
void TimerFunction::service()
{
    TotalOperationTime.Seconds++;
    Timer.timeCalculate();

    Grease.TimerFun.Proximity(); // Update proximity sensor timing
    Grease.TimerFun.AppOnline(); // Update application online timing
    Grease.TimerFun.PauseTime(); // Update pause time timing
    Grease.TimerFun.TestRunTimeCalculation();
    LoRa.TimerFun.NexuxInOnline(); // Update Nexus online timing
    // Do not start the receive watchdog merely because LoRaWAN connected. The
    // dashboard publish itself occurs later, and the network needs more than a
    // few seconds to return its downlink ACK.
    if (LoraDetails.Connected && !LoraDetails.UserEnterOfflineMode)
    {
        LoraDetails._wait_after_connected++;
        LoraDetails.lora_connected_but_not_received++;
        if (LoraDetails._wait_after_connected > LORA_WAIT_AFTER_CONNECT)
            LoraDetails._wait_after_connected = LORA_WAIT_AFTER_CONNECT;
        if (LoraDetails.lora_connected_but_not_received > LORA_CONNECTED_BUT_NOT_RECEIVED)
            LoraDetails.lora_connected_but_not_received = LORA_CONNECTED_BUT_NOT_RECEIVED;
    }
}

/**********************
 * TIMER INITIALIZATION
 **********************/
void TimerFunction::begin()
{
    // Initialize timer3 for proximity and app online timing
    timer3 = timerBegin(2, 80, true);              // Use timer 2, prescaler 80, count up
    timerAttachInterrupt(timer3, &onTimer3, true); // Attach interrupt handler
    timerAlarmWrite(timer3, 1000000, true);        // Set alarm for 1 second
    timerAlarmEnable(timer3);                      // Enable the timer alarm
}

/**********************
 * TIME CALCULATION
 **********************/
void TimerFunction::timeCalculate()
{
    if (TotalOperationTime.Seconds == 60)
    {
        TotalOperationTime.Seconds = 0;
        TotalOperationTime.Minute++;
    }

    if (TotalOperationTime.Minute == 60)
    {
        TotalOperationTime.Minute = 0;
        TotalOperationTime.Hour++;
    }
}
