/**
 * @file LoraFunction.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 2.0.0 - Robust TX/RX rewrite
 * @date 2025-10-01
 *
 * @note LoRa COMMUNICATION IMPLEMENTATION - Fixed RX starvation & flush bug
 * RAK11160 is Class C, downlinks can arrive at ANY time, even while we are
 * waiting for TX_DONE. The old code did `while(available) read()` before every
 * AT+SEND, which discarded pending downlinks. This rewrite never flushes.
 * RX task is the normal reader, but TX path also dispatches +EVT:RX_ lines
 * while it owns the UART mutex.
 */

#include "HeaderFile.h"

#define LORA_RX_PIN 16
#define LORA_TX_PIN 15

HardwareSerial LoRaSerial(1);

#define LORA_BAUD 115200

bool LoraInitialized = false;
bool LoraJoined = false;

// RAK11160 UART mutex - protects LoRaSerial
SemaphoreHandle_t LoRaSerialMutex = NULL;

// Forward
void processLoRaLine(String line);

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
  if (clean.length() % 2 != 0) return ""; // invalid

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
// Send AT command and wait for OK
// This is the ONLY place that talks to LoRaSerial for AT commands
// It NEVER flushes pending data - it reads line by line and
// dispatches any +EVT:RX_ that appears in between.
// ==================================================
bool sendATCommand(const char *command, uint32_t timeout = 5000)
{
  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(LoRaSerialMutex, portMAX_DELAY);

  // Send command
  LoRaSerial.print(command);
  LoRaSerial.print("\r\n");

  String responseAccum = "";
  responseAccum.reserve(256);
  unsigned long start = millis();
  bool gotOk = false;
  bool gotError = false;

  while (millis() - start < timeout)
  {
    while (LoRaSerial.available())
    {
      String line = LoRaSerial.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) continue;

      // Debug
      xSemaphoreTake(SerialMutex, portMAX_DELAY);
      Serial.print("[LORA AT] ");
      Serial.println(line);
      xSemaphoreGive(SerialMutex);

      // Always dispatch downlink if it sneaks in
      if (loraTryDispatchDownlink(line))
        continue;

      responseAccum += line + "\n";

      if (line.indexOf("AT_PARAM_ERROR") >= 0 ||
          line.indexOf("AT_BUSY_ERROR") >= 0 ||
          line.indexOf("AT_NO_NETWORK_JOINED") >= 0 ||
          line == "ERROR" || line.startsWith("ERROR"))
      {
        gotError = true;
        break;
      }
      if (line == "OK")
      {
        gotOk = true;
        break;
      }
      // Some RAK firmwares echo command, ignore
    }
    if (gotOk || gotError) break;
    vTaskDelay(pdMS_TO_TICKS(5));
  }

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);

  if (gotError)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("AT CMD ERROR: ");
    Serial.println(command);
    xSemaphoreGive(SerialMutex);
    return false;
  }
  if (gotOk)
  {
    return true;
  }

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.print("AT CMD TIMEOUT: ");
  Serial.println(command);
  xSemaphoreGive(SerialMutex);
  return false;
}

// --------------------------------------------------
// Send string through RAK11160 - ROBUST VERSION
// --------------------------------------------------
bool sendString(String message, int port)
{
  if (!LoraJoined)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("LoRa not joined, cannot send");
    xSemaphoreGive(SerialMutex);
    return false;
  }

  if (message.length() > 100)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("LoRa payload too large: ");
    Serial.println(message.length());
    xSemaphoreGive(SerialMutex);
    return false;
  }

  // Small gap to avoid spamming
  vTaskDelay(pdMS_TO_TICKS(100));

  String hexData = stringToHex(message);
  String command = "AT+SEND=" + String(port) + ":" + hexData;

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.print("UPLINK String: ");
  Serial.println(message);
  Serial.print("UPLINK HEX: ");
  Serial.println(hexData);
  xSemaphoreGive(SerialMutex);

  // --- Own UART ---
  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(LoRaSerialMutex, portMAX_DELAY);

  LoRaSerial.print(command);
  LoRaSerial.print("\r\n");

  bool sendAccepted = false;
  bool txDone = false;
  bool txFailed = false;

  unsigned long start = millis();

  while (millis() - start < 15000)
  {
    while (LoRaSerial.available())
    {
      String line = LoRaSerial.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) continue;

      xSemaphoreTake(SerialMutex, portMAX_DELAY);
      Serial.print("[LORA TX] ");
      Serial.println(line);
      xSemaphoreGive(SerialMutex);

      if (loraTryDispatchDownlink(line))
        continue;

      if (line == "OK")
      {
        sendAccepted = true;
        continue;
      }
      if (line.indexOf("+EVT:TX_DONE") >= 0 ||
          line.indexOf("+EVT:SEND_CONFIRMED_OK") >= 0)
      {
        txDone = true;
      }
      if (line.indexOf("+EVT:SEND_CONFIRMED_FAILED") >= 0 ||
          line.indexOf("AT_PARAM_ERROR") >= 0 ||
          line.indexOf("AT_BUSY_ERROR") >= 0 ||
          line.indexOf("AT_NO_NETWORK_JOINED") >= 0 ||
          line == "ERROR")
      {
        txFailed = true;
      }

      if (sendAccepted && txDone)
      {
        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);
        return true;
      }
      if (txFailed)
      {
        xSemaphoreTake(SerialMutex, portMAX_DELAY);
        Serial.println("LoRa SEND FAILED event");
        xSemaphoreGive(SerialMutex);
        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);
        return false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("LoRa TX RESPONSE TIMEOUT");
  Serial.print("  sendAccepted=");
  Serial.print(sendAccepted);
  Serial.print(" txDone=");
  Serial.println(txDone);
  xSemaphoreGive(SerialMutex);

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);

  // If we got OK but no TX_DONE within 15s, still consider it sent
  // because network may still deliver (Class C). Return sendAccepted.
  return sendAccepted;
}

// --------------------------------------------------
// Parse RAK11160 downlink event
// Example: +EVT:RX_1:-48:7:UNICAST:2:48656C6C6F
// --------------------------------------------------
void processLoRaLine(String line)
{
  line.trim();
  if (line.length() == 0) return;
  if (!line.startsWith("+EVT:RX_")) return;

  int lastColon = line.lastIndexOf(':');
  if (lastColon < 0)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("Invalid RX event, no colon");
    xSemaphoreGive(SerialMutex);
    return;
  }

  String hexData = line.substring(lastColon + 1);
  hexData.trim();
  if (hexData.length() == 0) return;

  String message = hexToString(hexData);
  if (message.length() == 0)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("HEX decode failed: ");
    Serial.println(hexData);
    xSemaphoreGive(SerialMutex);
    return;
  }

  // Message format from Python bridge: <ClientID>/<TOPIC>:<PAYLOAD>
  // Example: 901745858428/ACK_DASHBOARD:OK  or 901745858428/MODE:*RM-ST#
  String topic = "";
  String payload = "";

  int separator = message.indexOf(':');
  if (separator > 0)
  {
    topic = message.substring(0, separator);
    payload = message.substring(separator + 1);
  }
  else
  {
    // No colon, treat whole as topic (some old firmwares)
    topic = message;
    payload = "";
  }

  topic.trim();
  payload.trim();

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println();
  Serial.println("========================================");
  Serial.println("DOWNLINK RECEIVED");
  Serial.println("========================================");
  Serial.print("RAW LINE : "); Serial.println(line);
  Serial.print("HEX      : "); Serial.println(hexData);
  Serial.print("STRING   : "); Serial.println(message);
  Serial.print("TOPIC    : "); Serial.println(topic);
  Serial.print("PAYLOAD  : "); Serial.println(payload);
  Serial.println("========================================");
  xSemaphoreGive(SerialMutex);

  if (topic.length() > 0)
  {
    _receive_message_process(topic, payload);
  }
}

void loraReceiveProcess()
{
  if (!LoraInitialized) return;
  if (!LoraJoined) return;

  // Try to take mutex quickly, if TX task owns it, skip this cycle
  // TX task itself will dispatch RX lines while it owns mutex
  if (LoRaSerialMutex != NULL)
  {
    if (xSemaphoreTake(LoRaSerialMutex, pdMS_TO_TICKS(5)) != pdTRUE)
    {
      return;
    }
  }

  // Drain all available lines
  while (LoRaSerial.available())
  {
    String line = LoRaSerial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;

    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[LORA RX] ");
    Serial.println(line);
    xSemaphoreGive(SerialMutex);

    // Only dispatch if it's a downlink, other EVT like TX_DONE may appear
    // here if TX task missed it (should not), but we handle anyway
    if (line.startsWith("+EVT:RX_"))
    {
      processLoRaLine(line);
    }
    else if (line.indexOf("+EVT:JOINED") >= 0)
    {
      // Late JOINED (should be caught in loraJoin, but handle)
      LoraJoined = true;
    }
  }

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);
}

bool LoRaFunction::publish(const String &_topic, const String &_data)
{
  String data = _topic + ":" + _data;
  return sendString(data, 2);
}

bool LoRaFunction::response(const String &_topic, const String &_data)
{
  String data = _topic + ":" + _data;
  return sendString(data, 2);
}

// ==================================================
// LoRa initialization - robust
// ==================================================
bool loraBegin()
{
  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println();
  Serial.println("========================================");
  Serial.println("Initializing RAK11160...");
  Serial.println("========================================");
  xSemaphoreGive(SerialMutex);

  // UART with larger RX buffer to avoid overflow of long downlinks
  LoRaSerial.setRxBufferSize(1024);
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  delay(3000); // Give RAK time to boot

  // Clear any boot garbage without discarding in a way that loses future data
  // Just read and print
  if (LoRaSerialMutex == NULL)
  {
    // Should already be created in Task::create, but safety
    LoRaSerialMutex = xSemaphoreCreateMutex();
  }

  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(LoRaSerialMutex, portMAX_DELAY);

  unsigned long clearStart = millis();
  while (millis() - clearStart < 500)
  {
    while (LoRaSerial.available())
    {
      char c = LoRaSerial.read();
      xSemaphoreTake(SerialMutex, portMAX_DELAY);
      Serial.write(c);
      xSemaphoreGive(SerialMutex);
    }
    delay(10);
  }

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);

  // RAK READY check
  bool rakReady = false;
  for (int attempt = 1; attempt <= 5; attempt++)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("AT attempt ");
    Serial.print(attempt);
    Serial.println("...");
    xSemaphoreGive(SerialMutex);

    if (sendATCommand("AT", 2000))
    {
      rakReady = true;
      break;
    }
    delay(1000);
  }

  if (!rakReady)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("ERROR: RAK11160 not responding!");
    xSemaphoreGive(SerialMutex);
    return false;
  }

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("RAK11160 Ready!");
  xSemaphoreGive(SerialMutex);

  // CONFIGURATION - each step must succeed except CLASS which is optional
  if (!sendATCommand("AT+NWM=1")) return false;
  sendATCommand("AT+CLASS=C"); // Don't fail if this fails, but try

  if (!sendATCommand(("AT+APPEUI=" + LoraDetails.APPEUI).c_str())) return false;
  if (!sendATCommand(("AT+DEVEUI=" + LoraDetails.DEVEUI).c_str())) return false;
  if (!sendATCommand(("AT+APPKEY=" + LoraDetails.APPKEY).c_str())) return false;
  if (!sendATCommand("AT+BAND=3")) return false;
  sendATCommand("AT+NJM=1");

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("========================================");
  Serial.println("RAK11160 CONFIGURED");
  Serial.println("========================================");
  xSemaphoreGive(SerialMutex);

  LoraInitialized = true;
  return true;
}

bool loraJoin()
{
  if (!LoraInitialized)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.println("LoRa not initialized, cannot join");
    xSemaphoreGive(SerialMutex);
    return false;
  }

  LoraJoined = false;

  if (LoRaSerialMutex != NULL)
    xSemaphoreTake(LoRaSerialMutex, portMAX_DELAY);

  // Don't blindly flush, but ensure we start clean for JOIN
  // We will read everything and dispatch any RX (should be none)
  while (LoRaSerial.available())
  {
    String line = LoRaSerial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;
    xSemaphoreTake(SerialMutex, portMAX_DELAY);
    Serial.print("[LORA JOIN CLEAN] ");
    Serial.println(line);
    xSemaphoreGive(SerialMutex);
    if (line.startsWith("+EVT:RX_"))
    {
      // Should not happen before join, but dispatch if it does
      processLoRaLine(line);
    }
  }

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println();
  Serial.println("========================================");
  Serial.println("JOINING LORAWAN NETWORK");
  Serial.println("========================================");
  xSemaphoreGive(SerialMutex);

  LoRaSerial.print("AT+JOIN=1\r\n");

  unsigned long start = millis();
  String response = "";

  while (millis() - start < 60000)
  {
    while (LoRaSerial.available())
    {
      String line = LoRaSerial.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) continue;

      xSemaphoreTake(SerialMutex, portMAX_DELAY);
      Serial.print("[LORA JOIN] ");
      Serial.println(line);
      xSemaphoreGive(SerialMutex);

      if (line.startsWith("+EVT:RX_"))
      {
        processLoRaLine(line);
        continue;
      }

      if (line.indexOf("+EVT:JOINED") >= 0)
      {
        xSemaphoreTake(SerialMutex, portMAX_DELAY);
        Serial.println("========================================");
        Serial.println("LORAWAN JOIN SUCCESS");
        Serial.println("========================================");
        xSemaphoreGive(SerialMutex);
        LoraJoined = true;
        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);
        return true;
      }
      if (line.indexOf("+EVT:JOIN_FAILED") >= 0)
      {
        xSemaphoreTake(SerialMutex, portMAX_DELAY);
        Serial.println("LORAWAN JOIN FAILED");
        xSemaphoreGive(SerialMutex);
        if (LoRaSerialMutex != NULL)
          xSemaphoreGive(LoRaSerialMutex);
        return false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  xSemaphoreTake(SerialMutex, portMAX_DELAY);
  Serial.println("LORAWAN JOIN TIMEOUT");
  xSemaphoreGive(SerialMutex);

  if (LoRaSerialMutex != NULL)
    xSemaphoreGive(LoRaSerialMutex);

  return false;
}
