/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Bluetooth communication implementation for Basic configuration
 */

/**********************
 * BLUETOOTH COMMUNICATION IMPLEMENTATION
 * Handles BLE connectivity, data parsing, and device configuration
 * Manages eSIM credentials and system parameters received via Bluetooth
 **********************/

#include "Headerfile.h" // Include main system header

/**********************
 * RETURN RESPONSE FUNCTION
 * Sends success or error response back via BLE
 **********************/
void BLUETOOTHPROCESSFUNCTION::_return()
{
  if (bluetooth.com_data.indexOf(LORA_CONFIG) != -1) // Check if eSIM configuration was successful
  {
    Bluetooth.send(bluetooth.SendSuccess); // Send acknowledgment
    bluetooth.DataReceived = true;
    Serial.println("LoRa configuration successful");
  }
  else // If configuration failed
  {
    Bluetooth.send(bluetooth.SendError); // Send acknowledgment
    Serial.println("Configuration failed");
  }
}

/**********************
 * CSV FIELD SPLITTER
 * Splits `data` on ',' into out[] (each entry trimmed). Returns the field
 * count. Never restarts from the front on a missing separator.
 **********************/
static int _splitCsv(const String &data, String *out, int maxFields)
{
  int count = 0;
  int start = 0;
  while (count < maxFields)
  {
    int comma = data.indexOf(',', start);
    if (comma == -1)
    {
      out[count++] = data.substring(start);
      break;
    }
    out[count++] = data.substring(start, comma);
    start = comma + 1;
  }
  for (int i = 0; i < count; i++)
    out[i].trim();
  return count;
}

// Shared range checks for the numeric config fields.
static bool _validCommon(long capacity, long outlets,
                         long date, long month, long year, long hour, long minute, long second)
{
  if (capacity <= 0)
    return false;
  if (outlets < 1 || outlets > 3)
    return false;
  if (date < 1 || date > 31 || month < 1 || month > 12 || year < 1)
    return false;
  if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
    return false;
  return true;
}

/**********************
 * BLE DATA PARSING FUNCTION
 * Extracts and validates configuration data from BLE messages.
 * Returns true only when the message parsed and every field is in range;
 * on false nothing is committed and com_data is left untouched.
 *********************/
bool BLUETOOTHPROCESSFUNCTION::Process(String data)
{
  data.trim(); // Remove any leading/trailing whitespace

  if (data.startsWith(LORA_CONFIG)) // LoRa configuration
  {
    if (!_messageProcess(data))
    {
      return false;
    }
    return true;
  }
  return false;
}

/**********************
 * MESSAGE PROCESSING FUNCTION (LoRa)
 * Format: lora,appeui,deveui,appkey,capacity,outlets,date,month,year,hour,minute,second
 **********************/
bool BLUETOOTHPROCESSFUNCTION::_messageProcess(String data)
{
  bluetooth.com_data = LORA_CONFIG;
  String f[12];
  int n = _splitCsv(data, f, 12); // f[0] == "lora"

  if (n < 12) // tag + 11 values
  {
    Serial.printf("[BLUETOOTH] LoRa config: expected 12 fields, got %d\n", n);
    return false;
  }
  LoraDetails.APPEUI = f[1];
  LoraDetails.DEVEUI = f[2];
  LoraDetails.APPKEY = f[3];
  long capacity = f[4].toInt();
  long outlets = f[5].toInt();
  long date = f[6].toInt();
  long month = f[7].toInt();
  long year = f[8].toInt();
  long hour = f[9].toInt();
  long minute = f[10].toInt();
  long second = f[11].toInt();

  if (f[1].length() == 0 || f[2].length() == 0 || f[3].length() == 0) // APPEUI/DEVEUI/APPKEY must be present
    return false;
  if (!_validCommon(capacity, outlets, date, month, year, hour, minute, second))
  {
    Serial.println("[BLUETOOTH] LoRa config: field out of range");
    return false;
  }

  GreaseOut.Reservoir.Capacity = capacity;
  Config.NoOfOutlet = outlets;
  RTC.date = date;
  RTC.month = month;
  RTC.year = year;
  RTC.hour = hour;
  RTC.minute = minute;
  RTC.seconds = second;
  return true;
  //*lora,AC1F09FFF8681160,AC1F09FFFE20A015,1CDD3E9AC41D627F3D9D36C1F5E87E01,4000,1,21,9,2026,9,35,0#
}

/**********************
 * SYSTEM RESET FUNCTION
 * Resets system parameters after new configuration is applied
 **********************/
void BLUETOOTHPROCESSFUNCTION::Reset()
{

  RTC.Enable = false;

  /**********************
   * RTC AND LCD INITIALIZATION
   * Configure real-time clock and initialize LCD display
   **********************/
  rtc.setDateTime(RTC.Enable, RTC.date, RTC.month, RTC.year, RTC.hour, RTC.minute, RTC.seconds); // Set RTC date and time

  GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
  GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
  GreaseOut.TestRun.Enable = false;     // Disable test run mode

  SerialFun.WrongPassword = false;         // Reset wrong password flag
  SerialFun.WrongPassword_delay = 1000;    // Reset password delay timer
  SerialFun.WrongPassword_delay_count = 0; // Reset wrong password attempt counter

  Alert.Enable = false;     // Disable alert occurrence
  SetAlertName("NO");       // Reset alert name
  Alert.OverloadValue = 0;  // Reset overload value
  Alert.ReservoirValue = 0; // Reset reservoir level alert value
  Alert.NameCount = 0;      // Reset alert name counter
  Alert.Total.enable = true;
}

/**********************
 * BLUETOOTH FUNCTION
 * Manages BLE data reception and processing
 **********************/
void BLUETOOTHPROCESSFUNCTION::Function()
{
  String message = Bluetooth.read(); // Read data from client
  if (message == "")                 // Nothing to do
    return;

  Serial.println("=====================================");
  Serial.printf("[BLUETOOTH] Received data: %s\n", message.c_str()); // Debug message
  Serial.println("=====================================");

  if (BluetoothFun.Process(message)) // Parse + validate; commits only on success
  {
    Eeprom.write(); // Persist the new configuration
    BluetoothFun._return();
    bluetooth.Received = 1; // Set Bluetooth data received flag
    BluetoothFun.Reset();   // Reset system values
  }
  else
  {
    Bluetooth.send(bluetooth.SendError); // Reject - nothing saved, no restart
    Serial.println("[BLUETOOTH] Malformed config rejected");
  }
}

void BLUETOOTHPROCESSFUNCTION::Process()
{
  BluetoothFun.Function();    // Manage Bluetooth operations
  LcdUI.Bluetooth.Function(); // Update LCD UI for Bluetooth status

  if (bluetooth.Received == 1) // If Bluetooth data received
  {
    bluetooth.IncomingDataReceived = true; // Set data received flag
    bluetooth.IncomingDataReceive = false; // Clear receiving flag
    _Controller_online_data_send = false;
    bluetooth.Received = 2; // Set data processed flag
    Led.off();              // Turn off LED indicator
    LoraTime.EnableForOnline = true;
    bluetooth.ModeEnable = false; // Disable Bluetooth mode
    LcdUI.Bluetooth.Function();   // Update LCD UI for Bluetooth status
    Bluetooth.disconnect();       // Disconnect Bluetooth after processing
    Eeprom.write();               // Save configuration to EEPROM
    delay(5000);                  // Short delay to ensure disconnection
    ESP.restart();                // Restart system to apply new configuration
  }
}