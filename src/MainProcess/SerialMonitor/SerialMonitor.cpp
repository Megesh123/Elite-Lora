// /**
//  * @file Main.cpp
//  * @author Megeshwaran D <megesh@bfes.co.in>
//  * @version 1.0.0
//  * @date 2025-10-01
//  * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
//  *
//  * @note SERIAL MONITOR INTERFACE IMPLEMENTATION
//  */

// /**********************
//  * SERIAL MONITOR INTERFACE IMPLEMENTATION
//  * Handles serial communication, user interface, and system monitoring
//  * Provides password protection, real-time data display, and system control
//  **********************/

// #include "Headerfile.h" // Include main system header

// int ioCheckcount;

// /**********************
//  * PRINT FORMATTED DATE TIME
//  * Displays the current date and time in a formatted manner
//  **********************/
// void SERIALMONITOR::printMainMenu()
// {
//     Serial.println("\nENTER COMMAND (1-3):"); // Prompt for command input
//     Serial.println("1) DATA");                // Option 1: View system data
//     Serial.println("2) LIVE DATA");           // Option 2: Live data monitoring
//     Serial.println("3) FLASH DATA");          // Option 3: SD card operations
// }

// /**********************
//  * LIVE DATA PROCESSING
//  * Monitors and displays live data based on user selection
//  **********************/
// void SERIALMONITOR::livedata_process()
// {
//     printLiveDataMenu(); // Display live data menu

//     while (true) // Continuous monitoring loop
//     {
//         if (Serial.available() > 0) // Check if serial data is available
//         {
//             String input = Serial.readStringUntil('\n'); // Read user input
//             input.trim();                                // Remove whitespace
//             input.toUpperCase();                         // Convert to uppercase

//             if (input == "0") // Return to main menu
//             {
//                 if (SerialFun.liveDataOption != 0) // If currently in a live data option
//                 {
//                     SerialFun.liveDataOption = 0; // Reset live data option
//                     printLiveDataMenu();          // Show live data menu again
//                 }
//                 else // If already at top level
//                 {
//                     printMainMenu(); // Return to main menu
//                     return;          // Exit live data process
//                 }
//             }
//             else if (input.length() == 1 && input.toInt() >= 1 && input.toInt() <= 4) // Valid option selected
//             {
//                 SerialFun.liveDataOption = input.toInt(); // Set selected option

//                 if (SerialFun.liveDataOption == 1) // Live process monitoring
//                 {
//                     if (GreaseOut.Lubrication.Enable) // If lubrication active
//                     {
//                         Serial.println("LUBRICATION PROCESSING...");
//                     }
//                     else if (GreaseOut.PauseTime.Enable) // If pause time active
//                     {
//                         Serial.println("PAUSE TIME PROCESSING...");
//                     }
//                     else if (GreaseOut.TestRun.Enable) // If test run active
//                     {
//                         Serial.println("TEST RUN PROCESSING...");
//                     }
//                     else if (GreaseOut.Reservoir.Enable) // If refill mode active
//                     {
//                         Serial.print("REFILL MODE PROCESSING...");
//                     }
//                 }
//                 else if (SerialFun.liveDataOption == 2) // Current and voltage data
//                 {
//                     Serial.println("CURRENT & VOLTAGE");
//                 }
//                 else if (SerialFun.liveDataOption == 3) // Total run time
//                 {
//                     Serial.println("TOTAL RUN TIME");
//                 }
//                 else if (SerialFun.liveDataOption == 4) // Date and time
//                 {
//                     Serial.println("DATE & TIME");
//                     printFormattedDateTime(); // Display current date/time
//                 }
//             }
//             else // Invalid input
//             {
//                 Serial.println("INVALID INPUT. ENTER 1-5 OR '0'");
//             }
//         }

//         unsigned long currentMillis = millis();                                  // Get current time
//         if (currentMillis - SerialFun.previousMillisTime >= SerialFun.interval2) // Check update interval
//         {
//             SerialFun.previousMillisTime = currentMillis; // Update previous time

//             switch (SerialFun.liveDataOption) // Process based on selected option
//             {
//             case 1:                               // Live process monitoring
//                 if (GreaseOut.Lubrication.Enable) // Lubrication process
//                 {
//                     if (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_SENSE && SerialFun.last_lub_state) // Proximity sensor pressed
//                     {
//                         SerialFun.last_lub_state = false; // Update state
//                         Serial.printf("DISCHARGE VOLUME: %.2f, REFILL PERCENTAGE: %.2f%%\n",
//                                       GreaseOut.Lubrication.Temp.Grams, GreaseOut.Reservoir.Percentage); // Display discharge info
//                     }
//                     else if (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_NOT_SENSE) // Proximity sensor released
//                     {
//                         SerialFun.last_lub_state = true; // Update state
//                     }
//                 }
//                 else if (GreaseOut.PauseTime.Enable) // Pause time process
//                 {
//                     if (GreaseOut.PauseTime.Seconds != SerialFun.last_PauseTimeSec) // If time changed
//                     {
//                         SerialFun.last_PauseTimeSec = GreaseOut.PauseTime.Seconds;                                                                                  // Update last time
//                         Serial.printf("PAUSE TIME: %02d:%02d:%02d\n", GreaseOut.PauseTime.Temp.Hour, GreaseOut.PauseTime.Temp.Minute, GreaseOut.PauseTime.Seconds); // Display pause time
//                     }
//                 }
//                 break;

//             case 2:                                    // Current and voltage data
//                 SerialFun.count_for_current_voltage++; // Increment counter

//                 if (SerialFun.count_for_current_voltage > 10) // Every 10 cycles
//                 {
//                     if (CurrentMonitor.measured <= 0)                                                                    // If current reading is zero
//                         CurrentMonitor.measured = 0;                                                                     // Ensure zero value
//                     Serial.printf("CURRENT: %.2fA, VOLTAGE: %.2fV\n", CurrentMonitor.measured, VoltageMonitor.measured); // Display electrical data
//                 }

//                 if (SerialFun.count_for_current_voltage > 10) // Reset counter
//                     SerialFun.count_for_current_voltage = 0;
//                 break;

//             case 3:                                                             // Total run time
//                 if (TotalOperationTime.Seconds != SerialFun.last_Total_Run_Sec) // If run time changed
//                 {
//                     SerialFun.last_Total_Run_Sec = TotalOperationTime.Seconds; // Update last time
//                     Serial.printf("TOTAL RUN TIME: %ldh %ldm %ds\n",
//                                   TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds); // Display run time
//                 }
//                 break;

//             case 4:    // Date and time
//                 break; // Handled separately
//             }
//         }
//     }
// }

// /**********************
//  * PRINT LIVE DATA MENU
//  * Displays the live data monitoring options to the user
//  **********************/
// void SERIALMONITOR::printLiveDataMenu()
// {
//     Serial.println("\n=== LIVE DATA MONITORING ===");
//     Serial.println("1) LIVE PROCESS");           // Option 1: Process monitoring
//     Serial.println("2) VOLTAGE & CURRENT");      // Option 3: Electrical data
//     Serial.println("3) TOTAL RUN TIME");         // Option 4: Runtime data
//     Serial.println("4) DATE & TIME");            // Option 5: Date/time data
//     Serial.println("Enter option (1-5) or '0'"); // Instructions
// }

// /**********************
//  * PRINT FORMATTED DATE TIME
//  * Displays the current date and time in a formatted manner
//  **********************/
// void SERIALMONITOR::printFormattedDateTime()
// {
//     DateTime now = rtc.getDateTime();                                       // Get current time from RTC
//     Serial.printf("DATE: %02d-%02d-%04d\n", now.date, now.month, now.year); // Format: DD-MM-YYYY
//     Serial.printf("TIME: %02d:%02d\n", now.hour, now.minute);               // Format: HH:MM
// }

// /**********************
//  * SERIAL MONITOR ALERT ENABLE
//  * Enables and configures system alerts based on serial commands
//  **********************/
// void SERIALMONITOR::SerialMonitorAlertEnable()
// {
//     if (SerialFun.input.equalsIgnoreCase("AlertClear")) // Clear all alerts
//     {
//         Alert.Enable = false; // Disable alerts
//         Alert.Name = "NO";    // Reset alert name
//         AlertFun.clear();     // Clear alert system
//     }
//     else if (SerialFun.input.equalsIgnoreCase("overload")) // Simulate overload
//     {
//         Alert.Lora.DataSend = false;                   // Reset LoRa flag
//         Alert.Name = "OVER_LOAD_ALERT";                // Set alert name
//         Alert.OverloadValue = CurrentMonitor.measured; // Set overload value
//         Alert.Enable = true;                           // Enable alert
//     }
//     else if (SerialFun.input.equalsIgnoreCase("refill")) // Simulate low refill
//     {
//         Alert.Name = "LOW_LEVEL_RESERVOIR";                    // Set alert name
//         Alert.ReservoirValue = GreaseOut.Reservoir.Percentage; // Set reservoir value
//         Alert.Lora.DataSend = false;                           // Reset LoRa flag
//         Alert.Enable = true;                                   // Enable alert
//     }
//     else if (SerialFun.input.equalsIgnoreCase("Proximity")) // Simulate proximity issue
//     {
//         Alert.Name = "PROXIMITY_NOT_SENSE"; // Set alert name
//         Alert.Lora.DataSend = false;        // Reset LoRa flag
//         Alert.Enable = true;                // Enable alert
//     }
//     else if (SerialFun.input.equalsIgnoreCase("eeprom")) // Write to EEPROM
//     {
//         Eeprom.write();
//     }
//     else if (SerialFun.input.equalsIgnoreCase("add")) // Reset flash addresses
//     {
//         flash.currentAddr = 0x82EA24; // Reset current address
//         flash.readAddr = 0x82EA24;    // Reset read address
//         flash.StartDate = "";         // Clear start date
//         flash.StartAddress = "";      // Clear start address
//         Eeprom.write();               // Write to EEPROM
//     }
//     else if (SerialFun.input.equalsIgnoreCase("outlet")) // Reset outlet count
//     {
//         Config.NoOfOutlet = -1;        // Invalidate outlet count
//         Eeprom.write();                // Write to EEPROM
//         vTaskDelay(pdMS_TO_TICKS(10)); // Short delay
//         ESP.restart();                 // Restart system
//     }
//     else if (SerialFun.input.equalsIgnoreCase("write data")) // Enable offline data writing
//     {
//         GreaseOut.Lubrication.Enable = false; // Disable lubrication
//         GreaseOut.PauseTime.Enable = false;   // Disable pause time
//         GreaseOut.TestRun.Enable = false;     // Disable test run
//         Motor.off();                          // Turn off motor
//         SerialFun.OfflineDataWriteProcess = true;
//     }
//     else if (SerialFun.input.equalsIgnoreCase("passwordClear")) // Clear password delay
//     {
//         SerialFun.WrongPassword_delay = 1; // Reset delay
//     }
//     else if (SerialFun.input.equalsIgnoreCase("reset")) // Clear password delay
//     {
//         Config.NoOfOutlet = -1;         // Invalidate outlet count
//         Eeprom.write();                 // Write to EEPROM
//         vTaskDelay(pdMS_TO_TICKS(100)); // Short delay
//         ESP.restart();                  // Restart system
//     }
//     else
//     {
//         Serial.println("INVALID INPUT. PLEASE ENTER 1-3 OR '0'");
//     }
// }

// /**********************
//  * WRONG PASSWORD HANDLER
//  * Manages delays and security after incorrect password attempts
//  **********************/
// void SERIALMONITOR::worngPassword()
// {
//     if (SerialFun.WrongPassword) // If wrong password entered
//     {
//         for (int i = 0; i < SerialFun.WrongPassword_delay; i++) // Delay loop
//         {
//             Serial.printf("PLEASE WAIT %d SECONDS\n", SerialFun.WrongPassword_delay - i); // Show wait time

//             for (int j = 0; j < 10; j++) // Check for emergency override
//             {
//                 if (Serial.available() > 0) // If serial input available
//                 {
//                     String incomingString = Serial.readStringUntil('\n'); // Read input
//                     incomingString.trim();                                // Trim whitespace

//                     if (incomingString == "BFES@2020") // Emergency override code
//                     {
//                         SerialFun.WrongPassword_delay = 1; // Clear delay
//                         Serial.println("Delay cleared by command.");
//                         break;
//                     }
//                 }
//                 vTaskDelay(pdMS_TO_TICKS(100)); // Short delay
//             }
//         }
//         SerialFun.WrongPassword_delay *= 2; // Exponential backoff
//         SerialFun.WrongPassword = false;    // Reset flag
//     }
// }

// /**********************
//  * AVAILABLE SERIAL DATA HANDLER
//  * Checks for available serial data and processes input accordingly
//  **********************/
// void SERIALMONITOR::available()
// {
//     if (Serial.available() > 0) // If serial data available
//     {
//         if (SerialFun.userPassword) // If already authenticated
//         {
//             serial.passwordCorrect(); // Process authenticated commands
//         }
//         else
//         {
//             serial.passwordCheck(); // Check password
//         }
//     }
// }

// /**********************
//  * PASSWORD CHECKER
//  * Verifies user password input against the system password
//  **********************/
// void SERIALMONITOR::passwordCheck()
// {
//     String userPasswordRead = Serial.readStringUntil('\n'); // Read password
//     userPasswordRead.trim();                                // Trim whitespace

//     if (userPasswordRead == SYSTEM_PASSWORD) // If password matches
//     {
//         serial.passwordValid(); // Validate password
//     }
//     else
//     {
//         serial.passwordinvalid(); // Handle invalid password
//     }
// }

// /**********************
//  * PASSWORD INVALID HANDLER
//  * Manages actions after an incorrect password attempt
//  **********************/
// void SERIALMONITOR::passwordinvalid()
// {
//     Serial.println("WRONG PASSWORD");                                       // Inform user
//     if (SerialFun.WrongPassword_delay_count >= WRONG_PASSWORD_MAX_ATTEMPTS) // If too many attempts
//     {
//         SerialFun.WrongPassword = true; // Enable delay
//     }
//     else
//     {
//         SerialFun.WrongPassword_delay_count++; // Increment attempt counter
//     }
//     serial.worngPassword();       // Handle wrong password
//     vTaskDelay(pdMS_TO_TICKS(1)); // Short delay
// }

// /**********************
//  * PASSWORD VALID HANDLER
//  * Manages actions after a correct password entry
//  **********************/
// void SERIALMONITOR::passwordValid()
// {
//     SerialFun.WrongPassword_delay_count = 0; // Reset attempt counter
//     SerialFun.WrongPassword_delay = 1;       // Reset delay
//     SerialFun.WrongPassword = false;         // Reset wrong password flag
//     SerialFun.userPassword = true;           // Set authenticated flag
//     serial.printMainMenu();                  // Show main menu
// }

// /**********************
//  * PASSWORD CORRECT COMMAND PROCESSOR
//  * Processes user commands after successful password authentication
//  **********************/
// void SERIALMONITOR::passwordCorrect()
// {
//     SerialFun.input = Serial.readStringUntil('\n'); // Read command
//     SerialFun.input.trim();                         // Trim whitespace
//     SerialFun.input.toUpperCase();                  // Convert to uppercase

//     if (SerialFun.input == "0") // Return to main menu
//     {
//         serial.printMainMenu(); // Show main menu
//     }
//     else if (SerialFun.input.length() == 1 && SerialFun.input.toInt() >= 1 && SerialFun.input.toInt() <= 3) // Valid menu option
//     {
//         int choice = SerialFun.input.toInt(); // Get choice

//         switch (choice) // Handle choice
//         {
//         case 1:                 // Data display
//             serial.printdata(); // Print system data
//             Serial.println("\nEnter '0' to return to main menu");
//             break;
//         case 2:                        // Live data
//             serial.livedata_process(); // Start live data monitoring
//             Serial.println("\nEnter '0' to return to main menu");
//             break;
//         case 3: // Live data
//             Flash.SerialPrintFlashData();
//             Serial.println("\nEnter '0' to return to main menu");
//             break;
//         }
//     }
//     else // Special commands
//     {
//         serial.SerialMonitorAlertEnable(); // Process alert commands
//     }
// }

// /**********************
//  * PRINT SYSTEM DATA
//  * Displays comprehensive system configuration and status data via serial
//  **********************/
// #define PRINT(name, value) \
//     Serial.printf("%-10s %-10s\n", #name, String(value).c_str());

// /**********************
//  * PRINT DATA
//  * Displays comprehensive system configuration and status data via serial
//  **********************/
// void SERIALMONITOR::printdata()
// {

//     Serial.println("\n===== NETWORK CONFIGURATION =====");
//     PRINT("HOST ID > ", LoraDetails.host);
//     PRINT("PORT > ", LoraDetails.port);
//     PRINT("CLIENT ID > ", LoraDetails.Clientid);
//     PRINT("LoRa USERNAME > ", LoraDetails.Username);
//     PRINT("LoRa PASSWORD > ", LoraDetails.Password);

//     Serial.println("\n===== LUBRICATION PARAMETERS =====");
//     PRINT("DISCHARGE VOLUME >", GreaseOut.Lubrication.Grams);
//     PRINT("TOTAL DISCHARGE VOLUME > ", GreaseOut.Lubrication.Total.Grams);

//     char buffer1[50];
//     sprintf(buffer1, "PAUSE TIME > HOUR %02d MINUTE %02d\n", GreaseOut.PauseTime.Hour, GreaseOut.PauseTime.Minute);
//     Serial.println(buffer1);

//     char buffer2[50];
//     sprintf(buffer2, "TOTAL PAUSE TIME > HOUR %02d MINUTE %02d\n", GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute);
//     Serial.println(buffer2);

//     PRINT("NUMBER OF OUTLET > ", Config.NoOfOutlet);
//     PRINT("RESERVOIR CAPACITY > ", GreaseOut.Reservoir.Capacity * 1000);
//     PRINT("RESERVOIR LEVEL > ", GreaseOut.Reservoir.Percentage);

//     Serial.println("===== End =====");
// }