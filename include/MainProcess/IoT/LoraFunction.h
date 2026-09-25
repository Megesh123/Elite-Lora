/**********************
 * LORA COMMUNICATION HEADER
 * Defines interface for LoRa messaging and IoT connectivity
 * Handles LoRaWAN communication, topic management, and message processing
 *
 * v2.1.1 - TX rate limiting + input sanitization; truncate payloads at 70 bytes
 **********************/

#ifndef __LORA_FUNCTION_H__
#define __LORA_FUNCTION_H__

#include "Arduino.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/**********************
 * FUNCTION DECLARATIONS
 * Handlers for specific LoRa message topics
 * Each function processes messages for different system operations
 **********************/
void _offline_data_process(String payload);          // Handle offline data transmission
void _setting_data_process(String payload);          // Process system configuration data
void _alert_data_process(String payload);            // Handle alert acknowledgments
void _alert_clear_process(String payload);           // Process alert clearance commands
void _factory_reset_process(String payload);         // Handle factory reset commands
void _refill_complete_process(String payload);       // Process refill completion
void _refillMode_start_stop(String payload);         // Handle refill mode control
void _testRun_start_stop(String payload);            // Process test run commands
void _app_online_offline_check(String payload);      // Handle app status checks
void _reconfig_process(String payload);              // Process system reconfiguration
void _controller_online_came_return(String payload); // Handle controller status
void _controller_sleep_process(String payload);      // Process sleep mode commands
void _control_Snooze(String payload);                // Handle alert snooze commands
void _supervisory_Enable_Disable(String payload);    // Handle supervisory level enable/disable commands
void _refill_alert_enable_disable(String payload);   // Handle refill commands

void _receive_message_process(String Topic, String payload); // Route messages to appropriate handlers

void Receive();

struct ReceivedVariable
{
    String _offline_data_process = "";
    String _setting_data_process = "";
    String _alert_data_process = "";
    String _alert_clear_process = "";
    String _factory_reset_process = "";
    String _refill_complete_process = "";
    String _refillMode_start_stop = "";
    String _testRun_start_stop = "";
    String _app_online_offline_check = "";
    String _reconfig_process = "";
    String _controller_online_came_return = "";
    String _controller_sleep_process = "";
    String _control_Snooze = "";
    String _Setting = "";
    String _SupervisoryEnableDisable = "";
    String _refill = "";
    String _RefillAlert = "";
    String _mode = "";
    String _conmode = "";
};
extern ReceivedVariable Received; // Instance of ReceivedVariable for received pause time tracking

class LoRaFunctionTimer
{
public:
    void NexuxInOnline(); // Handle application online timing for LoRa
};

/**********************
 * LoRaFunction CLASS
 * Manages LoRa communication and IoT connectivity
 **********************/
class LoRaFunction
{
public:
    void MessageProcessing(String payload);  // Process incoming LoRa messages
    void SettingPageData(String payload);    // Handle setting page configuration data
    void OfflineSettingPageData();           // Handle offline setting page configuration data
    void ReconfigProcessing(String payload); // Process system reconfiguration commands

    void testRunStart(); // Start test run operation
    void testRunStop();  // Stop test run operation
    void refillStart();  // Start refill operation
    void refillStop();   // Stop refill operation
    void ResetValue();   // Reset operational values to defaults

    void Reservoir(String data); // Send reservoir capacity data
    void alert(String data);     // Send alert notifications
    void flashData(String data); // Handle flash data transmission

    void liveProcess_Lubrication();       // Process and format live data
    void liveProcess_Pause_Time();        // Process and format live data
    void liveProcess_Lubrication_Start(); // Process and format live data
    void liveProcess_Pause_Time_Start();  // Process and format live data
    void liveProcess_TestRun();           // Process and format live data
    void liveProcess_Alert();             // Process and format live data
    void liveProcess_Sleep();             // Process and format live data

    void NEXUS_IN_ON(String data);         // Send Nexus interface online status
    void NEXUS_NOT_REACHABLE(String data); // Send Nexus unreachable status
    void NEXUS_IN_ALERT(String data);      // Send Nexus alert status
    void NEXUS_ONLINE_UPDATE();            // Update Nexus online status
    LoRaFunctionTimer TimerFun;            // Instance of LoRaFunctionTimer for application online timing

    void playloadTopic(); // Generate payload topics
    void returnTopic();   // Generate return topics

    void controllerOnline(); // Handle controller online status
    void offlineDataStart(); // Start offline data transmission
    void offlineDataStop();  // Stop offline data transmission

    void OverloadAlert();          // Handle overload alert
    void ReservoirLowLevelAlert(); // Handle reservoir low level alert
    void SignalNotDetected();      // Handle signal not detected alert
    void SupervisoryALert();       // Handle supervisory alert
    void PumpWeakAlert();          // Handle pump weak alert

    void PumpStatus(); // Send pump status updates via LoRa

    bool publish(const String &_topic, const String &_data);  // Publish LoRa message
    bool response(const String &_topic, const String &_data); // Send LoRa response

private:
    void _supervisory1_Alert(); // Handle supervisory 1 alert
    void _supervisory2_Alert(); // Handle supervisory 2 alert
    void _supervisory3_Alert(); // Handle supervisory 3 alert

    void _pumpElement1_Weak(); // Handle pump element 1 weak alert
    void _pumpElement2_Weak(); // Handle pump element 2 weak alert
    void _pumpElement3_Weak(); // Handle pump element 3 weak alert

    void _pump_status_outlet_1(); // Check and send pump element 1 status
    void _pump_status_outlet_2(); // Check and send pump element 2 status
    void _pump_status_outlet_3(); // Check and send pump element 3 status

    void ReconfigReset();
};

extern LoRaFunction LoRa; // Global LoRa instance

bool loraBegin();
bool loraJoin();

void loraReceiveProcess();         // Drain LoRaSerial and dispatch complete downlink lines
void processLoRaLine(String line); // Process a complete downlink line from LoRaSerial
String loraSanitize(String input); // v2.1.1 Remove control/non-ASCII chars, trim, truncate at 70
bool loraUplinkAllowed();          // v2.1.0 Rate-limit gate for outgoing uplinks
extern bool LoraInitialized;       // Flag to indicate if LoRa is initialized
extern bool LoraJoined;            // Flag to indicate if LoRa network join succeeded

extern SemaphoreHandle_t LoRaSerialMutex; // Serialises LoRaSerial access between the publish/join calls and the RX task

#endif /* __LORA_FUNCTION_H__ */
