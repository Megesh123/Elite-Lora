#include "Headerfile.h"

TwowayProcessVariable Lora_twp;
TwowayProcessFunction Twoway;

void TwowayProcessFunction::Process()
{

    const int Interval = 3000;
    static long PerMillis = 0;

    if (millis() - PerMillis < Interval)
        return;

    PerMillis = millis();

    if (Lora_twp.TestRunOn)
    {
        LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.TestRunStart);
    }
    else if (Lora_twp.TestRunOff)
    {
        LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.TestRunStop);
    }
    else if (Lora_twp.SleepModeOn)
    {
        LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.SleepModeStart);
    }
    else if (Lora_twp.SleepModeOff)
    {
        LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.SleepModeStop);
    }
    else if (Lora_twp.RefillAlertEnableDisable)
    {
        if (Alert.EnableDisable.RefillAlert)
        {
            LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.Enable);
        }
        else
        {
            LoRa.publish(LoraTopics.ConMode, PlayLoadMessage.Disable);
        }
    }
    else if (Lora_twp.SupervisoryAlertEnableDisable)
    {
        String data;

        data = "*";
        data += Alert.EnableDisable.Supervisory1 ? "ENABLE," : "DISABLE,";
        data += Alert.EnableDisable.Supervisory2 ? "ENABLE," : "DISABLE,";
        data += Alert.EnableDisable.Supervisory3 ? "ENABLE" : "DISABLE";
        data += "#";

        LoRa.publish(LoraTopics.ConMode, data);
    }
    else if (Lora_twp.RefillSuccessful)
    {
        LoRa.publish(LoraTopics.ConMode, Lora_twp.RefillSuccessful_Data);
    }
    else if (Lora_twp.AlertClear)
    {
        LoRa.publish(LoraTopics.ConMode, Lora_twp.AlertClear_Data);
    }
}

void TwowayProcessFunction::Return()
{
    if (Lora_twp.TestRunOn)
    {
        Lora_twp.TestRunOn = false;
    }
    else if (Lora_twp.TestRunOff)
    {
        Lora_twp.TestRunOff = false;
    }
    else if (Lora_twp.SleepModeOn)
    {
        Lora_twp.SleepModeOn = false;
    }
    else if (Lora_twp.SleepModeOff)
    {
        Lora_twp.SleepModeOff = false;
    }
    else if (Lora_twp.RefillAlertEnableDisable)
    {
        Lora_twp.RefillAlertEnableDisable = false;
    }
    else if (Lora_twp.SupervisoryAlertEnableDisable)
    {
        Lora_twp.SupervisoryAlertEnableDisable = false;
    }
    else if (Lora_twp.RefillSuccessful)
    {
        Lora_twp.RefillSuccessful = false;
        Lora_twp.RefillSuccessful_Data = "";
        Serial.println("Refill Successful Data Cleared");
    }
    else if (Lora_twp.AlertClear)
    {
        Lora_twp.AlertClear = false;
        Lora_twp.AlertClear_Data = "";
    }
}