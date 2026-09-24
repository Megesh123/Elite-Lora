#include "Headerfile.h"

char _buffer[100]; // Buffer for LoRa messages

void LoRaFunction::PumpStatus()
{
    if (Supervisory._pump_status_send_lora)
    {
        _pump_status_outlet_1();
        _pump_status_outlet_2();
        _pump_status_outlet_3();

        LoRa.publish(LoraTopics.PumpStatus, _buffer); // Publish pump status to LoRa topic

        Supervisory._pump_status_send_lora = false; // Reset flag after sending status
    }
}

void LoRaFunction::_pump_status_outlet_1()
{
    if (Config.NoOfOutlet == 1)
    {
        snprintf(_buffer, sizeof(_buffer), "PUMP-1-%s-%.0f",
                 _pump1_condtion.c_str(), _pump1_percentage);
    }
}

void LoRaFunction::_pump_status_outlet_2()
{
    if (Config.NoOfOutlet == 2)
    {
        snprintf(_buffer, sizeof(_buffer), "PUMP-1-%s-%.0f,PUMP-2-%s-%.0f",
                 _pump1_condtion.c_str(), _pump1_percentage,
                 _pump2_condtion.c_str(), _pump2_percentage);
    }
}

void LoRaFunction::_pump_status_outlet_3()
{
    if (Config.NoOfOutlet == 3)
    {
        snprintf(_buffer, sizeof(_buffer), "PUMP-1-%s-%.0f,PUMP-2-%s-%.0f,PUMP-3-%s-%.0f",
                 _pump1_condtion.c_str(), _pump1_percentage,
                 _pump2_condtion.c_str(), _pump2_percentage,
                 _pump3_condtion.c_str(), _pump3_percentage);
    }
}
