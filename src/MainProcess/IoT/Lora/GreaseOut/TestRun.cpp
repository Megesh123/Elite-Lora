/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Test run handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * TEST RUN START FUNCTION
 * Handles test run operation initiation
 **********************/
void LoRaFunction::testRunStart()
{
  Grease.testRunStart(); // Start test run mode
  Led.Testrun_on();      // Activate test run LED indication
  Motor.on();            // Turn on motor
  ResetValue();          // Reset operational values
  GreaseOut.TestRun.sec = TEST_RUN_SEC;
  GreaseOut.TestRun.min = TEST_RUN_MIN;
}

/**********************
 * TEST RUN STOP FUNCTION
 * Handles test run operation termination
 **********************/
void LoRaFunction::testRunStop()
{
  Motor.reset(); // Reset motor
  Grease.stop(); // Stop grease system
  GreaseOut.TestRun.sec = TEST_RUN_SEC;
  GreaseOut.TestRun.min = TEST_RUN_MIN;
  Led.Testrun_off(); // Deactivate test run LED indication
}

/**********************
 * TEST RUN START/STOP PROCESS
 * Handles test run start/stop commands
 **********************/
void _testRun_start_stop(String payload)
{
  if (payload.indexOf(PlayLoadMessage.TestRunStart) != -1 && start_init == 1) // If test run start message
  {
    LoRa.testRunStart();                                   // Start test run operation
    LoRa.liveProcess_TestRun();                            // Update live process status
    LoRa.response(LoraTopics.testrun_return, Response.Ok); // Send OK response
  }
  else if (payload.indexOf(PlayLoadMessage.TestRunStop) != -1 && start_init == 1) // If test run stop message
  {
    Motor.reset();                                         // Reset motor
    Grease.stop();                                         // Stop grease system
    Grease.ResetProcess();                                 // Reset grease processing state
    Eeprom.write();                                        // Write to EEPROM
    Led.Testrun_off();                                     // Deactivate test run LED indication
    LoRa.response(LoraTopics.testrun_return, Response.Ok); // Send OK response
  }
  else if (payload.indexOf(PlayLoadMessage.TestRunStart) != -1 && start_init != 1)
  {
    LoRa.response(LoraTopics.testrun_return, Response.Error); // Send ERROR response
    TestRunEnableError = true;
  }
  else if (payload.indexOf(PlayLoadMessage.TestRunStop) != -1 && start_init != 1)
  {
    LoRa.response(LoraTopics.testrun_return, Response.Error); // Send ERROR response
    TestRunEnableError = true;
  }

  // else
  // {
  //   LoRa.response(LoraTopics.testrun_return, Response.Error); // Send ERROR response
  // }
}