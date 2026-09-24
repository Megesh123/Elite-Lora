/**********************
 * GREASE PROCESSING HEADER
 * Defines interface for grease dispensing control and system modes
 * Handles lubrication, pause timing, test runs, and sleep modes
 **********************/

#ifndef __GREASE_OUT_FUNCTION_H__
#define __GREASE_OUT_FUNCTION_H__

class GreaseTimerFunction
{
public:
    void Proximity();
    void AppOnline();
    void PauseTime();
    void TestRunTimeCalculation();
};

/**********************
 * GREASE_PROCESS_FUNCTION CLASS
 * Manages grease dispensing operations and system modes
 * Handles lubrication cycles, pause timing, test runs, and sleep states
 **********************/
class GREASE_PROCESS_FUNCTION
{
public:
    void lubrication(); // Main lubrication process control with proximity sensing
    void pauseTime();   // Pause time management between lubrication cycles
    void testRun();     // Test run operation mode for system verification

    void testRunStart();     // Start test run mode for system testing
    void sleepStart();       // Enter low-power sleep mode
    void lubricationStart(); // Start lubrication process with motor activation

    void stop();  // Stop all operations and deactivate motor
    void begin(); // Initialize grease processing system

    void TestRunPageEnableDisable(); // Enable or disable test run page display
    void TestRunOn();                // Activate test run mode
    void TestRunOff();               // Deactivate test run mode
    void ResetProcess();             // Reset grease processing state and variables

    void LoraLiveProcess();       // Send live process updates to LoRa for monitoring
    GreaseTimerFunction TimerFun; // Instance of timer functions for proximity and app online timing
    int TotalProxityCount = 0;

private:
    void _lubricationStop();      // Stop lubrication process and initiate pause
    void _lubricationStopCheck(); // Check if lubrication should stop based on grease quantity
    void _lubricationProcess();
    void _lubricationProcessCalculation(); // Round values to one decimal place for accuracy
    void _pauseTimeStart();                // Start pause time period between cycles
    void _pauseTimeStop();                 // Stop pause time and restart lubrication
    void _pauseTimeHourCaluculate();       // Calculate pause time hours decrement
    void _pauseTimeMinCaluculate();        // Calculate pause time minutes and seconds

    void _testRun();   // Perform test run operations after activation
    void _appOnline(); // Check application online status for LoRa

    bool _proximity_pressed = false; // Internal flag for proximity sensor state
    int LubricationReadCount = 0;
    uint8_t LubricationCycle = 0;
    const uint8_t Pattern[4] = {5, 5, 5, 6};
};

extern GREASE_PROCESS_FUNCTION Grease; // Global instance of GREASE_PROCESS_FUNCTION

#endif /* __GREASE_OUT_FUNCTION_H__ */