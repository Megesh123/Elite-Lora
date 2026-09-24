/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Alert handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * ALERT FUNCTION
 * Handles alert notifications and processing
 **********************/
void LoRaFunction::alert(String data)
{
  if (LoraJoined) // If LoRa connected
  {
    LoRa.publish(LoraTopics.alert, data); // Publish alert message
  }
  else // If offline
  {
    if (flash.OnceWrite) // Check if flash write is allowed
    {
      Flash.write(data);       // Store in flash
      flash.OnceWrite = false; // Prevent duplicate writes
    }
  }
}

/**********************
 * OVERLOAD ALERT FUNCTION
 * Sends overload alert notifications via LoRa
 **********************/
void LoRaFunction::OverloadAlert()
{
  if (!Alert.Lora.DataSend) // Check if LoRa data not already sent
  {
    char buffer[30];                                           // Create character buffer for message
    sprintf(buffer, "OVERLOAD-%.1f", CurrentMonitor.measured); // Format overload message with current value

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds

    LoRa.NEXUS_IN_ALERT(NexuxInAlert.OVERLOAD); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1));               // Short delay for processing

    LoRa.liveProcess_Alert();        // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for message processing
  }
}

/**********************
 * RESERVOIR LOW LEVEL ALERT FUNCTION
 * Sends reservoir low level alert notifications via LoRa
 **********************/
void LoRaFunction::ReservoirLowLevelAlert()
{

  if (!Alert.Lora.DataSend) // Check if LoRa data not already sent
  {
    char buffer[30];                                                             // Create character buffer for message
    sprintf(buffer, "RESERVOIR LOW LEVEL-%.0f", GreaseOut.Reservoir.Percentage); // Format reservoir message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                              // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                          // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.RESERVOIR_LOW_LEVEL); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));                       // Delay for message processing
  }
}

void LoRaFunction::SignalNotDetected()
{
  if (!Alert.Lora.DataSend) // Check if LoRa data not already sent
  {
    char buffer[30];                        // Create character buffer for message
    sprintf(buffer, "SIGNAL NOT DETECTED"); // Format proximity message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();     // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1)); // Update live data process
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.SND);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for message processing
  }
}

/**********************
 * SUPERVISORY ALERT FUNCTION
 * Handles supervisory alert checks and notifications
 **********************/
void LoRaFunction::SupervisoryALert()
{
  if (!Alert.Lora.DataSend) // Check if LoRa data not already sent
  {
    _supervisory1_Alert(); // Check and send supervisory 1 alert
    _supervisory2_Alert(); // Check and send supervisory 2 alert
    _supervisory3_Alert(); // Check and send supervisory 3 alert
  }
}

/**********************
 * SUPERVISORY 1 ALERT FUNCTION
 * Handles supervisory 1 alert checks and notifications
 **********************/
void LoRaFunction::_supervisory1_Alert()
{
  if (Alert.NameCount == 3) // Check if alert is enabled and is supervisory 1
  {
    char buffer[30];                  // Create character buffer for message
    sprintf(buffer, "SUPERVISORY 1"); // Format supervisory message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                      // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                  // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.SUPERVISORY); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));               // Delay for message processing
  }
}

/**********************
 * SUPERVISORY 2 ALERT FUNCTION
 * Handles supervisory 2 alert checks and notifications
 **********************/
void LoRaFunction::_supervisory2_Alert()
{
  if (Alert.NameCount == 4) // Check if alert is enabled and is supervisory 2
  {
    char buffer[30];                  // Create character buffer for message
    sprintf(buffer, "SUPERVISORY 2"); // Format supervisory message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                      // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                  // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.SUPERVISORY); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));               // Delay for message processing
  }
}

/**********************
 * SUPERVISORY 3 ALERT FUNCTION
 * Handles supervisory 3 alert checks and notifications
 **********************/
void LoRaFunction::_supervisory3_Alert()
{
  if (Alert.NameCount == 5) // Check if alert is enabled and is supervisory 3
  {
    char buffer[30];                  // Create character buffer for message
    sprintf(buffer, "SUPERVISORY 3"); // Format supervisory message
    int actual_length = strlen(buffer);

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                      // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                  // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.SUPERVISORY); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));               // Delay for message processing
  }
}

/**********************
 * PUMP WEAK ALERT FUNCTION
 * Handles pump weak alert checks and notifications
 **********************/
void LoRaFunction::PumpWeakAlert()
{
  if (!Alert.Lora.DataSend) // Check if LoRa data not already sent
  {
    _pumpElement1_Weak(); // Check and send pump element 1 weak alert
    _pumpElement2_Weak(); // Check and send pump element 2 weak alert
    _pumpElement3_Weak(); // Check and send pump element 3 weak alert
  }
}

/**********************
 * PUMP ELEMENT 1 WEAK ALERT FUNCTION
 * Handles pump element 1 weak alert checks and notifications
 **********************/
void LoRaFunction::_pumpElement1_Weak()
{
  if (Alert.NameCount == 7) // Check if alert is enabled and is pump weak 1
  {
    char buffer[30];                // Create character buffer for message
    sprintf(buffer, "PUMP WEAK 1"); // Format pump weak message
    int actual_length = strlen(buffer);

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                    // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.PUMP_WEAK); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));             // Delay for message processing
  }
}

/**********************
 * PUMP ELEMENT 2 WEAK ALERT FUNCTION
 * Handles pump element 2 weak alert checks and notifications
 **********************/
void LoRaFunction::_pumpElement2_Weak()
{
  if (Alert.NameCount == 8) // Check if alert is enabled and is pump weak 2
  {
    char buffer[30];                // Create character buffer for message
    sprintf(buffer, "PUMP WEAK 2"); // Format pump weak message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                    // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.PUMP_WEAK); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));             // Delay for message processing
  }
}

/**********************
 * PUMP ELEMENT 3 WEAK ALERT FUNCTION
 * Handles pump element 3 weak alert checks and notifications
 **********************/
void LoRaFunction::_pumpElement3_Weak()
{
  if (Alert.NameCount == 9) // Check if alert is enabled and is pump weak 3
  {
    char buffer[30];                // Create character buffer for message
    sprintf(buffer, "PUMP WEAK 3"); // Format pump weak message

    LoRa.alert(buffer);           // Send alert via LoRa
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for processing

    LoRa.liveProcess_Alert();                    // Update live data process
    vTaskDelay(pdMS_TO_TICKS(1));                // Short delay for processing
    LoRa.NEXUS_IN_ALERT(NexuxInAlert.PUMP_WEAK); // Send Nexus alert
    vTaskDelay(pdMS_TO_TICKS(1000));             // Delay for message processing
  }
}

/**********************
 * ALERT CLEAR PROCESS
 * Handles alert clearance messages
 **********************/
void _alert_clear_process(String payload)
{
  if (payload.indexOf(PlayLoadMessage.AlertRectified) != -1) // If alert rectified message
  {
    LoRa.response(LoraTopics.alert_clear_return, Response.Ok); // Send OK response
    Alert.SnoozeReservoir = false;                             // Disable reservoir snooze
    if (Alert.Name == AlertName.ReservoirLowLevel.c_str())
    {
      if (GreaseOut.Reservoir.Percentage == 100)
      {
        Alert.Enable = false; // Disable alert
        SetAlertName("NO");   // Reset alert name
        AlertFun.clear();     // Clear alert

        vTaskDelay(pdMS_TO_TICKS(100));
        VariableUpdate();
      }
      else
      {
        LoRa.response(LoraTopics.alert_clear_return, Response.Error); // Send ERROR response
      }
    }
    else
    {
      Alert.Enable = false; // Disable alert
      SetAlertName("NO");   // Reset alert name
      AlertFun.clear();     // Clear alert

      vTaskDelay(pdMS_TO_TICKS(100));
      VariableUpdate();
    }
  }
  else
  {
    LoRa.response(LoraTopics.alert_clear_return, Response.Error); // Send ERROR response
  }
}

/**********************
 * ALERT DATA PROCESS
 * Handles alert data acknowledgment messages
 **********************/
void _alert_data_process(String payload)
{
  if (payload.indexOf(PlayLoadMessage.Ok) != -1) // If OK message
  {
    Alert.Lora.DataSend = true; // Mark LoRa data as sent
  }
}

long per_state;
bool state = false;

void ALERTFUNCTION::Buzzer()
{
  if (Alert.Enable)
  {
    if (millis() - per_state > 1000)
    {
      per_state = millis();

      state = !state;

      if (state)
      {
        digitalWrite(PIN_BUZZER, HIGH);
      }
      else
      {
        digitalWrite(PIN_BUZZER, LOW);
      }
    }
  }
  else if (!ResetCompleted)
  {
    digitalWrite(PIN_BUZZER, LOW);
  }
}