/**********************
 * BLUETOOTH COMMUNICATION HEADER
 * Defines BLE interface for device configuration and data exchange
 * Handles WiFi credentials, system parameters, and device identification
 **********************/

#ifndef __BLUETOOTH_SCANNERH__
#define __BLUETOOTH_SCANNERH__

#include <Arduino.h>   // Arduino core
#include <BLEDevice.h> // BLE device
#include <BLEUtils.h>  // BLE utilities
#include <BLEServer.h> // BLE server

/**********************
 * BLUETOOTH CONFIGURATION
 **********************/
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"        // BLE service UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" // BLE characteristic UUID

// Connection status enum
enum BluetoothStatus {
    BLUETOOTH_DISCONNECTED,
    BLUETOOTH_CONNECTING,
    BLUETOOTH_CONNECTED,
    BLUETOOTH_ERROR
};

// External BLE objects
extern BLEServer *pServer;                 // Global BLE server pointer
extern BLECharacteristic *pCharacteristic; // Global BLE characteristic pointer

// Forward declaration for friend class
class MyServerCallbacks;

// Bluetooth processing class
class BLUETOOTHPROCESS
{
public:
    void enable();               // Enable BLE
    void send(String _message);  // Send data
    String read();               // Read data
    void name(String deviceName); // Set custom device name
    void connect();              // Connect/start advertising
    void disconnect();           // Disconnect/stop advertising
    bool CheckBluetoothConnect();
    String getName();
    BluetoothStatus status();    // Get connection status
    
    // Friend class to allow access to private members
    friend class MyServerCallbacks;
    
private:
    void _setBluetoothName();    // Generate unique device name
    
    String _customName = "";     // Custom device name
    bool _isConnected = false;   // Connection status flag
    bool _isAdvertising = false; // Advertising status flag
};

extern BLUETOOTHPROCESS Bluetooth; // Global instance

#endif