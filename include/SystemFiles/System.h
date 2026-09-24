/**********************
 * SYSTEM CONFIGURATION HEADER
 * Contains all pin definitions, constants, and external declarations
 * for the lubrication control system
 **********************/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "Arduino.h"
#include "MCP7940.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <ST7565_LCD.h>
#include <freertos/semphr.h>

/**********************
 * LCD DISPLAY INSTANCE
 * Global instance of ST7565_LCD for LCD management
 **********************/
extern ST7565_LCD display; // Global LCD display instance
extern SemaphoreHandle_t SerialMutex;

#define LCD_A0 13    // Command/Data pin
#define LCD_RESET 14 // Reset pin
#define LCD_CS 10    // Chip Select pin

#define OUTLET_DISCHARGE 0.2   // Discharge rate per outlet in grams per second
#define OVERLOAD_THRESHOLD 3.0 // Overload current threshold in amps

#define CURRENT_VERSION_LINK_ULTRA_ELITE "1.21" // Current firmware version for Link Ultra Elite
/**********************
 * SYSTEMFUNCTION CLASS
 * Manages overall system initialization and configuration
 **********************/
class SYSTEMFUNCTION
{
public:
    void begin(); // System initialization routine
    void init();  // System configuration setup
};
extern SYSTEMFUNCTION System;         // Global instance of SYSTEMFUNCTION
extern MCP7940 rtc;                   // Global instance of MCP7940 RTC
extern const char *rootCACertificate; // Root CA certificate for secure communication

/**********************
 * LED STATUS STRUCTURE
 * Structure for tracking LED status indicators
 **********************/
struct LedStatus
{
    int state;
};
extern LedStatus LedFun; // Global instance of LedStatus

/**********************
 * TOTAL OPERATION TIME STRUCTURE
 * Structure for tracking total operation time of the system
 **********************/
struct TotalOperationTimeVariable
{
    int Seconds; // Total run time in seconds
    int Minute;  // Total run time in minutes
    int Hour;    // Total run time in hours
};
extern TotalOperationTimeVariable TotalOperationTime; // Global instance of TotalOperationTimeVariable

/**********************
 * LORA DETAILS STRUCTURE
 * Structure for storing LoRaWAN connection parameters
 **********************/
#define LORA_WAIT_AFTER_CONNECT 10
#define HALF_HOUR 1800
// LoRaWAN can take several seconds to deliver the uplink and its downlink response.
// This is the maximum time to wait *after a dashboard message was sent*.
struct LoraDetailsVariables
{
    String Wifi;                       // WiFi SSID for network connection
    String Pass;                       // WiFi password for network connection
    String DEVEUI;                     // LoRaWAN Device EUI (received via Bluetooth config)
    String APPKEY;                     // LoRaWAN App Key (received via Bluetooth config)
    String APPEUI;                     // LoRaWAN App EUI (received via Bluetooth config)
    String Clientid;                   // BLE client identifier
    int port;                          // Reserved network port (unused)
    bool UserEnterOfflineMode = false; // Flag for user-initiated offline mode
    bool Connected = false;            // LoRaWAN connection status
    int _wait_after_connected = 0;
    long Seconds_after_connected = 0;
    int lora_connected_but_not_received = 0;
    bool DashboardAckPending = false; // Start the dashboard ACK timer only after publishing DASHBOARD
};
extern LoraDetailsVariables LoraDetails; // Global instance of LoraDetailsVariables

/**********************
 * LORA FUNCTION VARIABLES STRUCTURE
 * Structure for managing LoRa function variables
 **********************/
struct LoraFunVariables
{
    int AppOnlineSec = 20; // Application online check interval in seconds
    bool DataReceived;     // Flag for data reception status
};
extern LoraFunVariables LoraFun; // Global instance of LoraFunVariables

/**********************
 * LoRa TOPICS STRUCTURE
 * Structure for storing LoRa topic strings
 **********************/
struct LoraTopicsVariable
{
    String subscribe;        // Main subscription topic
    String subscribe_return; // Return topic for subscriptions

    String alert;        // Topic for alert notifications
    String alert_return; // Return topic for alerts

    String alert_clear;        // Topic for alert clearance
    String alert_clear_return; // Return topic for alert clearance

    String offline;        // Topic for offline status
    String offline_return; // Return topic for offline status

    String system_reset;        // Topic for system reset commands
    String system_reset_return; // Return topic for system reset

    String refill;        // Topic for refill operations
    String refill_return; // Return topic for refill operations

    String refill_mode;        // Topic for refill mode settings
    String refill_mode_return; // Return topic for refill mode

    String testrun;        // Topic for test run commands
    String testrun_return; // Return topic for test runs

    String Reservoircapacity;        // Topic for reservoir capacity
    String Reservoircapacity_return; // Return topic for capacity

    String LiveData; // Topic for live data streaming

    String SleepMode;        // Topic for sleep mode settings
    String SleepMode_return; // Return topic for sleep mode

    String check_app_online; // Topic for app online status check

    String LiveController;        // Topic for live controller data
    String LiveController_return; // Return topic for controller data

    String Reconfig;        // Topic for reconfiguration commands
    String Reconfig_return; // Return topic for reconfiguration

    String Dashboard;        // Topic for dashboard data
    String Dashboard_return; // Return topic for dashboard data

    String Snooze;        // Topic for snooze commands
    String Snooze_return; // Return topic for snooze commands

    String Setting;        // Topic for settings
    String Setting_return; // Return topic for settings

    String PumpStatus;        // Topic for pump status updates
    String PumpStatus_return; // Return topic for pump status

    String SupervisoryEnableDisable;        // Topic for supervisory level settings
    String SupervisoryEnableDisable_return; // Return topic for supervisory level settings

    String RefillAlert;        // Topic for refill alert notifications
    String RefillAlert_return; // Return topic for refill alert notifications

    String RefillAlertEnableDisable;
    String RefillAlertEnableDisable_return;

    String OTA;

    String Mode;

    String ConMode;
    String ConMode_return;

    String LiveMode;
};
extern LoraTopicsVariable LoraTopics; // Global instance of LoraTopicsVariable

/**********************
 * LoRa TIMING VARIABLES STRUCTURE
 * Structure for managing LoRa timing and intervals
 **********************/
struct LoraTimeVariable
{
    uint32_t ControllerOnlineCheckTime = 0;                  // Timer for controller online check
    const uint32_t ControllerOnlineCheckTimeInterval = 5000; // Interval for online checks (5 sec)
    bool datasend = false;                                   // Flag for data transmission status
    uint32_t OfflineCheckTime = 0;                           // Timer for offline status checking
    const uint32_t OfflineCheckInterval = 20000;             // Interval for offline checks (20 sec)
    bool EnableForOnline;                                    // Flag to enable online status checking
};
extern LoraTimeVariable LoraTime; // Global instance of LoraTimeVariable

/**********************
 * NEXUS INPUT VARIABLES STRUCTURE
 * Structure for managing Nexus input settings
 **********************/
struct NexuxInVariable
{
    bool Enable = true; // Nexus input enable flag
    int Seconds = 0;    // Nexus input timing in seconds
};
extern NexuxInVariable NexuxIn; // Global instance of NexuxInVariable

/**********************
 * NEXUS INPUT ALERT MESSAGES STRUCTURE
 * Structure for storing Nexus input alert message strings
 **********************/
struct NexuxInAlertVariable
{
    String OVERLOAD = "*NEXUS IN ON, NEXUS IN ALERT,OVERLOAD#";                       // Overload alert message
    String SND = "*NEXUS IN ON, NEXUS IN ALERT,SIGNAL NOT DETECTED#";                 // Signal not detected alert message
    String SUPERVISORY = "*NEXUS IN ON, NEXUS IN ALERT,SUPERVISORY#";                 // Supervisory  alert message
    String PUMP_WEAK = "*NEXUS IN ON, NEXUS IN ALERT,PUMP WEAK#";                     // Pump weak alert message
    String RESERVOIR_LOW_LEVEL = "*NEXUS IN ON, NEXUS IN ALERT,RESERVOIR LOW LEVEL#"; // Reservoir low level alert message
};
extern NexuxInAlertVariable NexuxInAlert; // Global instance of NexuxInAlertVariable

/**********************
 * BLUETOOTH VARIABLES STRUCTURE
 * Structure for managing Bluetooth communication and status
 **********************/
struct bluetoothVariable
{
    int Received = 0;         // Count of received Bluetooth data packets
    bool ModeCheck;           // Flag indicating Bluetooth mode is active
    bool ModeEnable;          // Flag indicating Bluetooth mode is enabled
    String com_data;          // Data received via Bluetooth
    bool Mode = false;        // Current Bluetooth mode status
    String incomingData = ""; // Buffer for incoming Bluetooth data
    uint64_t chipId;          // Unique chip ID for device identification
    bool DataReceived;        // Flag for Bluetooth data reception status

    bool Enable = false;               // Bluetooth functionality enabled flag
    bool Enabled = false;              // Bluetooth functionality active flag
    bool Connecting = false;           // Bluetooth connection in progress flag
    bool Connected = false;            // Bluetooth connected status flag
    bool IncomingDataReceive = false;  // Flag for incoming Bluetooth data reception
    bool IncomingDataReceived = false; // Flag for completed Bluetooth data reception

    const String SendSuccess = "SUCCESS";
    const String SendError = "ERROR";
};
extern bluetoothVariable bluetooth;

/**********************
 * FLASH VARIABLES STRUCTURE
 * Structure for managing flash memory operations and data
 **********************/
struct FlashVariable
{
    /**********************
     * TEMPORARY FLASH DATA STRUCTURE
     * Structure for temporary storage of flash memory addresses
     **********************/
    struct
    {
        String currentAddr; // Temporary storage for flash address
        String readAddr;    // Temporary storage for read address
    } Temp;                 // Temporary flash data storage

    size_t currentAddr;        // Current address for flash write operations
    size_t readAddr;           // Address for flash read operations
    bool DataReceived = false; // Confirmation flag for flash data reception
    bool Write;                // Flag to enable flash writing
    String TakeAddr;           // Address taken for specific operations
    size_t total_size;         // Total size of flash data
    bool OnceWrite;            // Flag to ensure one-time flash write
    String StartDate = "";     // Temporary storage for start date
    String StartAddress = "";  // Temporary storage for start address
    String LastDate;           // Last recorded date
    String PerverseDate;       // Previous date for comparison
};
extern FlashVariable flash; // Global instance of FlashVariable

/**********************
 * LUBRICATION FUNCTION VARIABLES STRUCTURE
 * Structure for managing lubrication system parameters
 **********************/
struct LubricationFunVariable
{
    float Grams;      // Amount of grease to be dispensed in grams
    bool Enable;      // Lubrication function active flag
    bool End = false; // Lubrication cycle end flag

    bool LoraDataSend = false; // Flag for LoRa data transmission status
    /**********************
     * TOTAL VARIABLE STRUCTURE
     * Structure for tracking total lubrication dispensed
     **********************/
    struct TotalVariable
    {
        float Grams; // Total grease dispensed in grams
    };

    /**********************
     * TEMPORARY VARIABLE STRUCTURE
     * Structure for temporary lubrication calculations
     **********************/
    struct TempVariable
    {
        float Grams;             // Temporary grease quantity for calculations
        float GramSupervisory_1; // Temporary grease quantity for supervisory mode
        float GramSupervisory_2; // Temporary grease quantity for supervisory mode
        float GramSupervisory_3; // Temporary grease quantity for supervisory mode
    };

    /**********************
     * LIVE VARIABLE STRUCTURE
     * Structure for real-time lubrication monitoring
     **********************/
    struct LiveVariable
    {
        float Grams; // Live grease quantity for real-time monitoring
    };

    LiveVariable Live;   // Instance of LiveVariable for real-time monitoring
    TotalVariable Total; // Instance of TotalVariable for total grease tracking
    TempVariable Temp;   // Instance of TempVariable for temporary calculations
};

struct PauseTimeFunvariable
{
    int Hour;    // Pause duration in hours
    int Minute;  // Pause duration in minutes
    int Seconds; // Pause duration in seconds
    bool Enable; // Pause time mode active flag

    bool LoraDataSend = false; // Flag for LoRa data transmission status
    /**********************
     * TOTAL VARIABLE STRUCTURE
     * Structure for tracking total pause time
     **********************/
    struct TotalVariable
    {
        int Hour;   // Total pause time in hours
        int Minute; // Total pause time in minutes
    };
    TotalVariable Total; // Instance of TotalVariable for total pause time tracking

    /**********************
     * TEMPORARY VARIABLE STRUCTURE
     * Structure for temporary pause time calculations
     **********************/
    struct TempVariable
    {
        int Hour;   // Temporary pause hours
        int Minute; // Temporary pause minutes
    };
    TempVariable Temp; // Instance of TempVariable for temporary calculations

    /**********************
     * LIVE VARIABLE STRUCTURE
     * Structure for real-time pause time monitoring
     **********************/
    struct LiveVariable
    {
        int Hour;   // Temporary pause hours
        int Minute; // Temporary pause minutes
    };
    LiveVariable Live; // Instance of LiveVariable for real-time monitoring
};

/**********************
 * GREASE OUT VARIABLE STRUCTURE
 * Structure for managing grease dispensing operations and system modes
 **********************/
struct TestRunVariable
{
    bool Enable;               // Test run mode active flag
    bool LoraDataSend = false; // Flag for LoRa data transmission status
    int min = 0;
    int sec = 0;
};

/**********************
 * RESERVOIR VARIABLE STRUCTURE
 * Structure for managing reservoir parameters
 **********************/
struct ReservoirVariable
{
    bool Enable = false; // Refill mode active flag
    float Capacity;      // Total reservoir capacity
    float Percentage;    // Current refill percentage
    /**********************
     * TEMPORARY VARIABLE STRUCTURE
     * Structure for temporary reservoir calculations
     **********************/
    struct TempVariable
    {
        float Percentage;                // Temporary refill percentage for calculation
        float _old_reservoir_value = -1; // Previous reservoir percentage for change detection
    };
    TempVariable Temp; // Instance of TempVariable for temporary calculations
};

/**********************
 * SLEEP VARIABLE STRUCTURE
 * Structure for managing sleep mode parameters
 **********************/
struct SleepVariable
{
    bool Enable;
};

/**********************
 * GREASE OUT VARIABLE STRUCTURE
 * Structure for managing grease dispensing operations and system modes
 **********************/
struct ProximityFunVariable
{
    int Seconds;             // Proximity sensor reading in seconds
    bool AlertCheck = false; // Flag for proximity alert checks
};

/**********************
 * CONFIGURATION FUNCTION VARIABLES STRUCTURE
 * Structure for managing system configuration parameters
 **********************/
struct ConfigFunVariable
{
    int NoOfOutlet; // Number of lubrication outlets
};
extern ConfigFunVariable Config; // Global instance of ConfigFunVariable

/**********************
 * GREASE OUT VARIABLE STRUCTURE
 * Structure for managing grease dispensing operations and system modes
 **********************/
struct RoundOffVariable
{
    double OutletDischare; // Rounded outlet discharge rate
    double NoOfOutlet;     // Rounded outlet count
    double Grams;          // Rounded temporary grease quantity
};

/**********************
 * SUPERVISORY TIME VARIABLE STRUCTURE
 * Structure for managing supervisory monitoring parameters
 **********************/
struct SupervisoryVariable
{
    float SupervisoryAverageTakeOutGrease_1;                     // Supervisory average grease taken out
    float SupervisoryAverageTakeOutGrease_2;                     // Supervisory average grease taken out
    float SupervisoryAverageTakeOutGrease_3;                     // Supervisory average grease taken out
    float SupervisoryNominalTakeOutGrease_1;                     // Supervisory nominal grease taken out
    float SupervisoryNominalTakeOutGrease_2;                     // Supervisory nominal grease taken out
    float SupervisoryNominalTakeOutGrease_3;                     // Supervisory nominal grease taken out
    bool Supervisory_Nominal_Take_OutGrease_Process_Completed_1; // Flag for completion of supervisory nominal grease process
    bool Supervisory_Nominal_Take_OutGrease_Process_Completed_2; // Flag for completion of supervisory nominal grease process
    bool Supervisory_Nominal_Take_OutGrease_Process_Completed_3; // Flag for completion of supervisory nominal grease process
    float SupervisoryOutGrease;                                  // Supervisory grease output
    float AlertCheckOutGrease;                                   // Supervisory alert check grease
    bool Supervisory1_LimitSwitch_Pressed;                       // Supervisory 1 limit switch status
    bool Supervisory2_LimitSwitch_Pressed;                       // Supervisory 2 limit switch status
    bool Supervisory3_LimitSwitch_Pressed;                       // Supervisory 3 limit switch status

    int Supervisory1_Count; // Supervisory 1 monitoring count
    int Supervisory2_Count; // Supervisory 2 monitoring count
    int Supervisory3_Count; // Supervisory 3 monitoring count

    bool AverageEnable_Supervisory1; // Supervisory 1 average enable flag
    bool AverageEnable_Supervisory2; // Supervisory 2 average enable flag
    bool AverageEnable_Supervisory3; // Supervisory 3 average enable flag

    float NWC_SUP1, AWC_SUP1;      // Supervisory 1 grease monitoring values
    float NWC_SUP2, AWC_SUP2;      // Supervisory 2 grease monitoring values
    float NWC_SUP3, AWC_SUP3;      // Supervisory 3 grease monitoring values
    int ELH_OL1, ELH_OL2, ELH_OL3; // Supervisory outlet element health overload counters
};
extern SupervisoryVariable SupervisoryTime; // Global instance of SupervisoryVariable

extern float _pump1_percentage; // Current percentage of pump 1 output
extern float _pump2_percentage; // Current percentage of pump 2 output
extern float _pump3_percentage; // Current percentage of pump 3 output

extern String _pump1_condtion; // Current condition of pump 1
extern String _pump2_condtion; // Current condition of pump 2
extern String _pump3_condtion; // Current condition of pump 3

/**********************
 * GREASE OUT VARIABLE STRUCTURE
 * Structure for managing grease dispensing operations and system modes
 **********************/
struct GreaseOutVariable
{
    LubricationFunVariable Lubrication; // Lubrication function variables
    PauseTimeFunvariable PauseTime;     // Pause time function variables
    ProximityFunVariable Proximity;     // Proximity function variables
    RoundOffVariable RoundOff;          // Rounding function variables
    TestRunVariable TestRun;            // Test run function variables
    ReservoirVariable Reservoir;        // Reservoir function variables
    SleepVariable Sleep;                // Sleep function variables
    SupervisoryVariable Supervisory;    // Supervisory function variables

    bool Lora_LubricationStart = false; // Flag to indicate start of lubrication process
    bool Lora_PauseStart = false;       // Flag to indicate start of pause time
};
extern GreaseOutVariable GreaseOut; // Global instance of GreaseOutVariable

/**********************
 * RTC VARIABLE STRUCTURE
 * Structure for managing real-time clock parameters
 **********************/
struct RTCVariable
{
    bool Enable = false; // RTC initialization status
    int year;            // Current year
    int month;           // Current month
    int date;            // Current date
    int hour;            // Current hour
    int minute;          // Current minute
    int seconds = 0;     // Current seconds
};
extern RTCVariable RTC; // Global instance of RTCVariable

/**********************
 * ALERT VARIABLE STRUCTURE
 * Structure for managing system alert parameters
 **********************/
struct AlertVariable
{
    bool Enable = false;  // Global alert enabled flag
    char Name[32] = "NO"; // Name of occurred alert (fixed buffer: no heap realloc, so a
                          // concurrent reader can never hit a freed pointer). Write only
                          // via SetAlertName().
    int NameCount = 0;    // Counter for alert occurrences
    float OverloadValue;  // Overload value threshold for alerts
    float ReservoirValue; // Reservoir low level alert threshold
    bool SnoozeReservoir; // Snooze flag for reservoir alerts

    struct
    {
        int NoOfAlert;
        int Reservoir;
        int SignalNotDetected;
        int OverLoad;
        int Supervisory1;
        int Supervisory2;
        int Supervisory3;
        int PumpWeak1;
        int PumpWeak2;
        int PumpWeak3;
        int WorngPassword;
        bool enable;
    } Total;

    /**********************
     * LoRa ALERT DATA STRUCTURE
     * Structure for managing LoRa alert data transmission
     **********************/
    struct
    {
        volatile bool DataSend = false; // Flag for LoRa alert data sending
    } Lora;                             // LoRa alert data management

    struct
    {
        bool Supervisory1;
        bool Supervisory2;
        bool Supervisory3;
        bool RefillAlert;

    } EnableDisable; // Alert enable/disable management
};
extern AlertVariable Alert; // Global instance of AlertVariable

/**********************
 * TOTAL ALERT VARIABLE STRUCTURE
 * Structure for tracking total alert occurrences
 **********************/
struct TotalAlertVariable
{
    int Total_Supervisory1; // Total supervisory 1 alerts
    int Total_Supervisory2; // Total supervisory 2 alerts
    int Total_Supervisory3; // Total supervisory 3 alerts
};
extern TotalAlertVariable TotalAlert; // Global instance of TotalAlertVariable

/**********************
 * VOLTAGE MONITORING STRUCTURE
 * Structure for electrical voltage monitoring and power-down detection
 **********************/
struct VoltageFunVariable
{
    float measured;         // Current voltage reading
    float calibration;      // Low voltage threshold
    int rawADC;             // Variable to store raw ADC reading
    float adcSum;           // Variable for averaging multiple readings
    bool PowerDownDetected; // Flag for power-down detection
};
extern VoltageFunVariable VoltageMonitor; // Global instance of VoltageFunVariable

/**********************
 * CURRENT MONITORING STRUCTURE
 * Structure for electrical current monitoring and overload detection
 **********************/
struct CurrentFunVariable
{
    float measured;       // Current electrical current reading
    float calibration;    // Current value for verification
    char safetyTripCount; // Counter for overload occurrences
    int rawADC;           // Variable to store raw ADC reading
    int adcSum;           // Variable for averaging multiple readings
};
extern CurrentFunVariable CurrentMonitor; // Global instance of CurrentFunVariable

/**********************
 * SERIAL FUNCTION VARIABLES STRUCTURE
 * Structure for managing serial communication parameters
 **********************/
struct SerialFunVariable
{
    bool userPassword;                    // User password verification flag
    String input = "";                    // Input buffer for serial communication
    bool WrongPassword = false;           // Wrong password entered flag
    long int WrongPassword_delay = 1000;  // Delay after wrong password
    int WrongPassword_delay_count = 0;    // Counter for wrong password attempts
    bool OfflineDataWriteProcess = false; // Offline data writing process flag
    unsigned long previousMillisTime = 0; // Previous time for interval calculation
    const long interval2 = 100;           // Interval for time-based operations (100ms)
    int last_Total_Run_Sec = -1;          // Last total run seconds value
    int liveDataOption = 0;               // Live data display option
    int last_PauseTimeSec = -1;           // Last pause time in seconds
    int count_for_current_voltage = 0;    // Counter for current/voltage readings
    bool last_lub_state = true;           // Last lubrication state
};
extern SerialFunVariable SerialFun; // Global instance of SerialFunVariable

#define PASSWORD 336699
struct AdminSettingVariables
{
    bool Enable = false;
    bool EnterPassword = false;
    bool PasswordCorrect = false;
    int EnteredPassword[6] = {0, 0, 0, 0, 0, 0};
};
extern AdminSettingVariables AdminSetting;

struct AboutDevicesVariable
{
    bool Enable = false;
};
extern AboutDevicesVariable AboutDevices;

struct OTAVariable
{
    bool Enable = false; // OTA update enable flag
    int progress;        // OTA update progress percentage
    String current_version;
};
extern OTAVariable OTA;

extern int start_init;      // Initialization start flag
extern int oldStateCount;   // Previous state count for system monitoring
extern int MenuSetting_sec; // Menu setting seconds counter

extern bool Keylock;       // Keylock status flag
extern bool TestrunPage;   // Test run page active flag
extern bool TestRunEnable; // Test run mode enabled flag

extern SemaphoreHandle_t lcdMutex; // Mutex for LCD access synchronization
extern SemaphoreHandle_t adcMutex; // Serialises ADC1 access across Volt/Current tasks

extern bool MainEnable;                       // Main system enable flag
extern bool MenuEnable;                       // Menu system enable flag
extern bool MenuSettingEditTrue;              // Menu setting edit mode flag
extern bool MenuResetnotCompleted; // Menu reset not completed flag
extern bool TestRunEnableError;    // Test run enable error flag
extern bool KeylockEnable;                    // Keylock enable flag
extern bool KeylockDisable;                   // Keylock disable flag
extern unsigned long ArrowlastBlinkTime;      // Last blink time for arrow indicator
extern bool ArrowblinkState;                  // Arrow blink state flag
extern char MenuFunSetcursor;                 // Menu function set cursor character
extern char ConfigFunSetcursor;               // Configuration function set cursor character
extern char SettingFunSetcursor;              // Setting function set cursor character
extern char RefillFunSetcursor;               // Refill function set cursor character
extern char AboutDeviceSetcursor;             // About device set cursor character
extern char SystemSetcursor;
extern bool ConfigEnable;                  // Configuration enable flag
extern bool SettingEnable;                 // Setting enable flag
extern bool RefillEnable;                  // Refill enable flag
extern bool RefillModeEnable;              // Refill mode enable flag
extern bool RefillConfirmation;            // Refill confirmation flag
extern bool RefillConfirmationYesNo;       // Refill confirmation yes/no flag
extern bool RefillConfirmationYesNoBlink;  // Refill confirmation yes/no blink flag
extern bool RefillStart;                   // Refill start flag
extern bool RefillStop;                    // Refill stop flag
extern bool RefillCompleted;               // Refill completed flag
extern String ConfigSelectVariable;        // Configuration select variable string
extern String SettingSelectVariable;       // Setting select variable string
extern bool VariableSeleteEnable;          // Variable select enable flag
extern bool TankBlink;                     // Tank blink indicator flag
extern bool OutletBlink;                   // Outlet blink indicator flag
extern bool LubBlink;                      // Lubrication blink indicator flag
extern bool PtmHourBlink;                  // PTM hour blink indicator flag
extern bool PtmMinBlink;                   // PTM minute blink indicator flag
extern bool supMinBlink[4];                // Supervisory minute blink indicators array
extern int scrollOffset;                   // Scroll offset for display
extern int totalItems;                     // Total number of items for display
extern int itemHeight;                     // Height of each item for display
extern int maxVisibleItems;                // Maximum visible items on display
extern int Space;                          // Space variable for display layout
extern float TankCapcity;                  // Tank capacity variable
extern float TempTankCapcity;              // Temporary tank capacity variable
extern int capcity;                        // Capacity variable
extern unsigned long lastBlinkTime;        // Last blink time for system indicator
extern bool blinkState;                    // Blink state flag for system indicator
extern bool ResetEnable;                   // Reset enable flag
extern bool Reset;                         // Reset flag
extern bool ResetCompleted;                // Reset completed flag
extern bool ResetAllValueSet;              // Reset all value set flag
extern bool Reset_SendData_Lora;           // Reset send data via LoRa flag
extern bool Reset_sendData_Lora_Subscribe; // Reset send data via LoRa subscribe flag

extern bool MainEnableWithReset;           // Main enable with reset flag
extern bool MenuSettingMotorOff;           // Menu setting motor off flag
extern bool MenuSettingEnableDisableCheck; // Menu setting enable/disable check flag
extern int speedcount;                     // Speed count variable
extern bool speedEnable;                   // Speed enable flag
extern int speed;                          // Speed variable

extern bool LoraPumpStatusSend; // Flag to indicate LoRa pump status data sending
extern int SleepModeEnableCount;
extern int SleepModeDisableCount;

extern int Lora_disconnected_count;
extern bool _Controller_online_data_send;

extern bool ReconfigEnableRefillNeed;

#endif /* __SYSTEM_H__ */
