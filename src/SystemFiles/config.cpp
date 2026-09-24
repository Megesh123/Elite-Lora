/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note SYSTEM CONFIGURATION CONSTANTS
 */

#include "Headerfile.h"

/**********************
 * SYSTEM CONFIGURATION CONSTANTS
 * Centralized configuration for easy maintenance
 **********************/

/**********************
 * PIN CONFIGURATION
 * Defines all hardware pin assignments
 **********************/

/**********************
 * SENSOR PINS
 * Input devices and sensor interfaces
 **********************/
const int PIN_PROXIMITY_SENSOR = 1; // Proximity sensor digital input
const int PIN_VOLTAGE_SENSOR = 4;   // Voltage sensor analog input
const int PIN_CURRENT_SENSOR = 5;   // Current sensor analog input

/**********************
 * OUTPUT DEVICE PINS
 * Actuators and output interfaces
 **********************/
const int PIN_PUMP_MOTOR = 6; // Pump motor PWM control
const int PIN_BUZZER = 7;     // Buzzer audio output

/**********************
 * BUTTON PINS
 * User input buttons for navigation and control
 **********************/
const int PIN_LIMIT_SW_1 = 18; // Limit switch 1 input
const int PIN_LIMIT_SW_2 = 19; // Limit switch 2 input
const int PIN_LIMIT_SW_3 = 20; // Limit switch 3 input
const int PIN_SW_MENU = 21;    // Menu button input
const int PIN_SW_BACK = 47;    // Back button input
const int PIN_SW_UP = 48;      // Up button input
const int PIN_SW_DOWN = 38;    // Down button input
const int PIN_SW_OK = 39;      // OK button input

/**********************
 * LED PINS
 * Status indicator LEDs
 **********************/
const int PIN_LED_SLEEP_FAULT = 42;     // Sleep/Fault status LED
const int PIN_LED_OVERLOAD_REFILL = 41; // Overload/Refill status LED
const int PIN_LED_TEST_RUN = 40;        // Test Run status LED
const int PIN_LED_COMMUNICATION = 2;    // Communication status LED

/**********************
 *  I2C PINS
 * I2C communication interface
 **********************/
const int PIN_I2C_SDA = 8; // I2C data line
const int PIN_I2C_SCL = 9; // I2C clock line

/**********************
 * SERIAL COMMUNICATION PINS
 * Serial interface for debugging and external communication
 **********************/

/**********************
 * SERIAL MONITOR CONFIGURATION
 * Serial communication settings
 **********************/
const int SERIAL_BAUD_RATE = 115200; // Serial monitor baud rate

/**********************
 * BLE CONFIGURATION
 * Bluetooth Low Energy settings
 **********************/
const char *BLE_SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *BLE_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

/**********************
 * MOTOR CONTROL CONFIGURATION
 * Motor operation parameters
 **********************/
const int PUMP_SPEED_FULL = 0;       // Full speed (0% duty cycle - depends on driver)
const int PUMP_SPEED_OFF = 255;      // Motor off (100% duty cycle - depends on driver)
const int PUMP_SPEED_MEDIUM = 192;   // Medium speed (~25% duty cycle)
const int PUMP_FREQUENCY_HZ = 25000; // PWM frequency for motor control

/**********************
 * VOLTAGE MONITORING THRESHOLDS
 * Voltage levels for system operation and safety
 **********************/
const float VOLTAGE_12V_SYSTEM_MIN = 9.0f;  // Minimum detection voltage for 12V systems
const float VOLTAGE_12V_SYSTEM_MAX = 15.0f; // Maximum detection voltage for 12V systems
const float VOLTAGE_12V_THRESHOLD = 9.0f;   // Low voltage shutdown threshold for 12V
const float VOLTAGE_24V_THRESHOLD = 9.0f;   // Low voltage shutdown threshold for 24V

/**********************
 * TIMING CONFIGURATION
 * System timing parameters for operations and safety
 **********************/

/**********************
 * PROXIMITY SENSOR AND OVERLOAD TIMING
 * Timing parameters for sensor detection and overload conditions
 **********************/
const unsigned long PROXIMITY_TIMEOUT_SEC = 10;        // Proximity sensor detection timeout
const unsigned long OVERLOAD_CHECK_INTERVAL_MS = 1000; // Current monitoring interval

/**********************
 * APP ONLINE/OFFLINE CHECK
 * Handles application online status checks via LoRa
 **********************/
const unsigned long APP_ONLINE_TIMEOUT_SEC = 15; // Application online status timeout

/**********************
 * OVERLOAD CONDITION DURATION
 * Duration to confirm overload before triggering alert
 **********************/
const unsigned long OVER_LOAD_TIME_MS = 5000; // Overload condition duration

/**********************
 * RESERVOIR LOW LEVEL ALERT
 * Duration to confirm low reservoir level before triggering alert
 **********************/

/**********************
 * RESERVOIR LOW LEVEL ALERT
 * Threshold percentage for low reservoir level alert
 **********************/
const float RESERVOIR_LOW_LEVEL_PERCENT = 1.0f; // Reservoir low level alert threshold

/**********************
 * SYSTEM PASSWORD CONFIGURATION
 * Default password and security settings
 **********************/
const char *SYSTEM_PASSWORD = "336699";    // Default system access password
const int WRONG_PASSWORD_MAX_ATTEMPTS = 3; // Maximum wrong password attempts

/**********************
 * FLASH MEMORY CONFIGURATION
 * Flash memory addresses and sizes
 **********************/

/**********************
 * FLASH MEMORY ADDRESSES
 * Defines start and end addresses for flash storage
 **********************/
const unsigned long FLASH_STORAGE_ADDR = 0x820000;    // Main storage address
const unsigned long FLASH_DATE_START_ADDR = 0x821000; // Date storage start address
const unsigned long FLASH_ADDR_START = 0x840000;      // General storage start
const unsigned long FLASH_ADDR_END = 0xFF0000;        // Storage end address

/**********************
 * FLASH MEMORY SIZES
 * Defines sizes for flash memory segments
 **********************/
const int FLASH_EEPROM_TOTAL_CHAR = 1000;       // EEPROM emulation character count
const int FLASH_DATE_STORAGE_TOTAL_CHAR = 2000; // Date storage character count

/**********************
 * BUFFER SIZES
 * Defines sizes for various data buffers
 **********************/

const int SSID_SIZE = 50;          // WiFi SSID buffer size
const int PASS_SIZE = 50;          // WiFi password buffer size
const int CURRENTADDR_SIZE = 20;   // Current address buffer size
const int READADDR_SIZE = 20;      // Read address buffer size
const int COM_DATA_SIZE = 10;      // Communication data buffer size
const int PERVERSE_DATE_SIZE = 20; // Previous date buffer size
const int LAST_DATE_SIZE = 20;     // Last date buffer size
const int DATE_TIME_SIZE = 32;     // Date/time char* buffer size
const int TOTAL_DAYS_SIZE = 1000;  // Total days storage size
const int TOTAL_ADDR_SIZE = 1000;  // Total address storage size

/**********************
 * MAX DAYS STORAGE
 * Maximum number of days to store in flash memory
 **********************/
const int MAX_DAYS_STORAGE = 90; // Maximum days to store in flash

/**********************
 * BUTTON AND SENSOR STATES
 * Defines active and inactive states for buttons and sensors
 **********************/
const int BUTTON_PRESSED = 0;      // Button pressed state (active low)
const int BUTTON_NOT_PRESSED = 1;  // Button not pressed state
const int PROXIMITY_SENSE = 0;     // Button pressed state (active low)
const int PROXIMITY_NOT_SENSE = 1; // Button not pressed state

/**********************
 * NEXUS INTERFACE CHECK INTERVAL
 * Timing for checking Nexus interface connectivity
 **********************/
const int NEXUS_IN_ON_MINUTE = 1;   // Nexus interface check interval (minutes)
const int NEXUS_IN_ON_SECONDS = 10; // Nexus interface seconds conversionconst
const int LORA_CONNECTED_BUT_NOT_RECEIVED = 70;

/**********************
 * NEXUS INTERFACE STATUS MESSAGES
 * Predefined LoRa messages for Nexus interface status
 **********************/
const char *NEXUS_IN_ON_DATA = "*NEXUS IN ON, NEXUS IN NORMAL#";
const char *NEXUS_IN_ALERT_DATA = "*NEXUS IN ON, NEXUS IN ALERT#";
const char *NEXUS_NOT_REACHABLE_DATA = "*NEXUS IN ON, NEXUS NOT REACHABLE#";
const char *NEXUS_REACHABLE_DATA = "*NEXUS IN ON, NEXUS REACHABLE#";

/**********************
 * DISPLAY COLOR DEFINITIONS
 * Color codes for display interface
 **********************/
const int White = 1;
const int Black = 0;

/**********************
 * CURRENT SENSOR CALIBRATION CONSTANTS
 * Calibration parameters for accurate current readings
 **********************/
const float ADC_REFERENCE_VOLTAGE = 3.3f;  // More specific
const uint16_t ADC_MAX_VALUE = 4095;       // Remove .0 for integer
const float CURRENT_SENSITIVITY = 0.044f;  // mV/A or V/A
const uint8_t OVERLOAD_DEBOUNCE_COUNT = 5; // More descriptive
const int ADC_AVERAGING_SAMPLES = 10;

/**********************
 * VOLTAGE SENSOR CALIBRATION CONSTANTS
 * Calibration parameters for accurate voltage readings
 **********************/
const float VOLTAGE_ADC_MIN = 1.26f;    // Min ADC voltage reading
const float VOLTAGE_ADC_MAX = 3.27f;    // Max ADC voltage reading
const float SYSTEM_VOLTAGE_MIN = 12.0f; // Corresponding min system voltage
const float SYSTEM_VOLTAGE_MAX = 30.0f; // Corresponding max system voltage

const char COMMA = ',';

/**********************
 * STRUCTURE INSTANTIATIONS
 * Global instances of configuration structures
 **********************/
PlayLoadTopicVariable PlayLoadTopic;
ResponseVariable Response;
ReturnTopicVariables ReturnTopic;
PlayLoadMessageVariable PlayLoadMessage;
AlertNameVariable AlertName;

/**********************
 * SUPERVISORY AVERAGE GRAM
 * Average gram threshold for supervisory mode
 **********************/
const int SUPERVISORY_AVERAGE_GRAM = 500;
const int SUPERVISORY_GREASE_SETTLING = 4000;
const int SUPERVISORY_NOMINAL_GRAM_INIT = 4000;
const int SUPERVISORY_ALERT_CHECK_GRAM = 3;
const float SUPERVISORY_NORMINAL_AVERAGE = 8.0f; // Nominal average for supervisory mode

const int LUB_DISCHARGE = 1;

const int TEST_RUN_MIN = 10;
const int TEST_RUN_SEC = 0;

const String LORA_CONFIG = "lora,"; // LoRa configuration string
