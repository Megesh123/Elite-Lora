/**********************
 * BLUETOOTH COMMUNICATION HEADER
 * Defines BLE interface for device configuration and data exchange
 * Handles WiFi credentials, system parameters, and device identification
 **********************/

#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include <Arduino.h> // Include Arduino core functionality

/**********************
 * BLUETOOTHPROCESSFUNCTION CLASS
 * Manages Bluetooth Low Energy (BLE) operations and data processing
 **********************/
class BLUETOOTHPROCESSFUNCTION
{
public:
    void Function();           // Main function to handle BLE operations
    bool Process(String data); // Parse+validate BLE config; true only if committed
    void Reset();              // Reset system values after new configuration
    void Process();

private:
    bool _messageProcess(String data); // Parse+validate lora config; true on success
    void _return();                    // Send success/error response back via BLE
};

extern BLUETOOTHPROCESSFUNCTION BluetoothFun; // Global instance of BLUETOOTHPROCESSFUNCTION

#endif /* __BLUETOOTH_H__ */