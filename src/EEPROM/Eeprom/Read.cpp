/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note EEPROM emulation using flash memory for non-volatile data storage.
 */

#include "Headerfile.h"  // Include main system header
#include "FlashWrite1.h" // Include flash write utility

void printTotalBytes()
{
  size_t totalBytes = 0;

// Helper lambda for primitives
#define ADD_BYTES(var) totalBytes += sizeof(var)

// For Strings: stores characters + null terminator ('\0') or length byte
#define ADD_STR_BYTES(str) totalBytes += (str.length() + 1)

  // 1. Mode Flags
  ADD_BYTES(GreaseOut.Lubrication.Enable);
  ADD_BYTES(GreaseOut.PauseTime.Enable);
  ADD_BYTES(GreaseOut.TestRun.Enable);
  ADD_BYTES(GreaseOut.Reservoir.Enable);

  // 2. Temporary Parameters
  ADD_BYTES(GreaseOut.Lubrication.Temp.Grams);
  ADD_BYTES(GreaseOut.PauseTime.Temp.Hour);
  ADD_BYTES(GreaseOut.PauseTime.Temp.Minute);
  ADD_BYTES(GreaseOut.PauseTime.Seconds);
  ADD_BYTES(GreaseOut.Reservoir.Temp.Percentage);

  // 3. Sensor Readings
  ADD_BYTES(CurrentMonitor.measured);
  ADD_BYTES(VoltageMonitor.measured);

  // 4. Total Operation Time
  ADD_BYTES(TotalOperationTime.Hour);
  ADD_BYTES(TotalOperationTime.Minute);
  ADD_BYTES(TotalOperationTime.Seconds);

  // 5. Alert Parameters
  ADD_BYTES(Alert.Enable);
  ADD_BYTES(Alert.OverloadValue);
  ADD_BYTES(Alert.ReservoirValue);
  ADD_BYTES(Alert.NameCount);
  ADD_BYTES(Alert.SnoozeReservoir);
  totalBytes += (strlen(Alert.Name) + 1); // Alert.Name is a fixed char buffer

  // 6. Totals
  ADD_BYTES(GreaseOut.Lubrication.Total.Grams);
  ADD_BYTES(GreaseOut.PauseTime.Total.Hour);
  ADD_BYTES(GreaseOut.PauseTime.Total.Minute);

  // 7. Configuration Parameters
  ADD_BYTES(GreaseOut.Sleep.Enable);
  ADD_BYTES(flash.Write);
  ADD_BYTES(GreaseOut.Lubrication.Grams);
  ADD_BYTES(GreaseOut.PauseTime.Hour);
  ADD_BYTES(GreaseOut.PauseTime.Minute);
  ADD_BYTES(Config.NoOfOutlet);
  ADD_BYTES(GreaseOut.Reservoir.Capacity);

  // 8. System Flags
  ADD_BYTES(flash.DataReceived);
  ADD_BYTES(flash.OnceWrite);

  // 9. Flash Addresses & Dates
  ADD_STR_BYTES(flash.Temp.currentAddr);
  ADD_STR_BYTES(flash.Temp.readAddr);
  ADD_STR_BYTES(flash.LastDate);
  ADD_STR_BYTES(flash.PerverseDate);

  // 10. RTC Parameters
  ADD_BYTES(bluetooth.Received);
  ADD_BYTES(RTC.Enable);
  ADD_BYTES(RTC.year);
  ADD_BYTES(RTC.month);
  ADD_BYTES(RTC.date);
  ADD_BYTES(RTC.hour);
  ADD_BYTES(RTC.minute);
  ADD_BYTES(RTC.seconds);

  // 11. LoRa Details
  ADD_STR_BYTES(LoraDetails.APPEUI);
  ADD_STR_BYTES(LoraDetails.Clientid);
  ADD_STR_BYTES(LoraDetails.DEVEUI);
  ADD_STR_BYTES(LoraDetails.APPKEY);
  ADD_BYTES(LoraDetails.port);
  ADD_STR_BYTES(LoraDetails.Wifi);
  ADD_STR_BYTES(LoraDetails.Pass);

  // 12. Bluetooth Data
  ADD_STR_BYTES(bluetooth.com_data);

  // 13. Capacities and Live status
  ADD_BYTES(capcity);
  ADD_BYTES(TankCapcity);
  ADD_BYTES(GreaseOut.Reservoir.Percentage);
  ADD_BYTES(GreaseOut.Lubrication.Live.Grams);
  ADD_BYTES(GreaseOut.PauseTime.Live.Hour);
  ADD_BYTES(GreaseOut.PauseTime.Live.Minute);

  // 14. Status Flags
  ADD_BYTES(start_init);
  ADD_BYTES(bluetooth.DataReceived);
  ADD_BYTES(LoraFun.DataReceived);
  ADD_BYTES(Reset_SendData_Lora);

  // 15. Supervisory & Pump Conditions
  ADD_BYTES(SupervisoryTime.SupervisoryAverageTakeOutGrease_1);
  ADD_BYTES(GreaseOut.Lubrication.Temp.GramSupervisory_1);
  ADD_BYTES(SupervisoryTime.AlertCheckOutGrease);

  ADD_STR_BYTES(_pump1_condtion);
  ADD_STR_BYTES(_pump2_condtion);
  ADD_STR_BYTES(_pump3_condtion);
  ADD_BYTES(_pump1_percentage);
  ADD_BYTES(_pump2_percentage);
  ADD_BYTES(_pump3_percentage);

  // 16. Test Run & Alerts
  ADD_BYTES(GreaseOut.TestRun.min);
  ADD_BYTES(GreaseOut.TestRun.sec);
  ADD_BYTES(Alert.Total.NoOfAlert);
  ADD_BYTES(Alert.Total.Reservoir);
  ADD_BYTES(Alert.Total.OverLoad);
  ADD_BYTES(Alert.Total.Supervisory1);
  ADD_BYTES(Alert.Total.Supervisory2);
  ADD_BYTES(Alert.Total.Supervisory3);
  ADD_BYTES(Alert.Total.SignalNotDetected);
  ADD_BYTES(Alert.Total.PumpWeak1);
  ADD_BYTES(Alert.Total.PumpWeak2);
  ADD_BYTES(Alert.Total.PumpWeak3);

  ADD_BYTES(Alert.EnableDisable.Supervisory1);
  ADD_BYTES(Alert.EnableDisable.Supervisory2);
  ADD_BYTES(Alert.EnableDisable.Supervisory3);
  ADD_BYTES(Alert.EnableDisable.RefillAlert);

  ADD_BYTES(Lora_disconnected_count);

  // 17. Supervisory Counts & Limits
  ADD_BYTES(SupervisoryTime.AverageEnable_Supervisory1);
  ADD_BYTES(SupervisoryTime.AverageEnable_Supervisory2);
  ADD_BYTES(SupervisoryTime.AverageEnable_Supervisory3);

  ADD_BYTES(SupervisoryTime.NWC_SUP1);
  ADD_BYTES(SupervisoryTime.AWC_SUP1);
  ADD_BYTES(SupervisoryTime.Supervisory1_Count);

  ADD_BYTES(SupervisoryTime.NWC_SUP2);
  ADD_BYTES(SupervisoryTime.AWC_SUP2);
  ADD_BYTES(SupervisoryTime.Supervisory2_Count);

  ADD_BYTES(SupervisoryTime.NWC_SUP3);
  ADD_BYTES(SupervisoryTime.AWC_SUP3);
  ADD_BYTES(SupervisoryTime.Supervisory3_Count);

  ADD_BYTES(SupervisoryTime.ELH_OL1);
  ADD_BYTES(SupervisoryTime.ELH_OL2);
  ADD_BYTES(SupervisoryTime.ELH_OL3);

  // 18. Board State & OTA
  ADD_BYTES(GreaseOut.Reservoir.Temp._old_reservoir_value);
  ADD_BYTES(Grease.TotalProxityCount);
  ADD_BYTES(_Controller_online_data_send);
  ADD_BYTES(oldStateCount);
  ADD_BYTES(AlreadyTestedBoard);
  ADD_BYTES(ReconfigEnableRefillNeed);
  ADD_BYTES(bluetooth.ModeEnable);
  ADD_BYTES(ChechOTAUpdate);
  ADD_STR_BYTES(OTA.current_version);

  // 19. LoRa TWP & Supervisory Final
  ADD_BYTES(Lora_twp.RefillAlertEnableDisable);
  ADD_BYTES(Lora_twp.SleepModeOff);
  ADD_BYTES(Lora_twp.SleepModeOn);
  ADD_BYTES(Lora_twp.SupervisoryAlertEnableDisable);
  ADD_BYTES(Lora_twp.TestRunOff);
  ADD_BYTES(Lora_twp.TestRunOn);
  ADD_BYTES(Lora_twp.AlertClear);
  ADD_BYTES(Lora_twp.RefillSuccessful);
  ADD_STR_BYTES(Lora_twp.RefillSuccessful_Data);
  ADD_STR_BYTES(Lora_twp.AlertClear_Data);

  ADD_BYTES(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1);
  ADD_BYTES(SupervisoryTime.SupervisoryNominalTakeOutGrease_1);
  ADD_BYTES(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2);
  ADD_BYTES(SupervisoryTime.SupervisoryNominalTakeOutGrease_2);
  ADD_BYTES(SupervisoryTime.SupervisoryAverageTakeOutGrease_2);
  ADD_BYTES(GreaseOut.Lubrication.Temp.GramSupervisory_2);

  ADD_BYTES(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3);
  ADD_BYTES(SupervisoryTime.SupervisoryNominalTakeOutGrease_3);
  ADD_BYTES(SupervisoryTime.SupervisoryAverageTakeOutGrease_3);
  ADD_BYTES(GreaseOut.Lubrication.Temp.GramSupervisory_3);

#undef ADD_BYTES
#undef ADD_STR_BYTES

  Serial.println("=====================================");
  Serial.print("Total Flash Memory Used: ");
  Serial.print(totalBytes);
  Serial.println(" Bytes");
  Serial.println("=====================================");
}

/**********************
 * EEPROM Read Method
 * Read all system configuration and operational data from flash memory
 **********************/
void EEPROMFUNCTION::read()
{
  /**********************
   * MODE FLAGS RETRIEVAL
   * Read operational mode enable/disable flags
   **********************/
  Flash1.read(GreaseOut.Lubrication.Enable); // Read lubrication mode flag
  Flash1.read(GreaseOut.PauseTime.Enable);   // Read pause time mode flag
  Flash1.read(GreaseOut.TestRun.Enable);     // Read test run mode flag
  Flash1.read(GreaseOut.Reservoir.Enable);   // Read refill mode flag

  /**********************
   * TEMPORARY PARAMETERS RETRIEVAL
   * Read temporary calculation variables
   **********************/
  Flash1.read(GreaseOut.Lubrication.Temp.Grams);    // Read temporary grease quantity
  Flash1.read(GreaseOut.PauseTime.Temp.Hour);       // Read temporary pause hours
  Flash1.read(GreaseOut.PauseTime.Temp.Minute);     // Read temporary pause minutes
  Flash1.read(GreaseOut.PauseTime.Seconds);         // Read pause time seconds
  Flash1.read(GreaseOut.Reservoir.Temp.Percentage); // Read temporary refill percentage

  /**********************
   * SENSOR READINGS RETRIEVAL
   * Read current and voltage sensor measurements
   **********************/
  Flash1.read(CurrentMonitor.measured); // Read current reading
  Flash1.read(VoltageMonitor.measured); // Read voltage reading

  /**********************
   * TOTAL OPERATION TIME RETRIEVAL
   * Read total system runtime counters
   **********************/
  Flash1.read(TotalOperationTime.Hour);    // Read total run hours
  Flash1.read(TotalOperationTime.Minute);  // Read total run minutes
  Flash1.read(TotalOperationTime.Seconds); // Read total run seconds

  /**********************
   * ALERT PARAMETERS RETRIEVAL
   * Read alert configuration and status
   **********************/
  // Flash1.read(SerialFun.WrongPassword);             // Read wrong password flag
  // Flash1.read(SerialFun.WrongPassword_delay_count); // Read wrong password count
  // Flash1.read(SerialFun.WrongPassword_delay);       // Read wrong password delay
  Flash1.read(Alert.Enable);          // Read alert enable flag
  Flash1.read(Alert.OverloadValue);   // Read overload value
  Flash1.read(Alert.ReservoirValue);  // Read reservoir level value
  Flash1.read(Alert.NameCount);       // Read alert count
  Flash1.read(Alert.SnoozeReservoir); // Read reservoir snooze state
  String _an;
  Flash1.readString(_an); // Read alert name
  strlcpy(Alert.Name, _an.c_str(), sizeof(Alert.Name));

  /**********************
   * TOTALS RETRIEVAL
   * Read cumulative operational totals
   **********************/
  Flash1.read(GreaseOut.Lubrication.Total.Grams); // Read total grease dispensed
  Flash1.read(GreaseOut.PauseTime.Total.Hour);    // Read total pause hours
  Flash1.read(GreaseOut.PauseTime.Total.Minute);  // Read total pause minutes

  /**********************
   * CONFIGURATION PARAMETERS RETRIEVAL
   * Read system configuration settings
   **********************/
  Flash1.read(GreaseOut.Sleep.Enable);       // Read sleep mode flag
  Flash1.read(flash.Write);                  // Read flash write flag
  Flash1.read(GreaseOut.Lubrication.Grams);  // Read grease quantity
  Flash1.read(GreaseOut.PauseTime.Hour);     // Read pause hours
  Flash1.read(GreaseOut.PauseTime.Minute);   // Read pause minutes
  Flash1.read(Config.NoOfOutlet);            // Read outlet count
  Flash1.read(GreaseOut.Reservoir.Capacity); // Read reservoir capacity
  /**********************
   * SYSTEM FLAGS AND DELAYS RETRIEVAL
   * Read various system status flags and delay settings
   **********************/
  Flash1.read(flash.DataReceived); // Read flash data confirm flag
  Flash1.read(flash.OnceWrite);    // Read once write flag

  /**********************
   * FLASH ADDRESS RETRIEVAL
   * Read stored flash memory addresses
   **********************/
  Flash1.readString(flash.Temp.currentAddr);                             // Read flash address string
  Flash1.readString(flash.Temp.readAddr);                                // Read read address string
  flash.currentAddr = strtoul(flash.Temp.currentAddr.c_str(), NULL, 16); // Convert to integer
  flash.readAddr = strtoul(flash.Temp.readAddr.c_str(), NULL, 16);       // Convert to integer

  /**********************
   * DATE PARAMETERS RETRIEVAL
   * Read stored date information
   **********************/
  Flash1.readString(flash.LastDate);     // Read last date
  Flash1.readString(flash.PerverseDate); // Read previous date

  /**********************
   * RTC PARAMETERS RETRIEVAL
   * Read real-time clock configuration and time data
   **********************/
  Flash1.read(bluetooth.Received); // Read Bluetooth data received flag
  Flash1.read(RTC.Enable);         // Read RTC initialized flag
  Flash1.read(RTC.year);           // Read year
  Flash1.read(RTC.month);          // Read month
  Flash1.read(RTC.date);           // Read date
  Flash1.read(RTC.hour);           // Read hour
  Flash1.read(RTC.minute);         // Read minute

  /**********************
   * LORA DETAILS RETRIEVAL
   * Read LoRaWAN provisioning parameters
   **********************/
  Flash1.readString(LoraDetails.APPEUI);   // Read LoRaWAN App EUI
  Flash1.readString(LoraDetails.Clientid); // Read client ID
  Flash1.readString(LoraDetails.DEVEUI);   // Read LoRaWAN Device EUI
  Flash1.readString(LoraDetails.APPKEY);   // Read LoRaWAN App Key
  Flash1.read(LoraDetails.port);           // Read network port
  Flash1.readString(LoraDetails.Wifi);     // Read WiFi SSID
  Flash1.readString(LoraDetails.Pass);     // Read WiFi password

  /**********************
   * BLUETOOTH DATA RETRIEVAL
   * Read Bluetooth communication data
   **********************/
  Flash1.readString(bluetooth.com_data); // Read Bluetooth communication data

  Flash1.read(capcity);
  Flash1.read(TankCapcity);
  Flash1.read(GreaseOut.Reservoir.Percentage);

  Flash1.read(GreaseOut.Lubrication.Live.Grams);
  Flash1.read(GreaseOut.PauseTime.Live.Hour);
  Flash1.read(GreaseOut.PauseTime.Live.Minute);

  Flash1.read(start_init);
  Flash1.read(bluetooth.DataReceived);
  Flash1.read(LoraFun.DataReceived);

  Flash1.read(Reset_SendData_Lora); // Read reset send data via LoRa flag

  Flash1.read(SupervisoryTime.SupervisoryAverageTakeOutGrease_1); // Read supervisory average grease taken out
  Flash1.read(GreaseOut.Lubrication.Temp.GramSupervisory_1);      // Read supervisory grease output
  Flash1.read(SupervisoryTime.AlertCheckOutGrease);               // Read supervisory alert check grease

  Flash1.readString(_pump1_condtion); // read pump 1 condition
  Flash1.readString(_pump2_condtion); // read pump 2 condition
  Flash1.readString(_pump3_condtion); // read pump 3 condition
  Flash1.read(_pump1_percentage);     // read pump 1 percentage
  Flash1.read(_pump2_percentage);     // read pump 2 percentage
  Flash1.read(_pump3_percentage);     // read pump 3 percentage

  Flash1.read(GreaseOut.TestRun.min); //
  Flash1.read(GreaseOut.TestRun.sec); //

  Flash1.read(Alert.Total.NoOfAlert);
  Flash1.read(Alert.Total.Reservoir);
  Flash1.read(Alert.Total.OverLoad);
  Flash1.read(Alert.Total.Supervisory1);
  Flash1.read(Alert.Total.Supervisory2);
  Flash1.read(Alert.Total.Supervisory3);
  Flash1.read(Alert.Total.SignalNotDetected);
  Flash1.read(Alert.Total.PumpWeak1);
  Flash1.read(Alert.Total.PumpWeak2);
  Flash1.read(Alert.Total.PumpWeak3);

  Flash1.read(Alert.EnableDisable.Supervisory1);
  Flash1.read(Alert.EnableDisable.Supervisory2);
  Flash1.read(Alert.EnableDisable.Supervisory3);
  Flash1.read(Alert.EnableDisable.RefillAlert);

  Flash1.read(Lora_disconnected_count);

  Flash1.read(SupervisoryTime.AverageEnable_Supervisory1);
  Flash1.read(SupervisoryTime.AverageEnable_Supervisory2);
  Flash1.read(SupervisoryTime.AverageEnable_Supervisory3);

  Flash1.read(SupervisoryTime.NWC_SUP1);
  Flash1.read(SupervisoryTime.AWC_SUP1);
  Flash1.read(SupervisoryTime.Supervisory1_Count);

  Flash1.read(SupervisoryTime.NWC_SUP2);
  Flash1.read(SupervisoryTime.AWC_SUP2);
  Flash1.read(SupervisoryTime.Supervisory2_Count);

  Flash1.read(SupervisoryTime.NWC_SUP3);
  Flash1.read(SupervisoryTime.AWC_SUP3);
  Flash1.read(SupervisoryTime.Supervisory3_Count);

  Flash1.read(SupervisoryTime.ELH_OL1);
  Flash1.read(SupervisoryTime.ELH_OL2);
  Flash1.read(SupervisoryTime.ELH_OL3);

  Flash1.read(GreaseOut.Reservoir.Temp._old_reservoir_value); // Read old reservoir value for change detection
  Flash1.read(Grease.TotalProxityCount);
  Flash1.read(_Controller_online_data_send);
  Flash1.read(oldStateCount);
  Flash1.read(AlreadyTestedBoard);
  Flash1.read(ReconfigEnableRefillNeed);
  Flash1.read(bluetooth.ModeEnable); // Read Bluetooth mode enable flag
  Flash1.read(ChechOTAUpdate);
  Flash1.readString(OTA.current_version); // Read OTA update version string

  Flash1.read(Lora_twp.RefillAlertEnableDisable);
  Flash1.read(Lora_twp.SleepModeOff);
  Flash1.read(Lora_twp.SleepModeOn);
  Flash1.read(Lora_twp.SupervisoryAlertEnableDisable);
  Flash1.read(Lora_twp.TestRunOff);
  Flash1.read(Lora_twp.TestRunOn);
  Flash1.read(Lora_twp.AlertClear);
  Flash1.read(Lora_twp.RefillSuccessful);
  Flash1.readString(Lora_twp.RefillSuccessful_Data);
  Flash1.readString(Lora_twp.AlertClear_Data);
  Flash1.read(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1);
  Flash1.read(SupervisoryTime.SupervisoryNominalTakeOutGrease_1);

  Flash1.read(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2);
  Flash1.read(SupervisoryTime.SupervisoryNominalTakeOutGrease_2);
  Flash1.read(SupervisoryTime.SupervisoryAverageTakeOutGrease_2); // Read supervisory average grease taken out
  Flash1.read(GreaseOut.Lubrication.Temp.GramSupervisory_2);      // Read supervisory grease output

  Flash1.read(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3);
  Flash1.read(SupervisoryTime.SupervisoryNominalTakeOutGrease_3);
  Flash1.read(SupervisoryTime.SupervisoryAverageTakeOutGrease_3); // Read supervisory average grease taken out
  Flash1.read(GreaseOut.Lubrication.Temp.GramSupervisory_3);      // Read supervisory grease output

  printTotalBytes();
}
