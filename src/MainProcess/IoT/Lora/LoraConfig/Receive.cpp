/**
 * @file Receive.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 2.0.0 - Robust RX handling
 */

#include "Headerfile.h"

/**********************
 * Helper: check if topic matches expected (exact or suffix)
 * This handles both cases:
 *  - "9017.../ACK_DASHBOARD" exact match
 *  - "/ACK_DASHBOARD" suffix match (if ClientID stripped)
 **********************/
static bool topicMatches(const String &incoming, const String &expectedFull)
{
  if (incoming == expectedFull) return true;
  // expectedFull is like "CLIENTID/ACK_DASHBOARD", extract suffix "/ACK_DASHBOARD"
  int slash = expectedFull.indexOf('/');
  if (slash >= 0)
  {
    String suffix = expectedFull.substring(slash); // "/ACK_DASHBOARD"
    if (incoming.endsWith(suffix)) return true;
    // Also handle incoming without leading slash: "ACK_DASHBOARD"
    if (incoming == suffix.substring(1)) return true;
  }
  else
  {
    if (incoming.endsWith(expectedFull)) return true;
  }
  return false;
}

static bool topicIs(const String &incoming, const char *suffix)
{
  String s = suffix;
  if (incoming == s) return true;
  if (incoming.endsWith(s)) return true;
  // Remove leading slash for comparison
  if (s.startsWith("/") && incoming.endsWith(s.substring(1))) return true;
  return false;
}

/**********************
 * LORA MESSAGE PROCESSOR
 * Routes messages to appropriate handlers based on topic and payload
 * v2.1.0 - topic/payload sanitized on entry (defense in depth); processLoRaLine
 * already sanitizes, this guards any other callers of _receive_message_process.
 **********************/
void _receive_message_process(String topic, String payload)
{
  topic = loraSanitize(topic);
  payload = loraSanitize(payload);
  topic.trim();
  payload.trim();

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.print("[RX PROCESS] Topic='");
  Serial.print(topic);
  Serial.print("' Payload='");
  Serial.print(payload);
  Serial.println("'");
  xSemaphoreGive(SerialMutex);

  // Always allow CONMODE even in offline mode
  if (topicMatches(topic, LoraTopics.ConMode_return) || topicIs(topic, "/ACK_CONMODE") || topicIs(topic, "/CONMODE"))
  {
    Received._conmode = payload;
    // Reset connection counters on any valid downlink
    LoraDetails.lora_connected_but_not_received = 0;
    LoraDetails.DashboardAckPending = false;
    return;
  }

  if (LoraDetails.UserEnterOfflineMode)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("[RX PROCESS] Offline mode, ignoring non-CONMODE");
    xSemaphoreGive(SerialMutex);
    return;
  }

  // Exact and suffix matching to be robust against ClientID variations
  if (topicMatches(topic, LoraTopics.Dashboard_return) || topicIs(topic, "/ACK_DASHBOARD"))
  {
    LoraDetails.lora_connected_but_not_received = 0;
    LoraDetails.DashboardAckPending = false;
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("[RX] Dashboard ACK received");
    xSemaphoreGive(SerialMutex);
  }
  else if (topicMatches(topic, LoraTopics.offline_return) || topicIs(topic, "/ASK_OFFLINE"))
  {
    Received._offline_data_process = payload;
  }
  else if (topicMatches(topic, LoraTopics.subscribe) || topicIs(topic, "/APP"))
  {
    Received._setting_data_process = payload;
  }
  else if (topicMatches(topic, LoraTopics.alert_return) || topicIs(topic, "/ACK_ALERT"))
  {
    Received._alert_data_process = payload;
  }
  else if (topicMatches(topic, LoraTopics.alert_clear) || topicIs(topic, "/ALERTRECTIFIED") || topicIs(topic, "/ASK_ALERTRECTIFIED"))
  {
    Received._alert_clear_process = payload;
  }
  else if (topicMatches(topic, LoraTopics.LiveController_return) || topicIs(topic, "/ASK_LIVE"))
  {
    Received._controller_online_came_return = payload;
  }
  else if (topicMatches(topic, LoraTopics.Setting_return) || topicIs(topic, "/ACK_CON_SDP"))
  {
    Received._Setting = payload;
  }
  else if (topicMatches(topic, LoraTopics.Mode) || topicIs(topic, "/MODE"))
  {
    Received._mode = payload;
    LoraDetails.lora_connected_but_not_received = 0;
  }
  else if (topicMatches(topic, LoraTopics.ConMode) || topicIs(topic, "/CONMODE"))
  {
    // Some app versions send CONMODE without ACK prefix for commands
    Received._conmode = payload;
  }
  // Handle other ACK topics that Python auto-returns - at least reset counters
  else if (topicIs(topic, "/ACK_APP") ||
           topicIs(topic, "/ACK_FACTORYRESET") ||
           topicIs(topic, "/ACK_REFILL") ||
           topicIs(topic, "/ACKREFILLMODE") ||
           topicIs(topic, "/ACK_TESTRUN") ||
           topicIs(topic, "/ASK_SLEEPMODE") ||
           topicIs(topic, "/ASK_RECONFIG") ||
           topicIs(topic, "/ASK_SNOOZE") ||
           topicIs(topic, "/ACK_PUMP") ||
           topicIs(topic, "/ACK_SUPERVISORY") ||
           topicIs(topic, "/ACK_REFILLALERT") ||
           topicIs(topic, "/ACK_CONMODE"))
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[RX] Generic ACK received: ");
    Serial.println(topic);
    xSemaphoreGive(SerialMutex);
    LoraDetails.lora_connected_but_not_received = 0;
    // If payload is OK, treat as success
    if (payload == "OK")
    {
      // For generic ACKs, we can store as Setting or conmode if needed
      // Dashboard already handled, others just log
    }
  }
  else
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[RX] UNKNOWN TOPIC: ");
    Serial.println(topic);
    xSemaphoreGive(SerialMutex);
    // Fallback: if topic contains MODE, treat as MODE command (robustness)
    if (topic.indexOf("/MODE") >= 0 || topic == "MODE")
    {
      Received._mode = payload;
    }
  }
}

void Receive()
{
  if (Received._offline_data_process != "")
  {
    Received._offline_data_process = "";
  }
  if (Received._setting_data_process != "")
  {
    _setting_data_process(Received._setting_data_process);
    Received._setting_data_process = "";
  }
  if (Received._alert_data_process != "")
  {
    _alert_data_process(Received._alert_data_process);
    Received._alert_data_process = "";
  }
  if (Received._alert_clear_process != "")
  {
    _alert_clear_process(Received._alert_clear_process);
    Received._alert_clear_process = "";
  }
  if (Received._controller_online_came_return != "")
  {
    _controller_online_came_return(Received._controller_online_came_return);
    Received._controller_online_came_return = "";
  }
  if (Received._Setting != "")
  {
    if (Received._Setting.indexOf("OK") != -1)
    {
      Reset_SendData_Lora = false;
    }
    Reset_sendData_Lora_Subscribe = false;
    Received._Setting = "";
  }
  if (Received._mode != "")
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[RECEIVE] Processing MODE payload: ");
    Serial.println(Received._mode);
    xSemaphoreGive(SerialMutex);

    _refillMode_start_stop(Received._mode);
    _testRun_start_stop(Received._mode);
    _controller_sleep_process(Received._mode);
    _control_Snooze(Received._mode);
    _app_online_offline_check(Received._mode);
    _refill_complete_process(Received._mode);
    _factory_reset_process(Received._mode);
    _reconfig_process(Received._mode);
    _refill_alert_enable_disable(Received._mode);
    _supervisory_Enable_Disable(Received._mode);

    Received._mode = "";
  }

  if (Received._conmode != "")
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[RECEIVE] CONMODE payload: ");
    Serial.println(Received._conmode);
    xSemaphoreGive(SerialMutex);

    if (Received._conmode == "OK" || Received._conmode.indexOf("OK") >= 0)
    {
      Twoway.Return();
    }
    else
    {
      Serial.println("CON MODE - UNKNOWN, still calling Return for robustness");
      Twoway.Return();
    }
    Received._conmode = "";
  }
}
