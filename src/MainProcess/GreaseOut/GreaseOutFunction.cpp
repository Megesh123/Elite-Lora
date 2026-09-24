/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Grease processing implementation for Lubrication Control System
 */

/**********************
 * GREASE PROCESSING IMPLEMENTATION
 * Handles grease dispensing control, timing, and system mode management
 * Implements lubrication cycles, pause periods, and test operations
 **********************/

#include "HeaderFile.h" // Include main system header

/**********************
 * LUBRICATION PROCESS FUNCTION
 * Manages the main lubrication dispensing operation
 **********************/
void GREASE_PROCESS_FUNCTION::lubrication()
{
  if (GreaseOut.Lubrication.Enable) // While lubrication mode is active
  {
    Motor.on();                                                          // Turn on motor for grease dispensing
    if (digitalRead(PIN_PROXIMITY_SENSOR) == LOW && !_proximity_pressed) // Check if proximity sensor is triggered
    {
      _lubricationProcess();
    }
    if (digitalRead(PIN_PROXIMITY_SENSOR) == HIGH)
    {
      _proximity_pressed = false; // Reset proximity pressed flag
    }
    AlertFun.proximityNotSense(); // Check for proximity sensor issues
  }
}

/**********************
 * PAUSE TIME PROCESS FUNCTION
 * Manages the pause time period between lubrication cycles
 **********************/
void GREASE_PROCESS_FUNCTION::pauseTime()
{
  if (GreaseOut.PauseTime.Enable) // While pause time mode is active
  {
    Motor.off();               // Ensure motor is off during pause time
    _pauseTimeMinCaluculate(); // Calculate pause minutes
    _pauseTimeStop();          // Check if pause time should end
  }
}

/**********************
 * TEST RUN PROCESS FUNCTION
 * Manages the test run operation mode for system verification
 **********************/
void GREASE_PROCESS_FUNCTION::testRun()
{
  if (GreaseOut.TestRun.Enable) // While test run mode is active
  {
    Motor.on();                                                          // Turn on motor for grease dispensing
    if (digitalRead(PIN_PROXIMITY_SENSOR) == LOW && !_proximity_pressed) // Check if proximity sensor is triggered
    {
      _testRun();
    }
    if (digitalRead(PIN_PROXIMITY_SENSOR) == HIGH)
    {
      _proximity_pressed = false; // Reset proximity pressed flag
    }

    AlertFun.proximityNotSense(); // Check for proximity sensor issues

    Led.Testrun_on();

    if (GreaseOut.TestRun.min < 0)
    {
      GreaseOut.TestRun.sec = TEST_RUN_SEC;
      GreaseOut.TestRun.min = TEST_RUN_MIN;

      STATE_LOCK();
      GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
      GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
      GreaseOut.TestRun.Enable = false;     // Disable test run mode
      GreaseOut.Sleep.Enable = false;       // Disable sleep mode
      STATE_UNLOCK();

      GreaseOut.Proximity.Seconds = 0; // Reset proximity sensor timing counter

      Motor.off();       // Turn off motor
      Led.Testrun_off(); // Deactivate test run LED indication
    }

    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

void GreaseTimerFunction::TestRunTimeCalculation()
{
  if (GreaseOut.TestRun.Enable)
  {
    GreaseOut.TestRun.sec--;
    if (GreaseOut.TestRun.sec <= 0)
    {
      GreaseOut.TestRun.sec = 60;
      GreaseOut.TestRun.min--;
    }
  }
}
/**********************
 * LUBRICATION START FUNCTION
 * Initiates the lubrication process and activates motor
 **********************/
void GREASE_PROCESS_FUNCTION::lubricationStart()
{
  STATE_LOCK();
  GreaseOut.Lubrication.Enable = true; // Enable lubrication mode
  GreaseOut.PauseTime.Enable = false;  // Disable pause time mode
  GreaseOut.TestRun.Enable = false;    // Disable test run mode
  GreaseOut.Sleep.Enable = false;      // Disable sleep mode
  STATE_UNLOCK();
  Motor.on(); // Turn on motor
}

/**********************
 * LUBRICATION STOP CHECK FUNCTION
 * Checks if lubrication process should stop based on grease quantity
 **********************/
void GREASE_PROCESS_FUNCTION::_lubricationStop()
{
  _pauseTimeStart();                // Start pause time period
  GreaseOut.Lora_PauseStart = true; // Set LoRa pause start flag
  Motor.off();                      // Turn off motor
}

void GREASE_PROCESS_FUNCTION::_lubricationProcess()
{
  if (!_proximity_pressed)
  {
    _proximity_pressed = true;
    GreaseOut.Proximity.Seconds = 0;
    LubricationReadCount++;
    Grease.TotalProxityCount++;

    if (LubricationReadCount >= Pattern[LubricationCycle])
    {

      _lubricationProcessCalculation();
      _lubricationStopCheck();

      GreaseOut.Lubrication.LoraDataSend = true;

      GreaseOut.Proximity.Seconds = 0;
      LubricationReadCount = 0;

      Reservoir.read();

      LubricationReadCount = 0;
      LubricationCycle = (LubricationCycle + 1) % 4;
    }
  }
}

/**********************
 * LUBRICATION STOP CHECK FUNCTION
 * Checks if lubrication process should stop based on grease quantity
 **********************/
void GREASE_PROCESS_FUNCTION::_lubricationStopCheck()
{
  GreaseOut.Lubrication.Live.Grams -= Config.NoOfOutlet; // Add grease quantity for current cycle
  if (GreaseOut.Lubrication.Live.Grams < Config.NoOfOutlet)
  {
    GreaseOut.Lubrication.Live.Grams += GreaseOut.Lubrication.Temp.Grams; // Add grease quantity for next cycle
    _lubricationStop();                                                   // Stop lubrication process
  }
}

/**********************
 * PAUSE TIME START FUNCTION
 * Initiates the pause time period between lubrication cycles
 **********************/
void GREASE_PROCESS_FUNCTION::_pauseTimeStart()
{
  STATE_LOCK();
  GreaseOut.Lubrication.Enable = false;                         // Disable lubrication mode
  GreaseOut.PauseTime.Enable = true;                            // Enable pause time mode
  GreaseOut.TestRun.Enable = false;                             // Disable test run mode
  GreaseOut.Sleep.Enable = false;                               // Disable sleep mode
  GreaseOut.PauseTime.Seconds = 60;                             // Set pause seconds counter
  GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute; // Set temporary pause minutes
  GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour;     // Set temporary pause hours
  STATE_UNLOCK();
  Motor.off(); // Turn off motor
}

/**********************
 * PAUSE TIME STOP FUNCTION
 * Ends the pause time period and restarts lubrication cycles
 **********************/
void GREASE_PROCESS_FUNCTION::_pauseTimeStop()
{
  if (GreaseOut.PauseTime.Live.Hour == 0 && GreaseOut.PauseTime.Live.Minute == 0) // Check if pause time completed
  {
    lubricationStart();                                           // Restart lubrication process
    GreaseOut.Lora_LubricationStart = true;                       // Set LoRa lubrication start flag
    GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute; // Set temporary pause minutes
    GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour;     // Set temporary pause hours
    Motor.on();                                                   // Turn on motor
  }
}

/**********************
 * PAUSE TIME HOUR CALCULATION FUNCTION
 * Calculates the decrement of pause time hours
 **********************/
void GREASE_PROCESS_FUNCTION::_pauseTimeHourCaluculate()
{
  if (GreaseOut.PauseTime.Live.Hour > 0 && GreaseOut.PauseTime.Live.Minute < 0) // Check if minutes underflowed
  {
    GreaseOut.PauseTime.Live.Minute = 59; // Reset minutes to 59
    GreaseOut.PauseTime.Live.Hour--;      // Decrement hours
  }
}

/**********************
 * PAUSE TIME MINUTE CALCULATION FUNCTION
 * Calculates the decrement of pause time minutes and seconds
 **********************/
void GREASE_PROCESS_FUNCTION::_pauseTimeMinCaluculate()
{
  if (GreaseOut.PauseTime.Seconds <= 0) // Check if seconds completed
  {
    GreaseOut.PauseTime.Live.Minute--; // Decrement minutes
    GreaseOut.PauseTime.Seconds = 60;  // Reset seconds counter
    _pauseTimeHourCaluculate();

    AboutDevice.total.pauseMinute();
    AboutDevice.total.pauseHour();

    // Log.info("Pause Time - Hour: %d, Minute: %d", GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute);
    GreaseOut.PauseTime.LoraDataSend = true; // Enable LoRa data send for pause time updates
    // Log.info("Pause Time - Hour: %d, Minute: %d", GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute);
  }
}

/**********************
 * TEST RUN START FUNCTION
 * Initiates the test run operation mode for system verification
 **********************/
void GREASE_PROCESS_FUNCTION::testRunStart()
{
  STATE_LOCK();
  GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
  GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
  GreaseOut.TestRun.Enable = true;      // Enable test run mode
  GreaseOut.Sleep.Enable = false;       // Disable sleep mode
  STATE_UNLOCK();
}

/**********************
 * TEST RUN FUNCTION
 * Manages the test run operation mode for system verification
 **********************/
void GREASE_PROCESS_FUNCTION::_testRun()
{

  if (!_proximity_pressed)
  {
    _proximity_pressed = true;

    GreaseOut.Proximity.Seconds = 0;
    LubricationReadCount++;
  }

  if (LubricationReadCount >= Pattern[LubricationCycle])
  {

    _lubricationProcessCalculation();          // Round values for calculation
    GreaseOut.Lubrication.LoraDataSend = true; // Enable LoRa data send for lubrication updates
    GreaseOut.Proximity.Seconds = 0;           // Reset proximity sensor timer
    LubricationReadCount = 0;

    Reservoir.read(); // Read reservoir level

    LubricationReadCount = 0;
    LubricationCycle = (LubricationCycle + 1) % 4;
  }
}

/**********************
 * SLEEP START FUNCTION
 * Initiates the low-power sleep mode
 **********************/
void GREASE_PROCESS_FUNCTION::sleepStart()
{
  STATE_LOCK();
  GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
  GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
  GreaseOut.TestRun.Enable = false;     // Disable test run mode
  GreaseOut.Sleep.Enable = true;        // Enable sleep mode
  STATE_UNLOCK();
}

/**********************
 * STOP FUNCTION
 * Stops all operations and deactivates the motor
 **********************/
void GREASE_PROCESS_FUNCTION::stop()
{
  STATE_LOCK();
  GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
  GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
  GreaseOut.TestRun.Enable = false;     // Disable test run mode
  GreaseOut.Sleep.Enable = false;       // Disable sleep mode
  STATE_UNLOCK();
  Motor.off(); // Turn off motor
}

/**********************
 * ROUND TO ONE DECIMALS FUNCTION
 * Rounds grease quantities to one decimal place
 **********************/
void GREASE_PROCESS_FUNCTION::_lubricationProcessCalculation()
{
  AboutDevice.total.greaseOut();

  if (Alert.EnableDisable.Supervisory1 || Alert.EnableDisable.Supervisory2 || Alert.EnableDisable.Supervisory3)
  {
    SupervisoryTime.AlertCheckOutGrease += LUB_DISCHARGE;
  }

  if (Alert.EnableDisable.Supervisory1)
  {
    GreaseOut.Lubrication.Temp.GramSupervisory_1 += LUB_DISCHARGE;

    if (GreaseOut.Lubrication.Temp.GramSupervisory_1 >= SUPERVISORY_GREASE_SETTLING)
    {
      GreaseOut.Lubrication.Temp.GramSupervisory_1 = SUPERVISORY_GREASE_SETTLING;
      SupervisoryTime.SupervisoryNominalTakeOutGrease_1 += LUB_DISCHARGE;
    }
    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_1 >= SUPERVISORY_NOMINAL_GRAM_INIT)
    {
      SupervisoryTime.SupervisoryNominalTakeOutGrease_1 = SUPERVISORY_NOMINAL_GRAM_INIT;
      SupervisoryTime.SupervisoryAverageTakeOutGrease_1 += LUB_DISCHARGE;
    }
  }

  if (Alert.EnableDisable.Supervisory2)
  {
    GreaseOut.Lubrication.Temp.GramSupervisory_2 += LUB_DISCHARGE;

    if (GreaseOut.Lubrication.Temp.GramSupervisory_2 >= SUPERVISORY_GREASE_SETTLING)
    {
      GreaseOut.Lubrication.Temp.GramSupervisory_2 = SUPERVISORY_GREASE_SETTLING;
      SupervisoryTime.SupervisoryNominalTakeOutGrease_2 += LUB_DISCHARGE;
    }
    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_2 >= SUPERVISORY_NOMINAL_GRAM_INIT)
    {
      SupervisoryTime.SupervisoryNominalTakeOutGrease_2 = SUPERVISORY_NOMINAL_GRAM_INIT;
      SupervisoryTime.SupervisoryAverageTakeOutGrease_2 += LUB_DISCHARGE;
    }
  }

  if (Alert.EnableDisable.Supervisory3)
  {
    GreaseOut.Lubrication.Temp.GramSupervisory_3 += LUB_DISCHARGE;

    if (GreaseOut.Lubrication.Temp.GramSupervisory_3 >= SUPERVISORY_GREASE_SETTLING)
    {
      GreaseOut.Lubrication.Temp.GramSupervisory_3 = SUPERVISORY_GREASE_SETTLING;
      SupervisoryTime.SupervisoryNominalTakeOutGrease_3 += LUB_DISCHARGE;
    }

    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_3 >= SUPERVISORY_NOMINAL_GRAM_INIT)
    {
      SupervisoryTime.SupervisoryNominalTakeOutGrease_3 = SUPERVISORY_NOMINAL_GRAM_INIT;
      SupervisoryTime.SupervisoryAverageTakeOutGrease_3 += LUB_DISCHARGE;
    }
  }
}

/**********************
 * BEGIN FUNCTION
 * Initializes grease process state based on previous operation mode
 **********************/
void GREASE_PROCESS_FUNCTION::begin()
{
  if (!GreaseOut.Sleep.Enable)
  {
    if (oldStateCount == 1)
    {
      GreaseOut.Lubrication.Enable = true;
    }
    else if (oldStateCount == 2)
    {
      GreaseOut.PauseTime.Enable = true;
    }
    else if (oldStateCount == 3)
    {
      GreaseOut.TestRun.Enable = true;
      TestRunOn();
      GreaseOut.Lubrication.Enable = false;
      GreaseOut.PauseTime.Enable = false;
    }
  }
  oldStateCount = 0;
}

/**********************
 * TEST RUN PAGE ENABLE/DISABLE FUNCTION
 * Handles enabling and disabling test run mode via button presses
 **********************/
void GREASE_PROCESS_FUNCTION::TestRunPageEnableDisable()
{
  if (!AdminSetting.Enable)
  {
    if (!MenuEnable && Button.ok.isPressed() && Button.up.isPressed() && start_init == 1 && !Keylock)
    {
      TestRunOn();
    }
    else if (!MenuEnable && Button.ok.isPressed() && Button.down.isPressed() && start_init == 1 && !Keylock)
    {
      TestRunOff();
    }
    else if (!MenuEnable && Button.ok.isPressed() && (Button.up.isPressed() || Button.down.isPressed()) && start_init == 0 && !Keylock)
    {
      TestRunEnableError = true;
    }
  }
}

/**********************
 * TEST RUN ON FUNCTION
 * Enables test run mode and updates system state
 **********************/
void GREASE_PROCESS_FUNCTION::TestRunOn()
{
  Button.ok.wait();
  Button.up.wait();
  Button.down.wait();

  STATE_LOCK();
  GreaseOut.TestRun.Enable = true;
  GreaseOut.Lubrication.Enable = false;
  GreaseOut.PauseTime.Enable = false;
  STATE_UNLOCK();

  Motor.on();
  Led.Testrun_on();

  Lora_twp.TestRunOn = true;

  vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
}

/**********************
 * TEST RUN OFF FUNCTION
 * Disables test run mode and resets system state
 **********************/
void GREASE_PROCESS_FUNCTION::TestRunOff()
{
  Button.ok.wait();
  Button.up.wait();
  Button.down.wait();

  GreaseOut.TestRun.Enable = false;

  Motor.off();
  Led.Testrun_off();

  Reset = true;
  Grease.TotalProxityCount = 0;
  Lora_disconnected_count = 0;

  Lora_twp.TestRunOff = true;
  vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
}

/**********************
 * RESET PROCESS FUNCTION
 * Resets grease process state to initial conditions
 **********************/
void GREASE_PROCESS_FUNCTION::ResetProcess()
{
  GreaseOut.PauseTime.Seconds = 60; // Set pause seconds counter
  GreaseOut.Proximity.Seconds = 0;  // Reset proximity sensor timing counter
  GreaseOut.Lubrication.Temp.Grams = GreaseOut.Lubrication.Grams;

  STATE_LOCK();
  GreaseOut.Lubrication.Enable = true;                          // Re-enable lubrication process
  GreaseOut.PauseTime.Enable = false;                           // Disable pause time mode
  GreaseOut.TestRun.Enable = false;                             // Disable test run mode
  GreaseOut.PauseTime.Seconds = 0;                              // Reset pause time seconds counter
  GreaseOut.PauseTime.Temp.Minute = GreaseOut.PauseTime.Minute; // Reset temporary pause minutes
  GreaseOut.PauseTime.Temp.Hour = GreaseOut.PauseTime.Hour;     // Reset temporary pause hours
  STATE_UNLOCK();
}

void GreaseTimerFunction::Proximity()
{
  if (GreaseOut.Lubrication.Enable || GreaseOut.Proximity.AlertCheck || GreaseOut.TestRun.Enable)
  {
    GreaseOut.Proximity.Seconds++;
  }
}

void GreaseTimerFunction::AppOnline()
{
  if (GreaseOut.Lubrication.Enable)
  {
    LoraFun.AppOnlineSec++;

    if (LoraFun.AppOnlineSec > APP_ONLINE_TIMEOUT_SEC)
    {
      LoraFun.AppOnlineSec = APP_ONLINE_TIMEOUT_SEC;
    }
  }
}

void GreaseTimerFunction::PauseTime()
{
  if (GreaseOut.PauseTime.Enable)
  {
    GreaseOut.PauseTime.Seconds--;
  }
}

void GREASE_PROCESS_FUNCTION::LoraLiveProcess()
{

  if (GreaseOut.Lora_PauseStart)
  {
    LoRa.liveProcess_Pause_Time_Start(); // Send LoRa live process update (code 4)
    GreaseOut.Lora_PauseStart = false;   // Reset LoRa pause start flag
  }
  if (GreaseOut.Lora_LubricationStart)
  {
    LoRa.liveProcess_Lubrication_Start();    // Send LoRa live process update (code 3)
    GreaseOut.Lora_LubricationStart = false; // Reset LoRa lubrication start flag
  }
  if (GreaseOut.Lubrication.LoraDataSend) // Check if within online time window
  {
    LoRa.liveProcess_Lubrication();             // Send LoRa live process update (code 1)
    GreaseOut.Lubrication.LoraDataSend = false; // Reset LoRa data send flag
  }
  if (GreaseOut.PauseTime.LoraDataSend)
  {
    LoRa.liveProcess_Pause_Time();            // Send LoRa live process update (code 2)
    GreaseOut.PauseTime.LoraDataSend = false; // Reset LoRa data send flag for pause time
  }

  if (GreaseOut.Reservoir.Temp._old_reservoir_value != GreaseOut.Reservoir.Percentage) // If reservoir percentage has changed
  {
    char buffer[20];                                                                // Buffer for formatted percentage string
    snprintf(buffer, sizeof(buffer), "%.0f%%", GreaseOut.Reservoir.Percentage);     // Format percentage as string (bounded)
    LoRa.Reservoir(buffer);                                                         // Send updated reservoir percentage via LoRa
    GreaseOut.Reservoir.Temp._old_reservoir_value = GreaseOut.Reservoir.Percentage; // Update old reservoir value
  }

  LoRa.PumpStatus();

  static bool lubricationProcessSent = false; // Flag to track if live process message has been sent
  static bool pauseTimeSent = false;          // Flag to track if pause time message has been sent

  if (GreaseOut.Lubrication.Enable && !lubricationProcessSent)
  {
    LoRa.publish(LoraTopics.LiveMode, PlayLoadMessage.LubricationStart); // Publish lubrication start message to LoRa
    lubricationProcessSent = true;                                       // Set flag to indicate message has been sent
  }
  else if (!GreaseOut.Lubrication.Enable)
  {
    lubricationProcessSent = false; // Reset flag when lubrication is not enabled
  }

  if (GreaseOut.PauseTime.Enable && !pauseTimeSent)
  {
    LoRa.publish(LoraTopics.LiveMode, PlayLoadMessage.PauseTimeStart); // Publish pause time start message to LoRa
    pauseTimeSent = true;                                              // Set flag to indicate message has been sent
  }
  else if (!GreaseOut.PauseTime.Enable)
  {
    pauseTimeSent = false; // Reset flag when pause time is not enabled
  }
}