#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

extern hw_timer_t *timer1;
extern hw_timer_t *timer2;
extern hw_timer_t *timer3;

// NO IRAM_ATTR HERE
void onTimer1();
void onTimer2();
void onTimer3();

// Incremented once per second by the onTimer3 ISR (which does nothing else).
// TimerServiceTask consumes the difference and runs the real per-second work
// in task context, so it is safe even while the flash cache is disabled
// (EEPROM/OTA writes) and shared counters can be updated without racing.
extern volatile uint32_t g_timerTicks;

class TimerFunction
{
public:
    void begin();
    void timeCalculate();
    void service(); // Per-second work, called from TimerServiceTask (task context)
};

extern TimerFunction Timer;

#endif
