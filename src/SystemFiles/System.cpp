/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note SYSTEM CONFIGURATION IMPLEMENTATION
 */

/**********************
 * LUBRICATION CONTROL SYSTEM
 * Main controller for automated lubrication system with IoT capabilities
 * Features: Bluetooth configuration, LoRaWAN connectivity, pressure monitoring,
 * grease dispensing control, reservoir management, and alert system
 **********************/

#include "Headerfile.h"
#include "esp_heap_caps.h"

/**********************
 * LCD DISPLAY INITIALIZATION
 * Sets up ST7565 LCD with specified control pins
 **********************/
ST7565_LCD display = ST7565_LCD(LCD_A0, LCD_RESET, LCD_CS);

/**********************
 * GLOBAL OBJECT INSTANTIATIONS
 * Create instances of system function classes for core operations
 **********************/
SYSTEMFUNCTION System;                 // System core functions
MCP7940 rtc;                           // Real-time clock instance
ALERTFUNCTION AlertFun;                // Alert management
BLUETOOTHPROCESSFUNCTION BluetoothFun; // Bluetooth processing
CURRENTFUNCTION Current;               // Current monitoring
EEPROMFUNCTION Eeprom;                 // EEPROM emulation
FLASHSTOREFUNCTION Flash;              // Flash storage management
GREASE_PROCESS_FUNCTION Grease;        // Grease dispensing process
LEDFUNCTION Led;                       // LED status indicators
MOTORFUNCTION Motor;                   // Motor pump control
LoRaFunction LoRa;                     // LoRa communication
RESERVOIRFUNCTION Reservoir;           // Reservoir level monitoring
// SERIALMONITOR serial;                  // Serial monitor interface
TaskFunction Task;                   // Task management
TimerFunction Timer;                 // Timer system
VOLTAGEFUNCTION Voltage;             // Voltage monitoring
SupervisoryVariable SupervisoryTime; // Supervisory mode variables
TotalAlertVariable TotalAlert;       // Total alert counters
KeyLockFunction KeyLock;
SLEEPMODE SLeepMode;
LcdSleepModeFunction Sleepmode;
LcdKeyLockFunction LcdKeyLock;
SignalBarFunction SignalBar; // Global instance of SignalBarFunction

/**********************
 * GLOBAL VARIABLE DEFINITIONS
 * Define global structures for system state and configuration
 **********************/
LedStatus LedFun;                              // LED status flags
LoraDetailsVariables LoraDetails;              // LoRa connection details
LoraFunVariables LoraFun;                      // LoRa functional flags
LoraTopicsVariable LoraTopics;                 // LoRa topic strings
NexuxInAlertVariable NexuxInAlert;             // Nexus input alert flags
bluetoothVariable bluetooth;                   // Bluetooth configuration flags
GreaseOutVariable GreaseOut;                   // Grease dispensing variables
RTCVariable RTC;                               // Real-time clock variables
AlertVariable Alert;                           // Alert system variables
FlashVariable flash;                           // Flash memory operation variables
TotalOperationTimeVariable TotalOperationTime; // Total operation time tracking
SerialFunVariable SerialFun;                   //  Serial communication variables
CurrentFunVariable CurrentMonitor;             // Current monitoring variables
VoltageFunVariable VoltageMonitor;             // Voltage monitoring variables
ConfigFunVariable Config;                      // System configuration variables
NexuxInVariable NexuxIn;                       // Nexus input monitoring variables
LoraTimeVariable LoraTime;                     // LoRa timing variables
SemaphoreHandle_t lcdMutex;                    // Mutex for LCD access synchronization
ReceivedVariable Received;                     // Received message tracking variables
AdminSettingVariables AdminSetting;
AboutDevicesVariable AboutDevices;
OTAVariable OTA; // OTA update variables

SemaphoreHandle_t adcMutex; // Mutex for thread-safe ADC access

portMUX_TYPE stateMux = portMUX_INITIALIZER_UNLOCKED; // Guards grouped writes of shared mode/alert flags

/**********************
 * SYSTEM BEGIN FUNCTION
 * Initializes hardware pins, motor, and Bluetooth mode detection
 **********************/

void SYSTEMFUNCTION::begin()
{
  /**********************
   * HARDWARE PIN SETUP
   * Configure GPIO pins for LEDs, sensors, buttons, and motor control
   **********************/
  // Initialize LED output pins for status indicators
  pinMode(PIN_LED_SLEEP_FAULT, OUTPUT);     // Sleep/Fault LED
  pinMode(PIN_LED_OVERLOAD_REFILL, OUTPUT); // Overload/Refill LED
  pinMode(PIN_LED_TEST_RUN, OUTPUT);        // Test Run LED
  pinMode(PIN_LED_COMMUNICATION, OUTPUT);   // Communication LED

  // Initialize sensor input pins for proximity and user interface
  pinMode(PIN_PROXIMITY_SENSOR, INPUT); // Proximity sensor input

  pinMode(PIN_SW_MENU, INPUT_PULLUP); // Menu button input
  pinMode(PIN_SW_BACK, INPUT_PULLUP); // Back button input
  pinMode(PIN_SW_UP, INPUT_PULLUP);   // Up button input
  pinMode(PIN_SW_DOWN, INPUT_PULLUP); // Down button input
  pinMode(PIN_SW_OK, INPUT_PULLUP);   // OK button input

  // Initialize limit switch input pins for safety controls
  pinMode(PIN_LIMIT_SW_1, INPUT); // Limit switch 1 input
  pinMode(PIN_LIMIT_SW_2, INPUT); // Limit switch 2 input
  pinMode(PIN_LIMIT_SW_3, INPUT); // Limit switch 3 input

  // Initialize analog sensor input pins for voltage and current monitoring
  pinMode(PIN_VOLTAGE_SENSOR, INPUT); // Voltage sensor input
  pinMode(PIN_CURRENT_SENSOR, INPUT); // Current sensor input

  /**********************
   * MOTOR INITIALIZATION
   * Set up motor control pin with high-frequency PWM
   **********************/
  analogWriteFrequency(PUMP_FREQUENCY_HZ); // Set high-frequency PWM for motor control
  pinMode(PIN_PUMP_MOTOR, OUTPUT);         // Motor pump control pin
  pinMode(PIN_BUZZER, OUTPUT);             // Buzzer audio output pin
  Motor.off();                             // Ensure motor starts in off state

  /**********************
   * BLUETOOTH MODE DETECTION
   * Check if Bluetooth configuration mode should be activated
   **********************/
  if (Button.menu.isPressed() || bluetooth.ModeEnable)
  {
    Serial.println("=====================================");
    Serial.println("Bluetooth Mode Activated");
    Serial.println("=====================================");
    bluetooth.ModeCheck = true; // Enable Bluetooth configuration mode
    Led.on();                   // Turn on LED to indicate Bluetooth mode
    // Log.debug("Button pressed"); // Debug log for button press
  }
  else
  {
    Serial.println("=====================================");
    Serial.println("Normal Operation Mode");
    Serial.println("=====================================");
    bluetooth.ModeCheck = false; // Normal operation mode
    TestingModeEnable = false;
  }
  adcMutex = xSemaphoreCreateMutex(); // Protect ADC read access in multitasking
}

/**********************
 * SYSTEM INIT FUNCTION
 * Comprehensive system initialization including memory, peripherals, and modes
 **********************/
void SYSTEMFUNCTION::init()
{
  /**********************
   * SYSTEM BEGIN INITIALIZATION
   * Initialize hardware pins and motor control
   **********************/
  System.begin(); // Initialize hardware pins and motor

  /**********************
   * EEPROM AND FLASH INITIALIZATION
   * Set up EEPROM emulation and flash storage for configuration data
   **********************/
  Eeprom.begin(); // Initialize EEPROM emulation

  Eeprom.read(); // Read configuration data from EEPROM

  Eeprom.addressRead(); // Read flash memory address information

  Eeprom.init(); // Initialize EEPROM with default values if needed

  /**********************
   * INPUT DEVICE INITIALIZATION
   * Set up voltage and current monitoring systems
   **********************/
  Voltage.begin(); // Initialize voltage monitoring

  Current.begin(); // Initialize current monitoring

  LoRa.playloadTopic(); // Set up LoRa topic structure

  LoRa.returnTopic(); // Set up LoRa return topics

  /**********************
   * RTC AND LCD INITIALIZATION
   * Configure real-time clock and initialize LCD display
   **********************/
  rtc.begin(); // Initialize RTC module

  /**********************
   * LCD MUTEX CREATION
   * Create mutex for synchronized access to LCD display
   **********************/
  LcdDisplay.Begin(); // Initialize LCD display

  /**********************
   * MODE SELECTION AND INITIALIZATION
   * Determine operation mode (Bluetooth configuration or normal) and initialize accordingly
   **********************/
  if (bluetooth.ModeCheck || bluetooth.ModeEnable)
  {
    bluetooth.ModeCheck = true;
    bluetooth.ModeEnable = false;
    Serial.println("=====================================");
    Serial.println("Bluetooth Mode Activated - running Bluetooth configuration");
    Serial.println("=====================================");
    bluetooth.Enable = true;                    // Enable Bluetooth configuration mode
    LcdUI.Bluetooth.Function();                 // Display Bluetooth configuration UI
    Bluetooth.enable();                         // Enable Bluetooth module
    delay(100);                                 // Wait for Bluetooth to initialize
    bluetooth.Enable = false;                   // Disable initial enable flag
    bluetooth.Enabled = true;                   // Mark Bluetooth as enabled
    bluetooth.Connecting = true;                // Mark Bluetooth as connecting
    LoraDetails.Clientid = Bluetooth.getName(); // Get Bluetooth device name for LoRa client ID
    Led.on();                                   // Turn on LED to indicate Bluetooth mode
  }
  else
  {
    LcdUI.Logo();     // Display system logo on LCD
    Led.off();        // Turn off configuration indicator LED
    Timer.begin();    // Start system timers
    AlertFun.begin(); // Initialize alert system
    Motor.begin();    // Initialize motor control system
    Grease.begin();
    GreaseOut.Reservoir.Enable = false; // Ensure reservoir mode is disabled on initialization
  }

  /**********************
   * TASK CREATION
   * Create FreeRTOS tasks for parallel system operations
   **********************/

  Task.create(); // Create background tasks for network communication

  // Serial.printf(
  //     "[MEM] Free Heap=%u Largest=%u\n",
  //     ESP.getFreeHeap(),
  //     ESP.getMaxAllocHeap());
  /**********************
   * SYSTEM RESOURCE LOGGING
   * Log available system resources for debugging
   **********************/
  // Log.info("Heap free: %u bytes", ESP.getFreeHeap());                                                    // Log free heap memory
  // Log.info("Min free heap: %u bytes", ESP.getMinFreeHeap());                                             // Log minimum free heap memory
  // Log.info("Task %s high water mark: %u bytes", pcTaskGetName(NULL), uxTaskGetStackHighWaterMark(NULL)); // Log task stack high water mark
  // Log.info("Internal RAM free: %u bytes", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  // Log.info("Internal RAM largest block: %u bytes", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
}

bool Keylock = false;       // Keylock status flag
bool TestrunPage = false;   // Test run page status flag
bool TestRunEnable = false; // Test run mode enable flag

bool MainEnable = true;               // Main system enable flag
bool MenuEnable = false;              // Menu system enable flag
bool MenuSettingEditTrue = false;     // Menu setting edit mode flag
bool MenuResetnotCompleted = false;   // Menu reset not completed flag
bool TestRunEnableError = false;      // Test run enable error flag
bool KeylockEnable = false;           // Keylock enable flag
bool KeylockDisable = false;          // Keylock disable flag
unsigned long ArrowlastBlinkTime = 0; // Last time the arrow blinked
bool ArrowblinkState = false;         // Arrow blink state
char MenuFunSetcursor = 12;           // Menu function set cursor position
char ConfigFunSetcursor = 12;         // Configuration function set cursor position
char SettingFunSetcursor = 12;        // Setting function set cursor position
char RefillFunSetcursor = 12;         // Refill function set cursor position
char AboutDeviceSetcursor = 12;       // About device set cursor position
char SystemSetcursor = 12;
bool ConfigEnable = false;                          // Configuration mode enable flag
bool SettingEnable = false;                         // Setting mode enable flag
bool RefillEnable = false;                          // Refill mode enable flag
bool RefillModeEnable = false;                      // Refill mode enable flag
bool RefillConfirmation = false;                    // Refill confirmation flag
bool RefillConfirmationYesNo = false;               // Refill confirmation yes/no flag
bool RefillConfirmationYesNoBlink = false;          // Refill confirmation yes/no blink flag
bool RefillStart = false;                           // Refill start flag
bool RefillStop = false;                            // Refill stop flag
bool RefillCompleted = false;                       // Refill completed flag
String ConfigSelectVariable = "";                   // Configuration selection variable
String SettingSelectVariable = "";                  // Setting selection variable
bool VariableSeleteEnable = false;                  // Variable select enable flag
bool TankBlink = false;                             // Tank level blink flag
bool OutletBlink = false;                           // Outlet blink flag
bool LubBlink = false;                              // Lubrication blink flag
bool PtmHourBlink = false;                          // Ptm hour blink flag
bool PtmMinBlink = false;                           // Ptm minute blink flag
bool supMinBlink[4] = {false, false, false, false}; // Supervisory minute blink flags
int scrollOffset = 0;                               // Scroll offset for menu display
int totalItems = 17;                                // Total number of menu items
int itemHeight = 12;                                // Height of each menu item
int maxVisibleItems = 5;                            // Maximum number of visible menu items
int Space = 12;                                     // Space between menu items
float TankCapcity = 0;                              // Tank capacity value
float TempTankCapcity = 0;                          // Temporary tank capacity value
int capcity;                                        // Capacity value
unsigned long lastBlinkTime = 0;                    // Last blink time for reset confirmation
bool blinkState = false;                            // Blink state for reset confirmation
bool ResetEnable = false;                           // Reset enable flag
bool Reset = false;                                 // Reset flag
bool ResetCompleted = false;                        // Reset completed flag
bool ResetAllValueSet = false;                      // Reset all value set flag
bool Reset_SendData_Lora;                           // Reset send data via LoRa flag
bool Reset_sendData_Lora_Subscribe = false;         // Reset send data via LoRa subscribe flag

int start_init;          // Start initialization flag
int oldStateCount;       // Old state count for button debouncing
int MenuSetting_sec = 0; // Menu setting seconds counter

bool MainEnableWithReset = false;           // Main enable with reset flag
bool MenuSettingMotorOff = false;           // Menu setting motor off flag
bool MenuSettingEnableDisableCheck = false; // Menu setting enable/disable check flag
int speedcount = 0;                         // Speed count for motor control
bool speedEnable = false;                   // Speed enable flag
int speed = 0;                              // Speed value for motor control

float _pump1_percentage; // Current percentage of pump 1 output
float _pump2_percentage; // Current percentage of pump 2 output
float _pump3_percentage; // Current percentage of pump 3 output

String _pump1_condtion; // Current condition of pump 1
String _pump2_condtion; // Current condition of pump 2
String _pump3_condtion; // Current condition of pump 3

int SleepModeEnableCount = 0;
int SleepModeDisableCount = 0;

int Lora_disconnected_count = 0;
bool _Controller_online_data_send = false; // Flag to track if data has been sent
bool ReconfigEnableRefillNeed = false;
