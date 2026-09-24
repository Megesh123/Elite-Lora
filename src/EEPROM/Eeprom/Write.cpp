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

/**********************
 * EEPROM Write Method
 * Write all system configuration and operational data to flash memory
 **********************/
void EEPROMFUNCTION::write()
{
  /**********************
   * MODE FLAGS STORAGE
   * Write operational mode enable/disable flags
   **********************/
  Flash1.write(GreaseOut.Lubrication.Enable); // Write lubrication mode flag
  Flash1.write(GreaseOut.PauseTime.Enable);   // Write pause time mode flag
  Flash1.write(GreaseOut.TestRun.Enable);     // Write test run mode flag
  Flash1.write(GreaseOut.Reservoir.Enable);   // Write refill mode flag

  /**********************
   * TEMPORARY PARAMETERS STORAGE
   * Write temporary calculation variables
   **********************/
  Flash1.write(GreaseOut.Lubrication.Temp.Grams);    // Write temporary grease quantity
  Flash1.write(GreaseOut.PauseTime.Temp.Hour);       // Write temporary pause hours
  Flash1.write(GreaseOut.PauseTime.Temp.Minute);     // Write temporary pause minutes
  Flash1.write(GreaseOut.PauseTime.Seconds);         // Write pause time seconds
  Flash1.write(GreaseOut.Reservoir.Temp.Percentage); // Write temporary refill percentage

  /**********************
   * SENSOR READINGS STORAGE
   * Write current and voltage sensor measurements
   **********************/
  Flash1.write(CurrentMonitor.measured); // Write current reading
  Flash1.write(VoltageMonitor.measured); // Write voltage reading

  /**********************
   * TOTAL OPERATION TIME STORAGE
   * Write total system runtime counters
   **********************/
  Flash1.write(TotalOperationTime.Hour);    // Write total run hours
  Flash1.write(TotalOperationTime.Minute);  // Write total run minutes
  Flash1.write(TotalOperationTime.Seconds); // Write total run seconds

  /**********************
   * ALERT PARAMETERS STORAGE
   * Write alert configuration and status
   **********************/
  // Flash1.write(SerialFun.WrongPassword);             // Write wrong password flag
  // Flash1.write(SerialFun.WrongPassword_delay_count); // Write wrong password count
  // Flash1.write(SerialFun.WrongPassword_delay);       // Write wrong password delay
  Flash1.write(Alert.Enable);             // Write alert enable flag
  Flash1.write(Alert.OverloadValue);      // Write overload value
  Flash1.write(Alert.ReservoirValue);     // Write reservoir level value
  Flash1.write(Alert.NameCount);          // Write alert count
  Flash1.write(Alert.SnoozeReservoir);    // Write reservoir snooze state
  Flash1.writeString(String(Alert.Name)); // Write alert name (fixed buffer -> String for storage)

  /**********************
   * TOTALS STORAGE
   * Write cumulative operational totals
   **********************/
  Flash1.write(GreaseOut.Lubrication.Total.Grams); // Write total grease dispensed
  Flash1.write(GreaseOut.PauseTime.Total.Hour);    // Write total pause hours
  Flash1.write(GreaseOut.PauseTime.Total.Minute);  // Write total pause minutes

  /**********************
   * CONFIGURATION PARAMETERS STORAGE
   * Write system configuration settings
   **********************/
  Flash1.write(GreaseOut.Sleep.Enable);       // Write sleep mode flag
  Flash1.write(flash.Write);                  // Write flash write flag
  Flash1.write(GreaseOut.Lubrication.Grams);  // Write grease quantity
  Flash1.write(GreaseOut.PauseTime.Hour);     // Write pause hours
  Flash1.write(GreaseOut.PauseTime.Minute);   // Write pause minutes
  Flash1.write(Config.NoOfOutlet);            // Write outlet count
  Flash1.write(GreaseOut.Reservoir.Capacity); // Write reservoir capacity

  /**********************
   * SYSTEM FLAGS AND DELAYS STORAGE
   * Write various system status flags and delay settings
   **********************/
  Flash1.write(flash.DataReceived); // Write flash data confirm flag
  Flash1.write(flash.OnceWrite);    // Write once write flag

  /**********************
   * FLASH ADDRESS STORAGE
   * Write stored flash memory addresses
   **********************/
  flash.Temp.currentAddr = String(flash.currentAddr, HEX); // Convert flash address to hex string
  flash.Temp.readAddr = String(flash.readAddr, HEX);       // Convert read address to hex string
  Flash1.writeString(flash.Temp.currentAddr);              // Write flash address
  Flash1.writeString(flash.Temp.readAddr);                 // Write read address

  /**********************
   * DATE PARAMETERS STORAGE
   * Write stored date information
   **********************/
  Flash1.writeString(flash.LastDate);     // Write last date
  Flash1.writeString(flash.PerverseDate); // Write previous date

  /**********************
   * RTC PARAMETERS STORAGE
   * Write real-time clock configuration and time data
   **********************/
  Flash1.write(bluetooth.Received); // Write Bluetooth data received flag
  Flash1.write(RTC.Enable);         // Write RTC initialized flag
  Flash1.write(RTC.year);           // Write year
  Flash1.write(RTC.month);          // Write month
  Flash1.write(RTC.date);           // Write date
  Flash1.write(RTC.hour);           // Write hour
  Flash1.write(RTC.minute);         // Write minute

  /**********************
   * LORA DETAILS STORAGE
   * Write LoRaWAN provisioning parameters
   **********************/
  Flash1.writeString(LoraDetails.APPEUI);   // Write LoRaWAN App EUI
  Flash1.writeString(LoraDetails.Clientid); // Write client ID
  Flash1.writeString(LoraDetails.DEVEUI);   // Write LoRaWAN Device EUI
  Flash1.writeString(LoraDetails.APPKEY);   // Write LoRaWAN App Key
  Flash1.write(LoraDetails.port);           // Write network port
  Flash1.writeString(LoraDetails.Wifi);     // Write WiFi SSID
  Flash1.writeString(LoraDetails.Pass);     // Write WiFi password

  /**********************
   * BLUETOOTH DATA STORAGE
   * Write Bluetooth communication data
   **********************/
  Flash1.writeString(bluetooth.com_data); // Write Bluetooth communication data

  Flash1.write(capcity);                        // Write tank capacity
  Flash1.write(TankCapcity);                    // Write tank capacity
  Flash1.write(GreaseOut.Reservoir.Percentage); // Write reservoir percentage

  Flash1.write(GreaseOut.Lubrication.Live.Grams); // Write live grease quantity
  Flash1.write(GreaseOut.PauseTime.Live.Hour);    // Write live pause hours
  Flash1.write(GreaseOut.PauseTime.Live.Minute);  // Write live pause minutes
  Flash1.write(start_init);                       // Write start initialization flag

  Flash1.write(bluetooth.DataReceived); // Write Bluetooth data received flag
  Flash1.write(LoraFun.DataReceived);   // Write LoRa data received flag
  Flash1.write(Reset_SendData_Lora);    // Write reset send data via LoRa flag

  Flash1.write(SupervisoryTime.SupervisoryAverageTakeOutGrease_1); // Write supervisory average grease taken out
  Flash1.write(GreaseOut.Lubrication.Temp.GramSupervisory_1);      // Write supervisory grease output
  Flash1.write(SupervisoryTime.AlertCheckOutGrease);               // Write supervisory

  Flash1.writeString(_pump1_condtion); // Write pump 1 condition
  Flash1.writeString(_pump2_condtion); // Write pump 2 condition
  Flash1.writeString(_pump3_condtion); // Write pump 3 condition
  Flash1.write(_pump1_percentage);     // Write pump 1 percentage
  Flash1.write(_pump2_percentage);     // Write pump 2 percentage
  Flash1.write(_pump3_percentage);     // Write pump 3 percentage

  Flash1.write(GreaseOut.TestRun.min); //
  Flash1.write(GreaseOut.TestRun.sec); //

  Flash1.write(Alert.Total.NoOfAlert);
  Flash1.write(Alert.Total.Reservoir);
  Flash1.write(Alert.Total.OverLoad);
  Flash1.write(Alert.Total.Supervisory1);
  Flash1.write(Alert.Total.Supervisory2);
  Flash1.write(Alert.Total.Supervisory3);
  Flash1.write(Alert.Total.SignalNotDetected);
  Flash1.write(Alert.Total.PumpWeak1);
  Flash1.write(Alert.Total.PumpWeak2);
  Flash1.write(Alert.Total.PumpWeak3);

  Flash1.write(Alert.EnableDisable.Supervisory1);
  Flash1.write(Alert.EnableDisable.Supervisory2);
  Flash1.write(Alert.EnableDisable.Supervisory3);
  Flash1.write(Alert.EnableDisable.RefillAlert);

  Flash1.write(Lora_disconnected_count);

  Flash1.write(SupervisoryTime.AverageEnable_Supervisory1);
  Flash1.write(SupervisoryTime.AverageEnable_Supervisory2);
  Flash1.write(SupervisoryTime.AverageEnable_Supervisory3);

  Flash1.write(SupervisoryTime.NWC_SUP1);
  Flash1.write(SupervisoryTime.AWC_SUP1);
  Flash1.write(SupervisoryTime.Supervisory1_Count);

  Flash1.write(SupervisoryTime.NWC_SUP2);
  Flash1.write(SupervisoryTime.AWC_SUP2);
  Flash1.write(SupervisoryTime.Supervisory2_Count);

  Flash1.write(SupervisoryTime.NWC_SUP3);
  Flash1.write(SupervisoryTime.AWC_SUP3);
  Flash1.write(SupervisoryTime.Supervisory3_Count);

  Flash1.write(SupervisoryTime.ELH_OL1);
  Flash1.write(SupervisoryTime.ELH_OL2);
  Flash1.write(SupervisoryTime.ELH_OL3);

  Flash1.write(GreaseOut.Reservoir.Temp._old_reservoir_value); // Write old reservoir value for change detection
  Flash1.write(Grease.TotalProxityCount);
  Flash1.write(_Controller_online_data_send);
  Flash1.write(oldStateCount);
  Flash1.write(AlreadyTestedBoard);
  Flash1.write(ReconfigEnableRefillNeed);
  Flash1.write(bluetooth.ModeEnable); // Write Bluetooth mode enable flag
  Flash1.write(ChechOTAUpdate);
  Flash1.writeString(OTA.current_version); // Write OTA update version string
  Flash1.write(Lora_twp.RefillAlertEnableDisable);
  Flash1.write(Lora_twp.SleepModeOff);
  Flash1.write(Lora_twp.SleepModeOn);
  Flash1.write(Lora_twp.SupervisoryAlertEnableDisable);
  Flash1.write(Lora_twp.TestRunOff);
  Flash1.write(Lora_twp.TestRunOn);
  Flash1.write(Lora_twp.AlertClear);
  Flash1.write(Lora_twp.RefillSuccessful);
  Flash1.writeString(Lora_twp.RefillSuccessful_Data);
  Flash1.writeString(Lora_twp.AlertClear_Data);
  Flash1.write(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1);
  Flash1.write(SupervisoryTime.SupervisoryNominalTakeOutGrease_1);

  Flash1.write(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2);
  Flash1.write(SupervisoryTime.SupervisoryNominalTakeOutGrease_2);
  Flash1.write(SupervisoryTime.SupervisoryAverageTakeOutGrease_2); // Write supervisory average grease taken out
  Flash1.write(GreaseOut.Lubrication.Temp.GramSupervisory_2);      // Write supervisory grease output

  Flash1.write(SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3);
  Flash1.write(SupervisoryTime.SupervisoryNominalTakeOutGrease_3);
  Flash1.write(SupervisoryTime.SupervisoryAverageTakeOutGrease_3); // Write supervisory average grease taken out
  Flash1.write(GreaseOut.Lubrication.Temp.GramSupervisory_3);      // Write supervisory grease output
  /**********************
   * COMMIT WRITE OPERATIONS
   * Finalize all writes to flash memory
   **********************/
  Flash1.commit();                          // Commit all writes to flash memory
  Serial.println("EEPROM WRITE COMPLETED"); // Log completion message
  // Log.info("EEPROM WRITE COMPLETED"); // Log completion message
}
