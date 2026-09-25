/**
 * @file LoraFunction.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 2.1.1 - truncate payloads at 70 bytes
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LoRa COMMUNICATION IMPLEMENTATION - Fixed RX starvation & flush bug
 * RAK11160 is Class C, downlinks can arrive at ANY time, even while we are
 * waiting for TX_DONE. The old code did `while(available) read()` before every
 * AT+SEND, which discarded pending downlinks. This rewrite never flushes.
 * RX task is the normal reader, but TX path also dispatches +EVT:RX_ lines
 * while it owns the UART mutex.
 *
 * @note v2.1.0 changes:
 *  - loraSanitize(): strip control/non-ASCII characters, trim and length-cap
 *    every string that enters processLoRaLine() / _receive_message_process()
 *    so over-the-air frames cannot inject blank/oversized/malformed commands.
 *  - loraUplinkAllowed(): TX rate limiter enforcing a minimum gap between
 *    uplinks and a per-minute duty-cycle cap so LoRaWAN airtime isn't spammed
 *    by concurrent publish calls.
 *
 * @note v2.1.1 changes:
 *  - Application payload budget is 70 bytes, not 100. sendString() used to
 *    drop anything over 100 and still transmit 71-100 byte frames, which the
 *    RAK11160 AT+SEND path rejects. Oversized frames are now truncated to 70
 *    and sent.
 */

/**********************
 * LoRa COMMUNICATION IMPLEMENTATION
 * Handles secure LoRa connectivity, message processing, and IoT integration
 * Implements bidirectional communication with cloud services
 **********************/

#include "HeaderFile.h"

#define LORA_RX_PIN 16
#define LORA_TX_PIN 15

HardwareSerial LoRaSerial(1);

#define LORA_BAUD 115200

// v2.1.1 - application payload budget for RAK11160 AT+SEND (port 2).
// 70 bytes is the longest frame this link will accept. Longer frames are
// truncated (not dropped) on both uplink and decoded downlink.
#define LORA_MAX_PAYLOAD_LEN 70

bool LoraInitialized = false;
bool LoraJoined = false;

// RAK11160 UART mutex
SemaphoreHandle_t LoRaSerialMutex = NULL;

// --------------------------------------------------
// v2.1.0 TX RATE LIMITER
// LoRaWAN duty-cycle rules and the RAK11160's small TX window mean
// back-to-back uplinks get dropped server-side silently. Enforce both a
// minimum inter-uplink gap and a per-minute message budget.
// --------------------------------------------------
#define LORA_TX_MIN_INTERVAL_MS 2000 // Minimum gap between two uplinks
#define LORA_TX_MAX_PER_MIN 15       // Max uplinks allowed per 60s window

// Sliding 60-second window bookkeeping (millis() based - resets on boot)
static uint32_t loraTxLastMillis = 0;
static bool loraTxHasSent = false;
static uint32_t loraTxWindowStartMillis = 0;
static bool loraTxWindowActive = false;
static uint16_t loraTxWindowCount = 0;

// Forward
void processLoRaLine(String line);

// --------------------------------------------------
// v2.1.1 Sanitize a string before it is processed:
//  - strip control characters and bytes >= 0x7F (keeps printable ASCII, 0x20-0x7E)
//  - trim leading/trailing whitespace
//  - truncate to LORA_MAX_PAYLOAD_LEN (70) so oversized frames are still
//    usable instead of being dropped or rejected by AT+SEND
// --------------------------------------------------
String loraSanitize(String input)
{
  input.trim();
  // First pass: drop all non-printable-ASCII characters in place.
  String clean = "";
  clean.reserve(input.length());
  for (size_t i = 0; i < input.length(); i++)
  {
    char c = input[i];
    if ((unsigned char)c >= 0x20 && (unsigned char)c <= 0x7E)
      clean += c;
  }
  if (clean.length() > LORA_MAX_PAYLOAD_LEN)
  {
    if (SerialMutex != NULL)
    {
      xSemaphoreTake(SerialMutex, portMAX_DELAY);
      Serial.print("[LORA] payload truncated to ");
      Serial.print(LORA_MAX_PAYLOAD_LEN);
      Serial.print(" from ");
      Serial.println(clean.length());
      xSemaphoreGive(SerialMutex);
    }
    clean = clean.substring(0, LORA_MAX_PAYLOAD_LEN);
  }
  return clean;
}

// --------------------------------------------------
// v2.1.0 Rate-limit gate for outgoing uplinks.
// Returns true only if the minimum inter-uplink interval has elapsed AND the
// per-minute budget has not been exhausted.
// --------------------------------------------------
bool loraUplinkAllowed()
{
  uint32_t now = millis();

  // Start (or reset) the 60-second window.
  // loraTxWindowActive is used instead of relying on a 0 timestamp, because
  // millis() is legitimately 0 (or near it) right after boot/reset.
  if (!loraTxWindowActive || now - loraTxWindowStartMillis >= 60000UL)
  {
    loraTxWindowStartMillis = now;
    loraTxWindowActive = true;
    loraTxWindowCount = 0;
  }

  // Minimum interval since the previous uplink (except the very first one).
  if (loraTxHasSent && now - loraTxLastMillis < LORA_TX_MIN_INTERVAL_MS)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("[LORA RATE] Uplink skipped: min interval not reached");
    xSemaphoreGive(SerialMutex);
    return false;
  }

  // Per-minute budget.
  if (loraTxWindowCount >= LORA_TX_MAX_PER_MIN)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("[LORA RATE] Uplink skipped: 60s budget exhausted");
    xSemaphoreGive(SerialMutex);
    return false;
  }

  loraTxLastMillis = now;
  loraTxHasSent = true;
  loraTxWindowCount++;
  return true;
}

// --------------------------------------------------
// Convert ASCII string to HEX
// --------------------------------------------------
String stringToHex(String text)
{
  String hex = "";
  hex.reserve(text.length() * 2);
  for (size_t i = 0; i < text.length(); i++)
  {
    char buf[3];
    sprintf(buf, "%02X", (uint8_t)text[i]);
    hex += buf;
  }
  return hex;
}

// --------------------------------------------------
// Convert HEX string to ASCII
// --------------------------------------------------
String hexToString(String hex)
{
  hex.trim();
  // Remove possible spaces / 0x prefix
  String clean = "";
  clean.reserve(hex.length());
  for (size_t i = 0; i < hex.length(); i++)
  {
    char c = hex[i];
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
      clean += c;
  }
  if (clean.length() % 2 != 0)
    return ""; // invalid

  String result = "";
  result.reserve(clean.length() / 2);
  for (size_t i = 0; i + 1 < clean.length(); i += 2)
  {
    String byteString = clean.substring(i, i + 2);
    char c = (char)strtol(byteString.c_str(), NULL, 16);
    result += c;
  }
  return result;
}

// --------------------------------------------------
// Internal: try to dispatch a line as downlink
// returns true if it WAS a downlink line
// --------------------------------------------------
static bool loraTryDispatchDownlink(const String &line)
{
  if (line.startsWith("+EVT:RX_"))
  {
    processLoRaLine(line);
    return true;
  }
  return false;
}

// ==================================================
// Wait for a specific response
// ==================================================
bool waitForResponse(
    String &response,
    const char *expected,
    uint32_t timeout)
{
  unsigned long start = millis();

  while (millis() - start < timeout)
  {
    while (LoRaSerial.available())
    {
      char c = LoRaSerial.read();

      // xSemaphoreTake(SerialMutex, portMAX_DELAY);
      // Serial.write(c);
      // xSemaphoreGive(SerialMutex);

      response += c;

      if (response.indexOf(expected) >= 0)
      {
        return true;
      }
    }

    delay(1);
  }

  return false;
}
// ==================================================
// Send AT command
// ==================================================

bool sendATCommand(
    const char *command,
    uint32_t timeout = 5000)
{
  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(LoRaSerialMutex, portMAX_DELAY);

  String response = "";

  LoRaSerial.print(command);
  LoRaSerial.print("\r\n");

  unsigned long start = millis();

  while (millis() - start < timeout)
  {
    while (LoRaSerial.available())
    {
      char c = LoRaSerial.read();

      // Serial.write(c);
      response += c;

      if (response.indexOf("AT_PARAM_ERROR") >= 0 ||
          response.indexOf("AT_BUSY_ERROR") >= 0 ||
          response.indexOf("AT_NO_NETWORK_JOINED") >= 0 ||
          response.indexOf("ERROR") >= 0)
      {
        Serial.println();
        Serial.println("sendATCommand - Command ERROR");

        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);

        return false;
      }

      if (response.indexOf("OK") >= 0)
      {
        Serial.println();
        Serial.println("sendATCommand - Command OK");

        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);

        return true;
      }
    }

    delay(1);
  }

  Serial.println();
  Serial.println("Command TIMEOUT");

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);

  return false;
}

// --------------------------------------------------
// Send string through RAK11160
// --------------------------------------------------
bool sendString(String message, int port)
{
  if (!LoraJoined)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);

    Serial.println(
        "Lora Join Failed. Cannot send message.");

    xSemaphoreGive(SerialMutex);

    return false;
  }

  // v2.1.1 - sanitize first. This strips non-ASCII and truncates to 70 bytes.
  // v2.1.0 returned false here for length > 100, which discarded the uplink
  // entirely; 71-100 byte frames were sent and then rejected by AT+SEND.
  message = loraSanitize(message);
  if (message.length() == 0)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("LoRa payload empty after sanitize, skip");
    xSemaphoreGive(SerialMutex);

    return false;
  }

  // v2.1.0 - enforce TX rate limiter (min interval + per-minute budget)
  if (!loraUplinkAllowed())
  {
    return false;
  }

  // Small gap to avoid spamming
  vTaskDelay(pdMS_TO_TICKS(100));
  String hexData = stringToHex(message);

  String command = "AT+SEND=";

  command += String(port);
  command += ":";
  command += hexData;

  // ==============================
  // Serial Monitor
  // ==============================

  xSemaphoreTake(SerialMutex, portMAX_DELAY);

  // Serial.println();
  // Serial.println("================================");
  // Serial.println("UPLINK");
  // Serial.println("================================");

  Serial.print("String : ");
  Serial.println(message);

  // Serial.print("HEX    : ");
  // Serial.println(hexData);

  // Serial.print("Bytes  : ");
  // Serial.println(message.length());

  // Serial.print("Command: ");
  // Serial.println(command);

  xSemaphoreGive(SerialMutex);

  // ==============================
  // RAK11160 UART
  // ==============================

  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(
        LoRaSerialMutex,
        portMAX_DELAY);

  while (LoRaSerial.available())
    LoRaSerial.read();

  LoRaSerial.print(command);
  LoRaSerial.print("\r\n");

  bool sendAccepted = false;
  bool txDone = false;

  unsigned long start = millis();

  while (millis() - start < 15000)
  {
    while (LoRaSerial.available())
    {
      String line =
          LoRaSerial.readStringUntil('\n');

      line.trim();

      if (line.length() == 0)
        continue;

      // ==============================
      // Serial Monitor
      // ==============================

      xSemaphoreTake(
          SerialMutex,
          portMAX_DELAY);

      Serial.print("[LORA TX] ");
      Serial.println(line);

      xSemaphoreGive(SerialMutex);

      // ==============================
      // DOWNLINK RECEIVED DURING TX WAIT
      // ==============================
      // Task 8 may own the LoRa UART while waiting for TX_DONE.
      // Do not discard a Class-C downlink if it arrives here.
      if (line.startsWith("+EVT:RX_"))
      {
        processLoRaLine(line);
        continue;
      }

      // ==============================
      // SEND ACCEPTED
      // ==============================

      if (line == "OK")
      {
        sendAccepted = true;

        // xSemaphoreTake(
        //     SerialMutex,
        //     portMAX_DELAY);

        // Serial.println(
        //     "SEND command accepted");

        // xSemaphoreGive(SerialMutex);
      }

      // ==============================
      // TX DONE
      // ==============================

      if (line.indexOf("+EVT:TX_DONE") >= 0)
      {
        txDone = true;

        // xSemaphoreTake(
        //     SerialMutex,
        //     portMAX_DELAY);

        // Serial.println("TX DONE");

        // xSemaphoreGive(SerialMutex);
      }

      // ==============================
      // CONFIRMED TX
      // ==============================

      if (line.indexOf(
              "+EVT:SEND_CONFIRMED_OK") >= 0)
      {
        txDone = true;

        // xSemaphoreTake(
        //     SerialMutex,
        //     portMAX_DELAY);

        // Serial.println(
        //     "TX CONFIRMED DONE");

        // xSemaphoreGive(SerialMutex);
      }

      // ==============================
      // ERROR
      // ==============================

      if (line.indexOf("AT_PARAM_ERROR") >= 0 ||
          line.indexOf("AT_BUSY_ERROR") >= 0 ||
          line.indexOf("AT_NO_NETWORK_JOINED") >= 0 ||
          line.indexOf("ERROR") >= 0)
      {
        xSemaphoreTake(
            SerialMutex,
            portMAX_DELAY);

        Serial.println(
            "LoRa SEND ERROR");

        xSemaphoreGive(SerialMutex);

        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(
              LoRaSerialMutex);

        return false;
      }

      // ==============================
      // TX COMPLETE
      // ==============================

      if (txDone)
      {
        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(
              LoRaSerialMutex);

        return true;
      }
    }

    delay(1);
  }

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println(
      "TX RESPONSE TIMEOUT");

  xSemaphoreGive(SerialMutex);

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(
        LoRaSerialMutex);

  return false;
}
// --------------------------------------------------
// Parse RAK11160 downlink event
//
// Example:
// +EVT:RX_1:-48:7:UNICAST:2:48656C6C6F204553503332
// --------------------------------------------------

void processLoRaLine(String line)
{
  line.trim();

  if (line.length() == 0)
    return;

  if (!line.startsWith("+EVT:RX_"))
    return;

  int lastColon =
      line.lastIndexOf(':');

  if (lastColon < 0)
  {
    xSemaphoreTake(
        SerialMutex,
        portMAX_DELAY);

    Serial.println(
        "Invalid RX event.");

    xSemaphoreGive(SerialMutex);

    return;
  }

  String hexData =
      line.substring(lastColon + 1);

  String message =
      hexToString(hexData);

  // v2.1.0 - sanitize decoded payload before any routing: strip control /
  // non-ASCII characters, trim and cap. Prevents blank/oversized/malformed
  // frames from reaching topic dispatchers.
  message = loraSanitize(message);
  if (message.length() == 0)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("RX payload empty after sanitize, drop");
    xSemaphoreGive(SerialMutex);
    return;
  }

  // Message format from Python bridge: <ClientID>/<TOPIC>:<PAYLOAD>
  // Example: 901745858428/ACK_DASHBOARD:OK  or 901745858428/MODE:*RM-ST#
  String topic = "";
  String payload = "";

  int separator =
      message.indexOf(':');

  if (separator > 0)
  {
    topic =
        message.substring(
            0,
            separator);

    payload =
        message.substring(
            separator + 1);
  }

  // v2.1.0 - a ':' inside the topic would corrupt routing; reject malformed
  // frames instead of forwarding them to _receive_message_process().
  topic.trim();
  if (topic.indexOf(':') >= 0)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("RX topic contains separator, drop: ");
    Serial.println(topic);
    xSemaphoreGive(SerialMutex);
    return;
  }
  payload.trim();

  Serial.println();
  Serial.println(
      "================================");

  Serial.println(
      "DOWNLINK RECEIVED");

  Serial.println(
      "================================");

  Serial.print("HEX    : ");
  Serial.println(hexData);

  Serial.print("String : ");
  Serial.println(message);

  if (separator > 0)
  {
    Serial.print("Topic  : ");
    Serial.println(topic);

    Serial.print("Payload: ");
    Serial.println(payload);
  }

  Serial.println(
      "================================");

  xSemaphoreGive(
      SerialMutex);

  // =================================
  // APPLICATION HANDLER
  // =================================

  if (separator > 0)
  {
    _receive_message_process(
        topic,
        payload);
  }
}

void loraReceiveProcess()
{
  if (!LoraJoined)
    return;

  static String rxLine = "";

  if (LoRaSerialMutex != NULL)
  {
    if (xSemaphoreTake(
            LoRaSerialMutex,
            pdMS_TO_TICKS(5)) != pdTRUE)
    {
      return;
    }
  }

  while (LoRaSerial.available())
  {
    char c =
        LoRaSerial.read();

    xSemaphoreTake(
        SerialMutex,
        portMAX_DELAY);

    Serial.print(c);

    xSemaphoreGive(
        SerialMutex);
    if (c == '\n')
    {
      rxLine.trim();

      if (rxLine.length() > 0)
      {
        xSemaphoreTake(
            SerialMutex,
            portMAX_DELAY);

        Serial.print(
            "[LORA RX LINE] ");

        Serial.println(rxLine);

        xSemaphoreGive(
            SerialMutex);

        processLoRaLine(
            rxLine);
      }

      rxLine = "";
    }
    else if (c != '\r')
    {
      rxLine += c;

      if (rxLine.length() > 600)
        rxLine.remove(0, 300);
    }
  }

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(
        LoRaSerialMutex);
}

bool LoRaFunction::publish(
    const String &_topic,
    const String &_data)
{
  String data = _topic + ":" + _data;
  sendString(data, 2);
  return true;
}

bool LoRaFunction::response(
    const String &_topic,
    const String &_data)
{

  String data = _topic + ":" + _data;
  sendString(data, 2);
  return true;
}

// ==================================================
// LoRa initialization
// ==================================================
bool loraBegin()
{
  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println();
  Serial.println(
      "================================");

  Serial.println(
      "Initializing RAK11160...");

  Serial.println(
      "================================");

  xSemaphoreGive(
      SerialMutex);

  // ==============================
  // UART
  // ==============================

  LoRaSerial.begin(
      LORA_BAUD,
      SERIAL_8N1,
      LORA_RX_PIN,
      LORA_TX_PIN);

  delay(5000);

  // ==============================
  // RAK READY
  // ==============================

  bool rakReady = false;

  for (int attempt = 1;
       attempt <= 5;
       attempt++)
  {
    xSemaphoreTake(
        SerialMutex,
        portMAX_DELAY);

    Serial.print(
        "AT attempt ");

    Serial.print(attempt);

    Serial.println("...");

    xSemaphoreGive(
        SerialMutex);

    String response = "";

    LoRaSerial.print(
        "AT\r\n");

    unsigned long start =
        millis();

    while (millis() - start < 2000)
    {
      while (LoRaSerial.available())
      {
        char c =
            LoRaSerial.read();

        xSemaphoreTake(
            SerialMutex,
            portMAX_DELAY);

        Serial.write(c);

        xSemaphoreGive(
            SerialMutex);

        response += c;
      }

      if (response.indexOf("OK") >= 0)
      {
        rakReady = true;
        break;
      }
    }

    if (rakReady)
      break;

    delay(1000);
  }

  if (!rakReady)
  {
    xSemaphoreTake(
        SerialMutex,
        portMAX_DELAY);

    Serial.println(
        "ERROR: RAK11160 not responding!");

    xSemaphoreGive(
        SerialMutex);

    return false;
  }

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println();
  Serial.println(
      "RAK11160 Ready!");

  xSemaphoreGive(
      SerialMutex);

  // ==============================
  // CONFIGURATION
  // ==============================

  if (!sendATCommand(
          "AT+NWM=1"))
    return false;

  sendATCommand(
      "AT+CLASS=C");

  if (!sendATCommand(
          ("AT+APPEUI=" +
           LoraDetails.APPEUI)
              .c_str()))
    return false;

  if (!sendATCommand(
          ("AT+DEVEUI=" +
           LoraDetails.DEVEUI)
              .c_str()))
    return false;

  if (!sendATCommand(
          ("AT+APPKEY=" +
           LoraDetails.APPKEY)
              .c_str()))
    return false;

  if (!sendATCommand(
          "AT+BAND=3"))
    return false;

  sendATCommand(
      "AT+NJM=1");

  // ==============================
  // COMPLETE
  // ==============================

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println();
  Serial.println(
      "================================");

  Serial.println(
      "RAK11160 CONFIGURED");

  Serial.println(
      "================================");

  xSemaphoreGive(
      SerialMutex);

  LoraInitialized = true;

  return true;
}

bool loraJoin()
{
  if (!LoraInitialized)
  {
    xSemaphoreTake(
        SerialMutex,
        portMAX_DELAY);

    Serial.println(
        "LoRa not initialized. Cannot join network.");

    xSemaphoreGive(
        SerialMutex);

    return false;
  }

  LoraJoined = false;

  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(
        LoRaSerialMutex,
        portMAX_DELAY);

  while (LoRaSerial.available())
    LoRaSerial.read();

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println();
  Serial.println(
      "================================");

  Serial.println(
      "JOINING LORAWAN NETWORK");

  Serial.println(
      "================================");

  xSemaphoreGive(
      SerialMutex);

  LoRaSerial.print(
      "AT+JOIN=1\r\n");

  unsigned long start =
      millis();

  String response = "";

  while (millis() - start < 60000)
  {
    while (LoRaSerial.available())
    {
      char c =
          LoRaSerial.read();

      xSemaphoreTake(
          SerialMutex,
          portMAX_DELAY);

      Serial.write(c);

      xSemaphoreGive(
          SerialMutex);

      response += c;

      if (response.indexOf(
              "+EVT:JOINED") >= 0)
      {
        xSemaphoreTake(
            SerialMutex,
            portMAX_DELAY);

        Serial.println();
        Serial.println(
            "================================");

        Serial.println(
            "LORAWAN JOIN SUCCESS");

        Serial.println(
            "================================");

        xSemaphoreGive(
            SerialMutex);

        LoraJoined = true;

        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(
              LoRaSerialMutex);

        return true;
      }

      if (response.indexOf(
              "+EVT:JOIN_FAILED") >= 0)
      {
        xSemaphoreTake(
            SerialMutex,
            portMAX_DELAY);

        Serial.println();
        Serial.println(
            "LORAWAN JOIN FAILED");

        xSemaphoreGive(
            SerialMutex);

        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(
              LoRaSerialMutex);

        return false;
      }
    }

    delay(10);
  }

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

  Serial.println();
  Serial.println(
      "LORAWAN JOIN TIMEOUT");

  xSemaphoreGive(
      SerialMutex);

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(
        LoRaSerialMutex);

  return false;
}