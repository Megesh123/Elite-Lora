/**********************
 * BLUETOOTH COMMUNICATION IMPLEMENTATION
 * Handles BLE connectivity, data parsing, and device configuration
 **********************/

#include <Arduino.h>    // Arduino core functionality
#include <BLEDevice.h>  // BLE device library
#include <BLEUtils.h>   // BLE utilities
#include <BLEServer.h>  // BLE server functionality
#include "BLEScanner.h" // Project BLE header

BLUETOOTHPROCESS Bluetooth; // Global instance

// Global BLE objects
BLEServer *pServer = nullptr;                 // BLE server pointer
BLECharacteristic *pCharacteristic = nullptr; // BLE characteristic pointer
String _clientid;                             // Unique client/device ID
String _incomingData;                         // Buffer for raw incoming data
String __incomingDataAfterFilter;             // Processed data after filtering
bool _messageReceive = false;                 // Flag indicating new message received

/**********************
 * BLE SERVER CALLBACKS
 **********************/
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override
    {
        Bluetooth._isConnected = true;
        // Serial.println("Client connected");
        pCharacteristic->setValue(_clientid.c_str()); // Send device ID to client
        pCharacteristic->notify();                    // Notify client
    }

    void onDisconnect(BLEServer *pServer) override
    {
        Bluetooth._isConnected = false;
        // Serial.println("Client disconnected");
        BLEAdvertising *pAdvertising = pServer->getAdvertising(); // Restart advertising
        pAdvertising->start();
        Bluetooth._isAdvertising = true;
    }
};

/**********************
 * BLE CHARACTERISTIC CALLBACKS
 **********************/
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override // On client write
    {
        String value = String(pCharacteristic->getValue().c_str()); // Convert bytes to String
        _incomingData += value;                                     // Append to buffer

        // Message must start with '*' and end with '#'
        if (_incomingData.startsWith("*") && _incomingData.endsWith("#"))
        {
            _incomingData = _incomingData.substring(1, _incomingData.length() - 1); // Remove markers
            _incomingData.trim();                                                   // Remove whitespace
            __incomingDataAfterFilter = _incomingData;                              // Store filtered message
            _messageReceive = true;

            _incomingData = ""; // Clear buffer
        } // Mark new message received
    }
};

/**********************
 * ENABLE BLE
 **********************/
void BLUETOOTHPROCESS::enable()
{
    _setBluetoothName();                                    // Set unique BLE device name
    Serial.println("Bluetooth Name: " + String(_clientid)); // Log Bluetooth name
    pServer = BLEDevice::createServer();                    // Create BLE server
    pServer->setCallbacks(new MyServerCallbacks());         // Register server callbacks

    BLEService *pService = pServer->createService(SERVICE_UUID); // Create BLE service

    // Create read/write characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->setCallbacks(new MyCallbacks()); // Register characteristic callback
    pService->start();                                // Start service

    connect(); // Start advertising automatically
}

// Send data to connected client
void BLUETOOTHPROCESS::send(String _message)
{
    if (_isConnected)
    {
        pCharacteristic->setValue(_message.c_str());
        pCharacteristic->notify();
    }
}

// Read received message
String BLUETOOTHPROCESS::read()
{
    if (_messageReceive) // If message available
    {
        String msg = __incomingDataAfterFilter; // Copy message
        __incomingDataAfterFilter = "";         // Clear buffer
        _messageReceive = false;                // Reset flag
        return msg;                             // Return message
    }
    else
    {
        return ""; // No message
    }
}

// Set custom Bluetooth device name
void BLUETOOTHPROCESS::name(String deviceName)
{
    _customName = deviceName;
}

// Start Bluetooth advertising
void BLUETOOTHPROCESS::connect()
{
    if (pServer && !_isAdvertising)
    {
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
        _isAdvertising = true;
        _isConnected = false; // Reset connection status when starting advertising
    }
}
// Start Bluetooth advertising
bool BLUETOOTHPROCESS::CheckBluetoothConnect()
{
    return Bluetooth._isConnected;
}
// Stop Bluetooth advertising and disconnect clients
void BLUETOOTHPROCESS::disconnect()
{
    if (pServer)
    {
        // Stop advertising first
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->stop();
        _isAdvertising = false;

        // Disconnect any connected clients
        // Note: BLEServer doesn't have disconnectAll(), so we rely on stopping advertising
        _isConnected = false;
    }
}

// Get Bluetooth connection status
BluetoothStatus BLUETOOTHPROCESS::status()
{
    if (!_isAdvertising)
    {
        return BLUETOOTH_DISCONNECTED;
    }
    else if (_isAdvertising && !_isConnected)
    {
        return BLUETOOTH_CONNECTING;
    }
    else if (_isConnected)
    {
        return BLUETOOTH_CONNECTED;
    }
    else
    {
        return BLUETOOTH_ERROR;
    }
}

// Set BLE device name using ESP chip ID or custom name
void BLUETOOTHPROCESS::_setBluetoothName()
{

    uint64_t chipId = ESP.getEfuseMac();                     // ESP32 MAC address
    String uniqueId = String((uint32_t)(chipId >> 32), HEX); // Upper 32 bits
    String chipIdStr = String((uint32_t)chipId, HEX);        // Lower 32 bits

    uniqueId.toUpperCase();  // Uppercase format
    chipIdStr.toUpperCase(); // Uppercase format

    _clientid = uniqueId + chipIdStr; // Combine to make device ID

    String bluetoothName;
    if (_customName != "")
    {
        bluetoothName = _customName; // Use custom name if set
    }
    else
    {
        bluetoothName = String(_clientid); // Use MAC-based name with prefix
        // Serial.println("Generated Bluetooth Name: " + bluetoothName);
    }

    BLEDevice::init(bluetoothName.c_str()); // Init BLE with device name
    BLEDevice::setMTU(512);                 // Set MTU size (max 512)
}
String BLUETOOTHPROCESS::getName()
{
    uint64_t chipId = ESP.getEfuseMac();                     // ESP32 MAC address
    String uniqueId = String((uint32_t)(chipId >> 32), HEX); // Upper 32 bits
    String chipIdStr = String((uint32_t)chipId, HEX);        // Lower 32 bits

    uniqueId.toUpperCase();  // Uppercase format
    chipIdStr.toUpperCase(); // Uppercase format

    _clientid = uniqueId + chipIdStr; // Combine to make device ID

    return _clientid;
}