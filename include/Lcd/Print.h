#ifndef __PRINT_H__
#define __PRINT_H__

extern const char *PRINT_CAPACITY;            // Label for capacity field
extern const char *PRINT_GRAM;                // Unit token for grams
extern const char *PRINT_NO_OF_OUTLET;        // Label for number of outlets
extern const char *PRINT_DISCHARGE;           // Label for discharge value
extern const char *PRINT_PAUSE_HOUR;          // Label for pause hour value
extern const char *PRINT_PAUSE_MIN;           // Label for pause minute value
extern const char *PRINT_SET_CAPACITY;        // Command text for capacity set
extern const char *PRINT_ASK_REFILL_MODE;     // Prompt text for refill mode query
extern const char *PRINT_REFILL_MODE;         // Refill mode title text
extern const char *PRINT_YES;                 // Generic yes token
extern const char *PRINT_NO;                  // Generic no token
extern const char *PRINT_REFILL_START;        // Refill start status text
extern const char *PRINT_REFILL_STOP;         // Refill stop status text
extern const char *PRINT_REFILL_SUCCESS;      // Refill success message
extern const char *PRINT_REFILL_COMPLETED;    // Refill completed message
extern const char *PRINT_REFILL_STATUS;       // Refill status label
extern const char *PRINT_PERCENTAGE_SYSMBOL;  // Percentage symbol token
extern const char *PRINT_REFILL_NOW;          // Refill now prompt
extern const char *PRINT_OK_COMPLETED;        // OK button completed hint
extern const char *PRINT_OK_STOP;             // OK button stop hint
extern const char *PRINT_CONFIGURATION;       // Configuration menu text
extern const char *PRINT_SETTING;             // Setting menu text
extern const char *PRINT_REFILL;              // Refill menu text
extern const char *PRINT_RESET;               // Reset menu text
extern const char *PRINT_RESET_COMPLETED;     // Reset completed message
extern const char *PRINT_OK_ON_OFF;           // OK button on/off hint
extern const char *PRINT_ON;                  // ON state token
extern const char *PRINT_OFF;                 // OFF state token
extern const char *PRINT_TEST_RUN;            // Test run title text
extern const char *PRINT_TESTRUN;             // Test run action text
extern const char *PRINT_TEST_RUN_INIT;       // Test run initialization text
extern const char *PRINT_NOT_COMPLETED;       // Not completed message
extern const char *PRINT_PRESS_BACK;          // Back button hint
extern const char *PRINT_RESERVOIR_LOW_LEVEL; // Reservoir low level alert text
extern const char *PRINT_REFILL_NOW_NEED;     // Refill required alert text
extern const char *PRINT_SET_OUTLET;          // Outlet setup title

extern const char *PRINT_AMPS;                             // Current unit in amps
extern const char *PRINT_RESET_OK;                         // Reset confirmation hint
extern const char *PRINT_ALERT;                            // Alert title text
extern const char *PRINT_ALERT_NAME_OVERLOAD;              // Overload alert name line
extern const char *PRINT_ALERT_NAME_RESERVOIR_1;           // Reservoir alert first line
extern const char *PRINT_ALERT_NAME_RESERVOIR_2;           // Reservoir alert second line
extern const char *PRINT_ALERT_NAME_SUPERVISORY_1;         // Supervisory 1 alert name
extern const char *PRINT_ALERT_NAME_SUPERVISORY_2;         // Supervisory 2 alert name
extern const char *PRINT_ALERT_NAME_SUPERVISORY_3;         // Supervisory 3 alert name
extern const char *PRINT_ALERT_NAME_SIGNAL_NOT_DETECTED_1; // Signal alert first line
extern const char *PRINT_ALERT_NAME_SIGNAL_NOT_DETECTED_2; // Signal alert second line
extern const char *PRINT_ALERT_NAME_ELEMENT_WEAK;          // Element weak alert name
extern const char *PRINT_ALERT_NAME_UNKNOW;                // Unknown alert name

extern const char *PRINT_ENABLE;     // Enable state text
extern const char *PRINT_ENABLED;    // Enabled confirmation text
extern const char *PRINT_CONNECTING; // Connecting state text
extern const char *PRINT_CONNECTED;  // Connected state text
extern const char *PRINT_RECEIVE;    // Receive label text
extern const char *PRINT_RECEIVED;   // Received status text

extern const char *PRINT_VOLTAGE;                         // Voltage label text
extern const char *PRINT_VOLT;                            // Voltage unit token
extern const char *PRINT_CURRENT;                         // Current label text
extern const char *PRINT_TOTAL_OPERATION_TIME;            // Total operation time label
extern const char *PRINT_TOTAL_LUBRICATION_DISCHARGE;     // Total lubrication discharge label
extern const char *PRINT_TOTAL_PAUSE_TIME;                // Total pause time label
extern const char *PRINT_TOTAL_ALERT;                     // Total alert label
extern const char *PRINT_TOTAL_ALERT_REFILL;              // Refill alert counter label
extern const char *PRINT_TOTAL_ALERT_OVERLOAD;            // Overload alert counter label
extern const char *PRINT_TOTAL_ALERT_SUPERVISORY_1;       // Supervisory 1 alert counter label
extern const char *PRINT_TOTAL_ALERT_SUPERVISORY_2;       // Supervisory 2 alert counter label
extern const char *PRINT_TOTAL_ALERT_SUPERVISORY_3;       // Supervisory 3 alert counter label
extern const char *PRINT_TOTAL_ALERT_SIGNAL_NOT_DETECTED; // Signal not detected alert counter label
extern const char *PRINT_TOTAL_ALERT_PUMPWEAK_1;          // Pump weak 1 alert counter label
extern const char *PRINT_TOTAL_ALERT_PUMPWEAK_2;          // Pump weak 2 alert counter label
extern const char *PRINT_TOTAL_ALERT_PUMPWEAK_3;          // Pump weak 3 alert counter label
extern const char *PRINT_SYSTEM;                          // System title text
extern const char *PRINT_FACTORY_RESET;                   // Factory reset title text

extern const char *PRINT_ENTER_PASSWORD;        // Enter password prompt
extern const char *PRINT_WRONG_PASSWORD;        // Wrong password alert
extern const char *PRINT_DO_YOU_NEED_RESET;     // Reset confirmation prompt
extern const char *PRINT_SUPERVISORY_1_ENABLE;  // SP1 enabled status text
extern const char *PRINT_SUPERVISORY_2_ENABLE;  // SP2 enabled status text
extern const char *PRINT_SUPERVISORY_3_ENABLE;  // SP3 enabled status text
extern const char *PRINT_SUPERVISORY_1_DISABLE; // SP1 disabled status text
extern const char *PRINT_SUPERVISORY_2_DISABLE; // SP2 disabled status text
extern const char *PRINT_SUPERVISORY_3_DISABLE; // SP3 disabled status text
extern const char *PRINT_REFILL_ALERT_ENABLE;   // Refill alert enabled status text
extern const char *PRINT_REFILL_ALERT_DISABLE;  // Refill alert disabled status text
extern const char *PRINT_OTA;                   // Percentage label text

extern const char *PRINT_RESET_TO_SAVE_CHANGES; // Reset required to save changes text

extern const char *PRINT_PRESS_OK; // OK button hint
extern const char *PRINT_ZERO;     // Zero string token

extern const char HORIZONTAL; // Horizontal line direction token
extern const char VERTICAL;   // Vertical line direction token

extern const char *PRINT_RESERVOIR_LEVEL; // Reservoir level label
extern const char *PRINT_MODE;            // Mode label
extern const char *PRINT_MODE_LUB;        // Lubrication mode label
extern const char *PRINT_MODE_PAUSE_TIME; // Pause time mode label
extern const char *PRINT_MODE_REFILL;
extern const char *PRINT_MODE_EMPTY;   // Empty mode fallback label
extern const char *PRINT_COMPANY_NAME; // Company name text
extern const char *PRINT_GOOD;         // Good condition text
extern const char *PRINT_AVERAGE;      // Average condition text
extern const char *PRINT_BAD;          // Bad condition text
extern const char *PRINT_CALCULATE;    // Calculation label text
extern const char *PRINT_DISABLE;
extern const char *PRINT_HYPHEN;                 // Hyphen token
extern const char *PRINT_OUTLET_1;               // Outlet 1 label
extern const char *PRINT_OUTLET_2;               // Outlet 2 label
extern const char *PRINT_OUTLET_3;               // Outlet 3 label
extern const char *PRINT_SET_KEY_LOCK_DISABLE_1; // Key lock disable prompt first line
extern const char *PRINT_SET_KEY_LOCK_DISABLE_2; // Key lock disable prompt second line
extern const char *PRINT_KEY_LOCK_ENABLE;        // Key lock enabled status text
extern const char *PRINT_KEY_LOCK_DISABLE;       // Key lock disabled status text

extern const char *PRINT_EMPTY;       // Empty string token
extern const char *PRINT_RIGHT_ARROW; // Right arrow symbol token
extern const char *PRINT_REFILL_IT;

#endif
