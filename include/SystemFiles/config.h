#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
/**********************
 * SYSTEM CONFIGURATION CONSTANTS
 * Centralized configuration for easy maintenance
 **********************/

/**********************
 * PIN ASSIGNMENTS
 * Defines pin numbers for various hardware components
 **********************/
extern const int PIN_PROXIMITY_SENSOR;    // Proximity sensor digital input
extern const int PIN_VOLTAGE_SENSOR;      // Voltage sensor analog input
extern const int PIN_CURRENT_SENSOR;      // Current sensor analog input
extern const int PIN_PUMP_MOTOR;          // Pump motor PWM control
extern const int PIN_BUZZER;              // Buzzer audio output
extern const int PIN_LIMIT_SW_1;          // Limit switch 1 input
extern const int PIN_LIMIT_SW_2;          // Limit switch 2 input
extern const int PIN_LIMIT_SW_3;          // Limit switch 3 input
extern const int PIN_SW_MENU;             // Menu button input
extern const int PIN_SW_BACK;             // Back button input
extern const int PIN_SW_UP;               // Up button input
extern const int PIN_SW_DOWN;             // Down button input
extern const int PIN_SW_OK;               // OK button input
extern const int PIN_LED_SLEEP_FAULT;     // Sleep/Fault status LED
extern const int PIN_LED_OVERLOAD_REFILL; // Overload/Refill status LED
extern const int PIN_LED_TEST_RUN;        // Test Run status LED
extern const int PIN_LED_COMMUNICATION;   // Communication status LED
extern const int PIN_I2C_SDA;             // I2C data line
extern const int PIN_I2C_SCL;             // I2C clock line

/**********************
 * SYSTEM CONSTANTS
 * Defines various system-wide constants and thresholds
 **********************/
extern const int SERIAL_BAUD_RATE;          // Serial monitor baud rate
extern const char *BLE_SERVICE_UUID;        // BLE service UUID
extern const char *BLE_CHARACTERISTIC_UUID; // BLE characteristic UUID

/**********************
 * MOTOR SPEED CONSTANTS
 * Defines motor speed settings and PWM frequency
 **********************/
extern const int PUMP_SPEED_FULL;   // Full speed (0% duty cycle)
extern const int PUMP_SPEED_OFF;    // Motor off (100% duty cycle)
extern const int PUMP_SPEED_MEDIUM; // Medium speed (~25% duty cycle)
extern const int PUMP_FREQUENCY_HZ; // PWM frequency for motor control

/**********************
 * VOLTAGE THRESHOLDS
 * Defines voltage levels for system operation and alerts
 **********************/
extern const float VOLTAGE_12V_SYSTEM_MIN; // Minimum detection for 12V systems
extern const float VOLTAGE_12V_SYSTEM_MAX; // Maximum detection for 12V systems
extern const float VOLTAGE_12V_THRESHOLD;  // Low voltage threshold for 12V
extern const float VOLTAGE_24V_THRESHOLD;  // Low voltage threshold for 24V

/**********************
 * TIMING CONSTANTS
 * Defines various timing intervals and durations
 **********************/
extern const unsigned long PROXIMITY_TIMEOUT_SEC;           // Proximity sensor timeout
extern const unsigned long OVERLOAD_CHECK_INTERVAL_MS;      // Current monitoring interval
extern const unsigned long APP_ONLINE_TIMEOUT_SEC;          // Application online timeout
extern const unsigned long OVER_LOAD_TIME_MS;               // Overload condition duration
extern const unsigned long PRESSURE_INCREASE_FAULT_TIME_MS; // Pressure increase fault
extern const unsigned long PRESSURE_DECREASE_FAULT_TIME_MS; // Pressure decrease fault

/**********************
 * SYSTEM THRESHOLDS
 * Defines thresholds for alerts and warnings
 **********************/
extern const float RESERVOIR_LOW_LEVEL_PERCENT; // Reservoir low level threshold
extern const char *SYSTEM_PASSWORD;             // Default system access password
extern const int WRONG_PASSWORD_MAX_ATTEMPTS;   // Max wrong password attempts

/**********************
 * FLASH MEMORY ADDRESSES AND SIZES
 * Defines storage locations and buffer sizes
 **********************/
extern const unsigned long FLASH_STORAGE_ADDR;    // Main storage address
extern const unsigned long FLASH_DATE_START_ADDR; // Date storage start address
extern const unsigned long FLASH_ADDR_START;      // General storage start
extern const unsigned long FLASH_ADDR_END;        // Storage end address
extern const int FLASH_EEPROM_TOTAL_CHAR;         // EEPROM character count
extern const int FLASH_DATE_STORAGE_TOTAL_CHAR;   // Date storage character count
#define DATA_SIZE 100                             // General data buffer size
extern const int SSID_SIZE;                       // WiFi SSID buffer size
extern const int PASS_SIZE;                       // WiFi password buffer size
extern const int CURRENTADDR_SIZE;                // Current address buffer size
extern const int READADDR_SIZE;                   // Read address buffer size
extern const int COM_DATA_SIZE;                   // Communication data buffer size
extern const int PERVERSE_DATE_SIZE;              // Previous date buffer size
extern const int LAST_DATE_SIZE;                  // Last date buffer size
extern const int DATE_TIME_SIZE;                  // Date/time string buffer size
extern const int TOTAL_DAYS_SIZE;                 // Total days storage size
extern const int TOTAL_ADDR_SIZE;                 // Total address storage size
extern const int MAX_DAYS_STORAGE;                // Maximum days to store

/**********************
 * LOGICAL STATE CONSTANTS
 * Defines logical states for buttons and sensors
 **********************/
extern const int BUTTON_PRESSED;      // Button pressed state (active low)
extern const int BUTTON_NOT_PRESSED;  // Button not pressed state
extern const int PROXIMITY_SENSE;     // Proximity sensor active state
extern const int PROXIMITY_NOT_SENSE; // Proximity sensor inactive state

/**********************
 * NEXUS INTERFACE CONSTANTS
 * Defines constants for Nexus IoT interface communication
 **********************/
extern const int NEXUS_IN_ON_MINUTE;  // Nexus interface check interval
extern const int NEXUS_IN_ON_SECONDS; // Nexus interface seconds conversion
extern const int LORA_CONNECTED_BUT_NOT_RECEIVED;
extern const char *NEXUS_IN_ON_DATA;         // Nexus online status message
extern const char *NEXUS_NOT_REACHABLE_DATA; // Nexus unreachable message
extern const char *NEXUS_REACHABLE_DATA;     // Nexus reachable message
extern const char *NEXUS_IN_ALERT_DATA;      // Nexus alert status message
/**********************
 * COLOR DEFINITIONS
 * Defines color constants for LCD display
 **********************/
extern const int White;
extern const int Black;

/**********************
 * VOLTAGE CALCULATION CONSTANTS
 * Defines constants for voltage measurement and scaling
 **********************/
extern const float ADC_REFERENCE_VOLTAGE;     // More specific
extern const uint16_t ADC_MAX_VALUE;          // Remove .0 for integer
extern const float CURRENT_SENSITIVITY;       // mV/A or V/A
extern const uint8_t OVERLOAD_DEBOUNCE_COUNT; // More descriptive
extern const int ADC_AVERAGING_SAMPLES;

/**********************
 * VOLTAGE CALCULATION CONSTANTS
 * Defines constants for voltage measurement and scaling
 **********************/
extern const float VOLTAGE_ADC_MIN;    // Min ADC voltage reading
extern const float VOLTAGE_ADC_MAX;    // Max ADC voltage reading
extern const float SYSTEM_VOLTAGE_MIN; // Corresponding min system voltage
extern const float SYSTEM_VOLTAGE_MAX; // Corresponding max system voltage

extern const char COMMA;

/**********************
 * RESPONSE MESSAGE CONSTANTS
 * Defines standard response messages for system communication
 **********************/
struct ResponseVariable
{
    const String Ok = "OK";                                // Standard acknowledgment message
    const String Error = "ERROR";                          // Standard error message
    const String ControllerOnline = "*CONTROLLER ONLINE#"; // Controller online status message
    const String OfflineDataStart = "OFFLINE DATA START";  // Offline data transmission start message
    const String OfflineDataEnd = "OFFLINE DATA END";      // Offline data transmission end message
};
extern ResponseVariable Response; // Global instance of ResponseVariable

/**********************
 * PAYLOAD AND TOPIC CONSTANTS
 * Defines LoRa payloads and topics for IoT communication
 **********************/
struct PlayLoadTopicVariable
{
    const String App = "/APP";                              // Application configuration topic
    const String Alert = "/CON_ALERT";                      // Alert notification topic
    const String Offline = "/CON_OFFLINE";                  // Offline data topic
    const String FactoryReset = "/CON_FACTORYRESET";        // Factory reset command topic
    const String Refill = "/REFILL";                        // Refill completion topic
    const String RefillMode = "/CON_REFILLMODE";            // Refill mode control topic
    const String TestRun = "/TESTRUN";                      // Test run command topic
    const String TotalDischarge = "/TOTALDISCHARGE";        // Total discharge data topic
    const String LiveDataUpdate = "/LIVEDATAUPDATE";        // Live data update topic
    const String SleepMode = "/SLEEPMODE";                  // Sleep mode command topic
    const String Live = "/LIVE";                            // Live data request topic
    const String Reconfig = "/RECONFIG";                    // System reconfiguration topic
    const String AlertRectified = "/ALERTRECTIFIED";        // Alert rectified notification topic
    const String Tank = "/TANK";                            // Tank capacity topic
    const String Dashboard = "/DASHBOARD";                  // Dashboard data topic
    const String Snooze = "/SNOOZE";                        // Snooze alert command topic
    const String Setting = "/CON_SDP";                      // Setting configuration topic
    const String PumpStatus = "/PUMP";                      // Pump status update topic
    const String SupervisoryEnableDisable = "/SUPERVISORY"; // Supervisory level topic
    const String RefillAlert = "/REFILLALERT";              // Refill alert topic
    const String Mode = "/MODE";
    const String ConMode = "/CONMODE";
};
extern PlayLoadTopicVariable PlayLoadTopic; // Global instance of PlayLoadTopicVariable

/**********************
 * RETURN TOPIC CONSTANTS
 * Defines LoRa return topics for system acknowledgments
 **********************/
struct ReturnTopicVariables
{
    const String App = "/ACK_APP";                              // Application acknowledgment topic
    const String Alert = "/ACK_ALERT";                          // Alert acknowledgment topic
    const String Offline = "/ASK_OFFLINE";                      // Offline data acknowledgment topic
    const String FactoryReset = "/ACK_FACTORYRESET";            // Factory reset acknowledgment topic
    const String Refill = "/ACK_REFILL";                        // Refill completion acknowledgment topic
    const String RefillMode = "/ACKREFILLMODE";                 // Refill mode acknowledgment topic
    const String TestRun = "/ACK_TESTRUN";                      // Test run acknowledgment topic
    const String SleepMode = "/ASK_SLEEPMODE";                  // Sleep mode acknowledgment topic
    const String Live = "/ASK_LIVE";                            // Live data acknowledgment topic
    const String Reconfig = "/ASK_RECONFIG";                    // System reconfiguration acknowledgment topic
    const String AlertRectified = "/ASK_ALERTRECTIFIED";        // Alert rectified acknowledgment topic
    const String Snooze = "/ASK_SNOOZE";                        // Snooze alert acknowledgment topic
    const String Setting = "/ACK_CON_SDP";                      // Setting configuration acknowledgment topic
    const String PumpStatus = "/ACK_PUMP";                      // Pump status acknowledgment topic
    const String SupervisoryEnableDisable = "/ACK_SUPERVISORY"; // Supervisory level acknowledgment topic
    const String RefillAlert = "/ACK_REFILLALERT";              // Refill alert acknowledgment topic
    const String Dashboard = "/ACK_DASHBOARD";                  // Dashboard acknowledgment topic
    const String ConMode = "/ACK_CONMODE";
};
extern ReturnTopicVariables ReturnTopic; // Global instance of ReturnTopicVariables

/**********************
 * PAYLOAD MESSAGE CONSTANTS
 * Defines standard payload messages for LoRa communication
 **********************/
struct PlayLoadMessageVariable
{
    const String Ok = "OK";                                     // Standard acknowledgment message
    const String AlertRectified = "*ALERTRECTIFIED#";           // Alert rectified message
    const String FactoryReset = "*FR#";                         // Factory reset command message
    const String RefillSuccess = "*REFILL SUCCESS#";            // Refill success message
    const String RefillModeStart = "*RM-ST#";                   // Refill mode start message
    const String RefillModeStop = "*RM-SP#";                    // Refill mode stop message
    const String TestRunStart = "*TS-ST#";                      // Test run start message
    const String TestRunStop = "*TS-SP#";                       // Test run stop message
    const String SleepModeStart = "*ST#";                       // Sleep mode start message
    const String SleepModeStop = "*SP#";                        // Sleep mode stop message
    const String CheckAppOnline = "*LU-ST#";                    // Check app online message
    const String Snooze = "*SNOOZE_OVERIDE_RESERVOIRLOWLEVEL#"; // Snooze alert message
    const String Enable = "*ENABLE#";                           // Enable command message
    const String Disable = "*DISABLE#";                         // Disable command message
    const String LubricationStart = "LUB-ST";                   // Lubrication start message
    const String PauseTimeStart = "*PT-ST";                     // Lubrication
};
extern PlayLoadMessageVariable PlayLoadMessage; // Global instance of PlayLoadMessageVariable

/**********************
 * ALERT NAME CONSTANTS
 * Defines alert names for system notifications
 **********************/
struct AlertNameVariable
{
    const String Overload = "OVER_LOAD_ALERT";              // Overload alert name
    const String No = "NO";                                 // No alert name
    const String ReservoirLowLevel = "LOW_LEVEL_RESERVOIR"; // Reservoir low level alert name
    const String ProximityNotSense = "PROXIMITY_NOT_SENSE"; // Proximity not sensing alert name
    const String Supervisory_1 = "SUPERVISORY 1";           // Supervisory alert 1 name
    const String Supervisory_2 = "SUPERVISORY 2";           // Supervisory alert 2 name
    const String Supervisory_3 = "SUPERVISORY 3";           // Supervisory alert 3 name
    const String PumpWeak_1 = "PUMP WEAK 1";                // Pump weak alert 1 name
    const String PumpWeak_2 = "PUMP WEAK 2";                // Pump weak alert 2 name
    const String PumpWeak_3 = "PUMP WEAK 3";                // Pump weak alert 3 name
};
extern AlertNameVariable AlertName; // Global instance of AlertNameVariable

/**********************
 * SUPERVISORY LEVEL CONSTANTS
 * Defines supervisory level thresholds for system monitoring
 **********************/
extern const int SUPERVISORY_AVERAGE_GRAM;       // Average supervisory level in grams
extern const int SUPERVISORY_GREASE_SETTLING;    // Initial Grease settling supervisory level in grams
extern const int SUPERVISORY_NOMINAL_GRAM_INIT;  // Initial nominal supervisory level in grams
extern const int SUPERVISORY_ALERT_CHECK_GRAM;   // Gram threshold for supervisory alert checks
extern const float SUPERVISORY_NORMINAL_AVERAGE; // Nominal average for supervisory mode

extern const int LUB_DISCHARGE; // Base discharge amount used for lubricant output calculations

extern const int TEST_RUN_MIN; // Test-run duration minutes component
extern const int TEST_RUN_SEC; // Test-run duration seconds component

extern const int SUB_DELAY; // Delay between successive subscription/communication retries

extern const String LORA_CONFIG; // LoRa configuration string

#define PUMP_GOOD 1    // Good supervisory level
#define PUMP_AVERAGE 2 // Average supervisory level
#define PUMP_BAD 3     // Bad supervisory level

#define RESERVOIR_READ_STROCK 5   // Number of lubrication cycles before reading reservoir level
#define LUBRICATION_READ_STROCK 5 // Number of lubrication cycles before performing lubrication process calculations

/**********************
 * CROSS-TASK STATE GUARD
 * Short spinlock used to make grouped writes of shared mode/alert flags atomic
 * with respect to other tasks. Keep the guarded region tiny: flag assignments
 * only - never call Serial, delay(), flash writes or motor I/O inside it.
 **********************/
extern portMUX_TYPE stateMux;
#define STATE_LOCK() taskENTER_CRITICAL(&stateMux)
#define STATE_UNLOCK() taskEXIT_CRITICAL(&stateMux)

#endif