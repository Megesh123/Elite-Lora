/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note EEPROM emulation using flash memory for non-volatile data storage.
 */

#include "Headerfile.h" // Include main system header
#include "FlashWrite1.h"

/**********************
 * EEPROM Init Method
 * Initialize EEPROM with default values if uninitialized or corrupted
 **********************/
void EEPROMFUNCTION::init()
{
  // Config.NoOfOutlet = -1;
  //       Check if outlet count is invalid (indicating uninitialized EEPROM)
  if (Grease.TotalProxityCount < 0)
  {
    Grease.TotalProxityCount = 0;
  }
  // Restore factory defaults when the stored EEPROM record failed its
  // integrity check (blank/corrupt/half-written) as well as when it is
  // explicitly uninitialised (NoOfOutlet < 0).
  if (!Flash1.valid() || Config.NoOfOutlet < 0)
  {
    /**********************
     * EEPROM Initialization
     * Set all system parameters to default values
     **********************/
    GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
    GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
    GreaseOut.TestRun.Enable = false;     // Disable test run mode
    GreaseOut.Sleep.Enable = false;       // Disable sleep mode

    /**********************
     * DEFAULT PARAMETERS RESET
     * Initialize system configuration to factory defaults
     **********************/
    GreaseOut.Lubrication.Grams = 1;                    // Reset grease quantity
    GreaseOut.PauseTime.Hour = 0;                       // Reset pause hours
    GreaseOut.PauseTime.Minute = 1;                     // Reset pause minutes
    GreaseOut.Lubrication.Total.Grams = 0;              // Reset total grease dispensed
    GreaseOut.PauseTime.Total.Hour = 0;                 // Reset total pause hours
    GreaseOut.PauseTime.Total.Minute = 0;               // Reset total pause minutes
    GreaseOut.Lubrication.Live.Grams = 0;               // Reset live grease quantity
    GreaseOut.PauseTime.Live.Hour = 0;                  // Reset live pause hours
    GreaseOut.PauseTime.Live.Minute = 0;                // Reset live pause minutes
    GreaseOut.Reservoir.Capacity = 0;                   // Reset reservoir capacity
    GreaseOut.Reservoir.Percentage = 0;                 // Reset reservoir percentage
    GreaseOut.Reservoir.Temp._old_reservoir_value = -1; // Reset old reservoir value for change detection
    GreaseOut.TestRun.min = TEST_RUN_MIN;
    GreaseOut.TestRun.sec = TEST_RUN_SEC;

    Config.NoOfOutlet = 1; // Set default outlet count to 1

    /**********************
     * TEMPORARY PARAMETERS RESET
     * Clear temporary calculation variables
     **********************/
    GreaseOut.Lubrication.Temp.Grams = 0; // Reset temporary grease quantity
    GreaseOut.PauseTime.Temp.Hour = 0;    // Reset temporary pause hours
    GreaseOut.PauseTime.Temp.Minute = 0;  // Reset temporary pause minutes

    /**********************
     * TOTAL OPERATION TIME RESET
     * Clear total runtime counters
     **********************/
    TotalOperationTime.Seconds = 0; // Reset run seconds
    TotalOperationTime.Minute = 0;  // Reset run minutes
    TotalOperationTime.Hour = 0;    // Reset run hours

    /**********************
     * SERIAL COMMUNICATION RESET
     * Clear serial input buffers and flags
     **********************/
    SerialFun.WrongPassword = false;         // Reset wrong password flag
    SerialFun.WrongPassword_delay = 1;       // Reset password delay
    SerialFun.WrongPassword_delay_count = 0; // Reset wrong password count

    /**********************
     * LORA DETAILS RESET
     * Clear LoRaWAN provisioning parameters
     **********************/
    LoraDetails.DEVEUI = "";   // Clear LoRaWAN Device EUI
    LoraDetails.APPKEY = "";   // Clear LoRaWAN App Key
    LoraDetails.APPEUI = "";   // Clear LoRaWAN App EUI
    LoraDetails.Clientid = ""; // Clear client ID
    LoraDetails.Wifi = "";     // Clear WiFi SSID
    LoraDetails.Pass = "";     // Clear WiFi password
    LoraDetails.port = 0;      // Reset network port

    /**********************
     * ALERT PARAMETERS RESET
     * Clear alert configuration and status
     **********************/
    Alert.Enable = false;     // Disable alert occurrence
    SetAlertName("NO");       // Reset alert name
    Alert.OverloadValue = 0;  // Reset overload value
    Alert.ReservoirValue = 0; // Reset reservoir level alert
    Alert.NameCount = 0;      // Reset alert count

    /**********************
     * SYSTEM RTC RESET
     * Clear real-time clock parameters
     **********************/
    RTC.year = 0;       // Reset year
    RTC.month = 0;      // Reset month
    RTC.date = 0;       // Reset date
    RTC.hour = 0;       // Reset hour
    RTC.minute = 0;     // Reset minute
    RTC.Enable = false; // Disable RTC
    RTC.seconds = 0;    // Reset seconds

    GreaseOut.PauseTime.Seconds = 0;         // Reset pause seconds
    GreaseOut.Reservoir.Temp.Percentage = 0; // Reset refill percentage
    CurrentMonitor.measured = 0;             // Reset current reading
    VoltageMonitor.measured = 0;             // Reset voltage reading
    flash.Write = false;                     // Reset flash write flag
    flash.currentAddr = 0x82EA24;            // Set default flash address
    flash.readAddr = 0x82EA24;               // Set default read address
    flash.Temp.currentAddr = "82EA24";       // Set default flash address string
    flash.OnceWrite = true;                  // Reset once write flag
    flash.DataReceived = false;              // Reset flash data confirm
    flash.StartDate = "";                    // Clear start date
    flash.StartAddress = "";                 // Clear start address
    flash.LastDate = "";                     // Clear last date
    flash.PerverseDate = "";                 // Clear previous date
    bluetooth.com_data = "";                 // Clear Bluetooth data
    bluetooth.DataReceived = false;          // Reset Bluetooth data flag
    bluetooth.Received = 0;                  // Reset Bluetooth packet count
    Alert.SnoozeReservoir = false;           // Reset reservoir snooze

    start_init = 0;               // Reset start initialization flag
    LoraFun.DataReceived = false; // Reset LoRa data received flag

    Reset_SendData_Lora = false; // Reset send data via LoRa flag

    SupervisoryTime.SupervisoryAverageTakeOutGrease_1 = 0;                          // Reset supervisory average grease taken out
    SupervisoryTime.SupervisoryNominalTakeOutGrease_1 = 0;                          // Reset supervisory nominal grease taken out
    SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1 = false; // Reset supervisory nominal grease process completion flag
    GreaseOut.Lubrication.Temp.GramSupervisory_1 = 0;                               // Reset supervisory grease output

    SupervisoryTime.SupervisoryAverageTakeOutGrease_2 = 0;                          // Reset supervisory average grease taken out
    SupervisoryTime.SupervisoryNominalTakeOutGrease_2 = 0;                          // Reset supervisory nominal grease taken out
    SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2 = false; // Reset supervisory nominal grease process completion flag
    GreaseOut.Lubrication.Temp.GramSupervisory_2 = 0;                               // Reset supervisory grease output

    SupervisoryTime.SupervisoryAverageTakeOutGrease_3 = 0;                          // Reset supervisory average grease taken out
    SupervisoryTime.SupervisoryNominalTakeOutGrease_3 = 0;                          // Reset supervisory nominal grease taken out
    SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3 = false; // Reset supervisory nominal grease process completion flag
    GreaseOut.Lubrication.Temp.GramSupervisory_3 = 0;                               // Reset supervisory grease output

    SupervisoryTime.AlertCheckOutGrease = 0; // Reset supervisory alert check grease

    _pump1_condtion = "ANALYSIS";
    _pump2_condtion = "ANALYSIS";
    _pump3_condtion = "ANALYSIS";

    _pump1_percentage = 0;
    _pump2_percentage = 0;
    _pump3_percentage = 0;
    Lora_disconnected_count = 0;

    Alert.EnableDisable.Supervisory1 = true;
    Alert.EnableDisable.Supervisory2 = true;
    Alert.EnableDisable.Supervisory3 = true;
    Alert.EnableDisable.RefillAlert = true;

    Alert.Total.NoOfAlert = 0;
    Alert.Total.Reservoir = 0;
    Alert.Total.SignalNotDetected = 0;
    Alert.Total.OverLoad = 0;
    Alert.Total.Supervisory1 = 0;
    Alert.Total.Supervisory2 = 0;
    Alert.Total.Supervisory3 = 0;
    Alert.Total.PumpWeak1 = 0;
    Alert.Total.PumpWeak2 = 0;
    Alert.Total.PumpWeak3 = 0;
    Alert.Total.WorngPassword = 0;
    Alert.Total.enable = false;

    SupervisoryTime.AverageEnable_Supervisory1 = false;
    SupervisoryTime.AverageEnable_Supervisory2 = false;
    SupervisoryTime.AverageEnable_Supervisory3 = false;

    SupervisoryTime.NWC_SUP1 = 0;
    SupervisoryTime.AWC_SUP1 = 0;
    SupervisoryTime.NWC_SUP2 = 0;
    SupervisoryTime.AWC_SUP2 = 0;
    SupervisoryTime.NWC_SUP3 = 0;
    SupervisoryTime.AWC_SUP3 = 0;
    SupervisoryTime.Supervisory1_Count = 0;
    SupervisoryTime.Supervisory2_Count = 0;
    SupervisoryTime.Supervisory3_Count = 0;

    SupervisoryTime.ELH_OL1 = 0;
    SupervisoryTime.ELH_OL2 = 0;
    SupervisoryTime.ELH_OL3 = 0;
    _Controller_online_data_send = false;
    Grease.TotalProxityCount = 0;

    AlreadyTestedBoard = false;

    oldStateCount = 0;
    ReconfigEnableRefillNeed = false;
    ChechOTAUpdate = false;
    OTA.current_version = CURRENT_VERSION_LINK_ULTRA_ELITE;

    Lora_twp.RefillAlertEnableDisable = false;
    Lora_twp.SleepModeOff = false;
    Lora_twp.SleepModeOn = false;
    Lora_twp.SupervisoryAlertEnableDisable = false;
    Lora_twp.TestRunOff = false;
    Lora_twp.TestRunOn = false;
    Lora_twp.RefillSuccessful = false;
    Lora_twp.AlertClear = false;
    Lora_twp.RefillSuccessful_Data = "";
    Lora_twp.AlertClear_Data = "";
    /**********************
     * EEPROM WRITE TO FLASH
     * Persist default configuration to flash memory
     **********************/
    Flash.erase(4);        // Erase flash sector 4
    Eeprom.write();        // Write default values to flash
    Eeprom.addressWrite(); // Write address information
    Serial.println("=====================================");
    Serial.println("init completed");
    Serial.println("=====================================");
  }

  if (start_init == 0)
  {
    /**********************
     * EEPROM Initialization
     * Set all system parameters to default values
     **********************/
    GreaseOut.Lubrication.Enable = false; // Disable lubrication mode
    GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
    GreaseOut.TestRun.Enable = false;     // Disable test run mode
    GreaseOut.Sleep.Enable = false;       // Disable sleep mode

    /**********************
     * DEFAULT PARAMETERS RESET
     * Initialize system configuration to factory defaults
     **********************/
    GreaseOut.Lubrication.Grams = 1;       // Reset grease quantity
    GreaseOut.PauseTime.Hour = 0;          // Reset pause hours
    GreaseOut.PauseTime.Minute = 1;        // Reset pause minutes
    GreaseOut.Lubrication.Total.Grams = 0; // Reset total grease dispensed
    GreaseOut.PauseTime.Total.Hour = 0;    // Reset total pause hours
    GreaseOut.PauseTime.Total.Minute = 0;  // Reset total pause minutes
    GreaseOut.Lubrication.Live.Grams = 0;  // Reset live grease quantity
    GreaseOut.PauseTime.Live.Hour = 0;     // Reset live pause hours
    GreaseOut.PauseTime.Live.Minute = 0;   // Reset live pause minutes
    GreaseOut.TestRun.min = TEST_RUN_MIN;
    GreaseOut.TestRun.sec = TEST_RUN_SEC;

    /**********************
     * TEMPORARY PARAMETERS RESET
     * Clear temporary calculation variables
     **********************/
    GreaseOut.Lubrication.Temp.Grams = 0; // Reset temporary grease quantity
    GreaseOut.PauseTime.Temp.Hour = 0;    // Reset temporary pause hours
    GreaseOut.PauseTime.Temp.Minute = 0;  // Reset temporary pause minutes

    bluetooth.ModeEnable = false; // Reset Bluetooth mode enable flag
  }

  // Serial.printf("SSID: %s\n PASS: %s\n HOST: %s\n PORT: %d\n CLIENTID: %s\n", LoraDetails.Wifi.c_str(), LoraDetails.Pass.c_str(), LoraDetails.host.c_str(), LoraDetails.port, LoraDetails.Clientid.c_str());
}
