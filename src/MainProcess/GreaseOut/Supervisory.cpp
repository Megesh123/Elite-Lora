/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Supervisory monitoring implementation for Lubrication Control System
 */

#include "HeaderFile.h"

SUPERVISORYFUNCTION Supervisory;

/**********************
 * SUPERVISORY FUNCTION
 * Monitors and manages supervisory checks for grease dispensing system
 **********************/

void SUPERVISORYFUNCTION::Supervisory()
{

    bool _send_lora_status = false; // Flag to track if LoRa status has been sent

    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_1 >= SUPERVISORY_NOMINAL_GRAM_INIT && !SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_1 = true;
        if (Alert.EnableDisable.Supervisory1) // Check if supervisory for outlet 1 is enabled
        {
            _supervisory1_norminal(); // Evaluate nominal supervisory data for outlet 1
        }
        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }
    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_2 >= SUPERVISORY_NOMINAL_GRAM_INIT && !SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_2 = true;

        if (Alert.EnableDisable.Supervisory2) // Check if supervisory for outlet 2 is enabled
        {
            _supervisory2_norminal(); // Evaluate nominal supervisory data for outlet 2
        }
        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }
    if (SupervisoryTime.SupervisoryNominalTakeOutGrease_3 >= SUPERVISORY_NOMINAL_GRAM_INIT && !SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.Supervisory_Nominal_Take_OutGrease_Process_Completed_3 = true;

        if (Alert.EnableDisable.Supervisory3) // Check if supervisory for outlet 3 is enabled
        {
            _supervisory3_norminal(); // Evaluate nominal supervisory data for outlet 3
        }
        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }

    if (SupervisoryTime.SupervisoryAverageTakeOutGrease_1 >= SUPERVISORY_AVERAGE_GRAM) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.SupervisoryAverageTakeOutGrease_1 = 0; // Reset average grease dispensed

        if (Alert.EnableDisable.Supervisory1) // Check if supervisory for outlet 1 is enabled
        {
            _supervisory1_average();               // Calculate average supervisory data for outlet 1
            _supervisory1_Outlet_Element_Health(); // Assess health of outlet 1 based on supervisory data
        }

        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }
    if (SupervisoryTime.SupervisoryAverageTakeOutGrease_2 >= SUPERVISORY_AVERAGE_GRAM) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.SupervisoryAverageTakeOutGrease_2 = 0; // Reset average grease dispensed

        if (Alert.EnableDisable.Supervisory2) // Check if supervisory for outlet 2 is enabled
        {
            _supervisory2_average();               // Calculate average supervisory data for outlet 2
            _supervisory2_Outlet_Element_Health(); // Assess health of outlet 2 based on supervisory data
        }

        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }
    if (SupervisoryTime.SupervisoryAverageTakeOutGrease_3 >= SUPERVISORY_AVERAGE_GRAM) // Check if average grease dispensed exceeds threshold
    {
        SupervisoryTime.SupervisoryAverageTakeOutGrease_3 = 0; // Reset average grease dispensed

        if (Alert.EnableDisable.Supervisory3) // Check if supervisory for outlet 3 is enabled
        {
            _supervisory3_average();               // Calculate average supervisory data for outlet 3
            _supervisory3_Outlet_Element_Health(); // Assess health of outlet 3 based on supervisory data
        }
        if (!_send_lora_status)
        {
            _pump_status_send_lora = true;
            _send_lora_status = true; // Set flag to send LoRa status update
        }
    }
}

/**********************
 * CHECK SUPERVISORY ALERT FUNCTION
 * Placeholder for checking and triggering supervisory alerts
 **********************/
void SUPERVISORYFUNCTION::Check_Supervisory_Alert()
{
    if (SupervisoryTime.AlertCheckOutGrease >= SUPERVISORY_ALERT_CHECK_GRAM) // Trigger alert validation only after enough grease-out accumulation
    {
        _Supervisory1_Alert();                   // Evaluate supervisory alert state for outlet 1
        _Supervisory2_Alert();                   // Evaluate supervisory alert state for outlet 2
        _Supervisory3_Alert();                   // Evaluate supervisory alert state for outlet 3
        SupervisoryTime.AlertCheckOutGrease = 0; // Reset accumulated alert-check grease counter for next cycle
    }
}

/**********************
 * SUPERVISORY 1 FUNCTION
 * Handles supervisory checks for outlet 1
 **********************/
// Edge latches so a held limit switch sets SupervisoryN_LimitSwitch_Pressed
// exactly once per press, consistently for all three outlets.
bool __LimitSwitch1Pressed = false;
bool __LimitSwitch2Pressed = false;
bool __LimitSwitch3Pressed = false;
void SUPERVISORYFUNCTION::Supervisory1()
{
    if (Alert.EnableDisable.Supervisory1)
    {
        if (digitalRead(PIN_LIMIT_SW_1) == Pressed && Config.NoOfOutlet > 0 && GreaseOut.Lubrication.Temp.GramSupervisory_1 >= SUPERVISORY_GREASE_SETTLING && !LimitSwitch1Pressed) // Check if limit switch 1 is pressed and conditions are met
        {
            SupervisoryTime.Supervisory1_Count++; // Increment supervisory count for outlet 1
            LimitSwitch1Pressed = true;           // Allow next valid press to be counted as a new cycle
        }
        if (digitalRead(PIN_LIMIT_SW_1) == NotPressed) // Detect release state for outlet 1 limit switch
        {
            LimitSwitch1Pressed = false; // Allow next valid press to be counted as a new cycle
            __LimitSwitch1Pressed = false;
        }
        if (digitalRead(PIN_LIMIT_SW_1) == Pressed && Config.NoOfOutlet > 0 && !__LimitSwitch1Pressed) // Detect active press state when outlet 1 is configured
        {
            SupervisoryTime.Supervisory1_LimitSwitch_Pressed = true; // Set limit switch pressed flag for outlet 1
            __LimitSwitch1Pressed = true;
        }
    }
}

/**********************
 * SUPERVISORY 1 ALERT FUNCTION
 * Placeholder for triggering alert for supervisory 1
 **********************/
void SUPERVISORYFUNCTION::_Supervisory1_Alert()
{
    if (Config.NoOfOutlet > 0 && Alert.EnableDisable.Supervisory1) // Run supervisory-1 alert logic only when outlet 1 exists and is enabled
    {
        if (SupervisoryTime.Supervisory1_LimitSwitch_Pressed) // A valid limit-switch event occurred in this alert window
        {
            SupervisoryTime.Supervisory1_LimitSwitch_Pressed = false; // Consume the event so it is not reused next cycle
        }
        else
        {
            Alert.NameCount = 3;           // Select supervisory-1 alert code/index
            Alert.Enable = true;           // Enable alert publishing/processing
            SetAlertName("SUPERVISORY 1"); // Set human-readable alert name
        }
    }
}

/**********************
 * SUPERVISORY 2 FUNCTION
 * Handles supervisory checks for outlet 2
 **********************/
void SUPERVISORYFUNCTION::Supervisory2()
{
    if (Alert.EnableDisable.Supervisory2)
    {
        if (digitalRead(PIN_LIMIT_SW_2) == Pressed && Config.NoOfOutlet > 1 && GreaseOut.Lubrication.Temp.GramSupervisory_2 >= SUPERVISORY_GREASE_SETTLING && !LimitSwitch2Pressed) // Check if limit switch 2 is pressed and conditions are met
        {
            SupervisoryTime.Supervisory2_Count++; // Increment supervisory count for outlet 2
            LimitSwitch2Pressed = true;           // Latch current press state to avoid duplicate counts on same press
        }
        if (digitalRead(PIN_LIMIT_SW_2) == NotPressed) // Detect release state for outlet 2 limit switch
        {
            LimitSwitch2Pressed = false; // Allow next valid press to be counted as a new cycle
            __LimitSwitch2Pressed = false;
        }
        if (digitalRead(PIN_LIMIT_SW_2) == Pressed && Config.NoOfOutlet > 1 && !__LimitSwitch2Pressed) // Detect active press edge when outlet 2 is configured
        {
            SupervisoryTime.Supervisory2_LimitSwitch_Pressed = true; // Set limit switch pressed flag for outlet 2
            __LimitSwitch2Pressed = true;
        }
    }
}

/**********************
 * SUPERVISORY 2 ALERT FUNCTION
 * Placeholder for triggering alert for supervisory 2
 **********************/
void SUPERVISORYFUNCTION::_Supervisory2_Alert()
{
    if (Config.NoOfOutlet > 1 && Alert.EnableDisable.Supervisory2) // Run supervisory-2 alert logic only when outlet 2 exists and is enabled
    {
        if (SupervisoryTime.Supervisory2_LimitSwitch_Pressed) // A valid limit-switch event occurred in this alert window
        {
            SupervisoryTime.Supervisory2_LimitSwitch_Pressed = false; // Consume the event so it is not reused next cycle
        }
        else
        {
            Alert.NameCount = 4;           // Select supervisory-2 alert code/index
            Alert.Enable = true;           // Enable alert publishing/processing
            SetAlertName("SUPERVISORY 2"); // Set human-readable alert name
        }
    }
}

/**********************
 * SUPERVISORY 3 FUNCTION
 * Handles supervisory checks for outlet 3
 **********************/
void SUPERVISORYFUNCTION::Supervisory3()
{
    if (Alert.EnableDisable.Supervisory3)
    {
        if (digitalRead(PIN_LIMIT_SW_3) == Pressed && Config.NoOfOutlet > 2 && GreaseOut.Lubrication.Temp.GramSupervisory_3 >= SUPERVISORY_GREASE_SETTLING && !LimitSwitch3Pressed) // Check if limit switch 3 is pressed and conditions are met
        {
            SupervisoryTime.Supervisory3_Count++; // Increment supervisory count for outlet 3
            LimitSwitch3Pressed = true;           // Latch current press state to avoid duplicate counts on same press
        }
        if (digitalRead(PIN_LIMIT_SW_3) == NotPressed) // Detect release state for outlet 3 limit switch
        {
            LimitSwitch3Pressed = false; // Allow next valid press to be counted as a new cycle
            __LimitSwitch3Pressed = false;
        }

        if (digitalRead(PIN_LIMIT_SW_3) == Pressed && Config.NoOfOutlet > 2 && !__LimitSwitch3Pressed) // Detect active press edge when outlet 3 is configured
        {
            SupervisoryTime.Supervisory3_LimitSwitch_Pressed = true; // Set limit switch pressed flag for outlet 3
            __LimitSwitch3Pressed = true;
        }
    }
}

/**********************
 * SUPERVISORY 3 ALERT FUNCTION
 * Placeholder for triggering alert for supervisory 3
 **********************/
void SUPERVISORYFUNCTION::_Supervisory3_Alert()
{
    if (Config.NoOfOutlet > 2 && Alert.EnableDisable.Supervisory3) // Run supervisory-3 alert logic only when outlet 3 exists and is enabled
    {
        if (SupervisoryTime.Supervisory3_LimitSwitch_Pressed) // A valid limit-switch event occurred in this alert window
        {
            SupervisoryTime.Supervisory3_LimitSwitch_Pressed = false; // Consume the event so it is not reused next cycle
        }
        else
        {
            Alert.NameCount = 5;           // Select supervisory-3 alert code/index
            Alert.Enable = true;           // Enable alert publishing/processing
            SetAlertName("SUPERVISORY 3"); // Set human-readable alert name
        }
    }
}

/**********************
 * SUPERVISORY 1 NOMINAL FUNCTION
 * Evaluates supervisory data for outlet 1 under nominal conditions
 **********************/
void SUPERVISORYFUNCTION::_supervisory1_norminal()
{
    if (!SupervisoryTime.AverageEnable_Supervisory1 && Config.NoOfOutlet > 0) // Check if average mode is disabled and outlet 1 is configured
    {
        float a = SupervisoryTime.Supervisory1_Count;
        SupervisoryTime.NWC_SUP1 = a / SUPERVISORY_NORMINAL_AVERAGE; // Set nominal working count for outlet 1
        SupervisoryTime.AverageEnable_Supervisory1 = true;           // Enable average mode for outlet 1
        SupervisoryTime.Supervisory1_Count = 0;                      // Reset supervisory count for outlet 1
        SupervisoryTime.ELH_OL1 = PUMP_GOOD;                         // Set status to GOOD
        _pump1_condtion = "GOOD";
        _pump1_percentage = 100;
        Serial.println("Supervisory 1 Nominal Condition Evaluated - NWC Set and Average Mode Enabled");
    }
}

/**********************
 * SUPERVISORY 2 NOMINAL FUNCTION
 * Evaluates supervisory data for outlet 2 under nominal conditions
 **********************/
void SUPERVISORYFUNCTION::_supervisory2_norminal()
{
    if (!SupervisoryTime.AverageEnable_Supervisory2 && Config.NoOfOutlet > 1) // Check if average mode is disabled and outlet 2 is configured
    {
        float b = SupervisoryTime.Supervisory2_Count;
        SupervisoryTime.NWC_SUP2 = b / SUPERVISORY_NORMINAL_AVERAGE; // Set nominal working count for outlet 2
        SupervisoryTime.AverageEnable_Supervisory2 = true;           // Enable average mode for outlet 2
        SupervisoryTime.Supervisory2_Count = 0;                      // Reset supervisory count for outlet 2
        SupervisoryTime.ELH_OL2 = PUMP_GOOD;                         // Set status to GOOD
        _pump2_condtion = "GOOD";
        _pump2_percentage = 100;
    }
}

/**********************
 * SUPERVISORY 3 NOMINAL FUNCTION
 * Evaluates supervisory data for outlet 3 under nominal conditions
 **********************/
void SUPERVISORYFUNCTION::_supervisory3_norminal()
{
    if (!SupervisoryTime.AverageEnable_Supervisory3 && Config.NoOfOutlet > 2) // Check if average mode is disabled and outlet 3 is configured
    {
        float c = SupervisoryTime.Supervisory3_Count;
        SupervisoryTime.NWC_SUP3 = c / SUPERVISORY_NORMINAL_AVERAGE; // Set nominal working count for outlet 3
        SupervisoryTime.AverageEnable_Supervisory3 = true;           // Enable average mode for outlet 3
        SupervisoryTime.Supervisory3_Count = 0;                      // Reset supervisory count for outlet 3
        SupervisoryTime.ELH_OL3 = PUMP_GOOD;                         // Set status to GOOD
        _pump3_condtion = "GOOD";
        _pump3_percentage = 100;
    }
}

/**********************
 * SUPERVISORY 1 AVERAGE FUNCTION
 * Calculates average supervisory data for outlet 1
 **********************/
void SUPERVISORYFUNCTION::_supervisory1_average()
{
    if (SupervisoryTime.AverageEnable_Supervisory1 && Config.NoOfOutlet > 0) // Check if average mode is enabled and outlet 1 is configured
    {
        SupervisoryTime.AWC_SUP1 = SupervisoryTime.Supervisory1_Count; // Set average working count for outlet 1
        SupervisoryTime.Supervisory1_Count = 0;                        // Reset supervisory count for outlet 1
    }
}

/**********************
 * SUPERVISORY 2 AVERAGE FUNCTION
 * Calculates average supervisory data for outlet 2
 **********************/
void SUPERVISORYFUNCTION::_supervisory2_average()
{
    if (SupervisoryTime.AverageEnable_Supervisory2 && Config.NoOfOutlet > 1) // Check if average mode is enabled and outlet 2 is configured
    {
        SupervisoryTime.AWC_SUP2 = SupervisoryTime.Supervisory2_Count; // Set average working count for outlet 2
        SupervisoryTime.Supervisory2_Count = 0;                        // Reset supervisory count for outlet 2
    }
}

/**********************
 * SUPERVISORY 3 AVERAGE FUNCTION
 * Calculates average supervisory data for outlet 3
 **********************/
void SUPERVISORYFUNCTION::_supervisory3_average()
{
    if (SupervisoryTime.AverageEnable_Supervisory3 && Config.NoOfOutlet > 2) // Check if average mode is enabled and outlet 3 is configured
    {
        SupervisoryTime.AWC_SUP3 = SupervisoryTime.Supervisory3_Count; // Set average working count for outlet 3
        SupervisoryTime.Supervisory3_Count = 0;                        // Reset supervisory count for outlet 3
    }
}

/**********************
 * SUPERVISORY 1 OUTLET ELEMENT HEALTH FUNCTION
 * Assesses health of outlet 1 based on supervisory data
 **********************/
void SUPERVISORYFUNCTION::_supervisory1_Outlet_Element_Health()
{
    if (Config.NoOfOutlet > 0) // Check if outlet 1 is configured
    {
        if (SupervisoryTime.AWC_SUP1 > 0 && SupervisoryTime.NWC_SUP1 > 0) // Both counts valid (NWC guards the divide)
        {
            if (SupervisoryTime.AWC_SUP1 > SupervisoryTime.NWC_SUP1) // Ensure average working count does not exceed nominal working count
            {
                SupervisoryTime.AWC_SUP1 = SupervisoryTime.NWC_SUP1; // Cap average working count to nominal working count
            }

            _pump1_percentage = (SupervisoryTime.AWC_SUP1 / SupervisoryTime.NWC_SUP1) * 100; // Calculate percentage difference

            if (_pump1_percentage >= 90 && _pump1_percentage <= 100) // Excellent condition
            {
                SupervisoryTime.ELH_OL1 = PUMP_GOOD; // Set status to GOOD
                _pump1_condtion = "GOOD";
            }
            else if (_pump1_percentage >= 75 && _pump1_percentage < 90) // Average condition
            {
                SupervisoryTime.ELH_OL1 = PUMP_AVERAGE; // Set status to AVERAGE
                _pump1_condtion = "AVG";
            }
            else if (_pump1_percentage < 75) // Poor condition
            {
                SupervisoryTime.ELH_OL1 = PUMP_BAD; // Set status to BAD
                _pump1_condtion = "BAD";
                Alert.NameCount = 7;
                Alert.Enable = true;
                SetAlertName("PUMP WEAK 1");
            }
        }
        else
        {
            SupervisoryTime.ELH_OL1 = PUMP_GOOD; // Set status to GOOD
        }
    }
}

/**********************
 * SUPERVISORY 2 OUTLET ELEMENT HEALTH FUNCTION
 * Assesses health of outlet 2 based on supervisory data
 **********************/
void SUPERVISORYFUNCTION::_supervisory2_Outlet_Element_Health()
{
    if (Config.NoOfOutlet > 1) // Check if outlet 2 is configured
    {
        if (SupervisoryTime.AWC_SUP2 > 0 && SupervisoryTime.NWC_SUP2 > 0) // Both counts valid (NWC guards the divide)
        {
            if (SupervisoryTime.AWC_SUP2 > SupervisoryTime.NWC_SUP2) // Ensure average working count does not exceed nominal working count
            {
                SupervisoryTime.AWC_SUP2 = SupervisoryTime.NWC_SUP2; // Cap average working count to nominal working count
            }
            _pump2_percentage = (SupervisoryTime.AWC_SUP2 / SupervisoryTime.NWC_SUP2) * 100; // Calculate percentage difference

            if (_pump2_percentage >= 90 && _pump2_percentage <= 100) // Excellent condition
            {
                SupervisoryTime.ELH_OL2 = PUMP_GOOD; // Set status to GOOD
                _pump2_condtion = "GOOD";
            }
            else if (_pump2_percentage >= 75 && _pump2_percentage < 90) // Average condition
            {
                SupervisoryTime.ELH_OL2 = PUMP_AVERAGE; // Set status to AVERAGE
                _pump2_condtion = "AVG";
            }
            else if (_pump2_percentage < 75) // Poor condition
            {
                SupervisoryTime.ELH_OL2 = PUMP_BAD; // Set status to BAD
                _pump2_condtion = "BAD";
                Alert.NameCount = 8;
                Alert.Enable = true;
                SetAlertName("PUMP WEAK 2");
            }
        }
        else
        {
            SupervisoryTime.ELH_OL2 = PUMP_GOOD; // Set status to GOOD
        }
    }
}

/**********************
 * SUPERVISORY 3 OUTLET ELEMENT HEALTH FUNCTION
 * Assesses health of outlet 3 based on supervisory data
 **********************/
void SUPERVISORYFUNCTION::_supervisory3_Outlet_Element_Health()
{
    if (Config.NoOfOutlet > 2) // Check if outlet 3 is configured
    {
        if (SupervisoryTime.AWC_SUP3 > 0 && SupervisoryTime.NWC_SUP3 > 0) // Both counts valid (NWC guards the divide)
        {
            if (SupervisoryTime.AWC_SUP3 > SupervisoryTime.NWC_SUP3) // Ensure average working count does not exceed nominal working count
            {
                SupervisoryTime.AWC_SUP3 = SupervisoryTime.NWC_SUP3; // Cap average working count to nominal working count
            }
            _pump3_percentage = (SupervisoryTime.AWC_SUP3 / SupervisoryTime.NWC_SUP3) * 100; // Calculate percentage difference

            if (_pump3_percentage >= 90 && _pump3_percentage <= 100) // Excellent condition
            {
                SupervisoryTime.ELH_OL3 = PUMP_GOOD; // Set status to GOOD
                _pump3_condtion = "GOOD";
            }
            else if (_pump3_percentage >= 75 && _pump3_percentage < 90) // Average condition
            {
                SupervisoryTime.ELH_OL3 = PUMP_AVERAGE; // Set status to AVERAGE
                _pump3_condtion = "AVG";
            }
            else if (_pump3_percentage < 75) // Poor condition
            {
                SupervisoryTime.ELH_OL3 = PUMP_BAD; // Set status to BAD
                _pump3_condtion = "BAD";
                Alert.NameCount = 9;
                Alert.Enable = true;
                SetAlertName("PUMP WEAK 3");
            }
        }
        else
        {
            SupervisoryTime.ELH_OL3 = PUMP_GOOD; // Set status to GOOD
        }
    }
}
