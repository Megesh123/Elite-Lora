/**
 * @file LoraFunction.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LoRa COMMUNICATION IMPLEMENTATION
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

bool LoraInitialized = false;
bool LoraJoined = false;

// RAK11160 UART mutex
SemaphoreHandle_t LoRaSerialMutex = NULL;

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
// Convert ASCII string to HEX
// Example: "Hello" -> "48656C6C6F"
// --------------------------------------------------
String stringToHex(String text)
{
  String hex = "";

  for (size_t i = 0; i < text.length(); i++)
  {
    char buf[3];

    sprintf(
        buf,
        "%02X",
        (uint8_t)text[i]);

    hex += buf;
  }

  return hex;
}
// --------------------------------------------------
// Convert HEX string to ASCII
// Example: "48656C6C6F" -> "Hello"
// --------------------------------------------------
String hexToString(String hex)
{
  String result = "";

  for (int i = 0; i < hex.length(); i += 2)
  {
    if (i + 1 >= hex.length())
      break;

    String byteString = hex.substring(i, i + 2);

    char c = (char)strtol(
        byteString.c_str(),
        NULL,
        16);

    result += c;
  }

  return result;
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

  if (message.length() > 100)
  {
    xSemaphoreTake(SerialMutex, portMAX_DELAY);

    Serial.print(
        "sendString - ERROR: LoRa payload too large: ");

    Serial.print(message.length());

    Serial.println(" bytes");

    xSemaphoreGive(SerialMutex);

    return false;
  }

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

  // =================================
  // SERIAL MONITOR
  // =================================

  xSemaphoreTake(
      SerialMutex,
      portMAX_DELAY);

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