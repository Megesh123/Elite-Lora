/**
 * @file OTA.cpp
 * @brief WIFI OTA Implementation
 */

#include "OTA.h"
#include "Headerfile.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define OTA_BUFFER_SIZE 2048

int _oldProgress = 0;
bool ota_process_1 = false;
bool ota_process_2 = false;
bool ota_process_3 = false;
bool ota_process_4 = false;

bool ChechOTAUpdate;

const char *ssid = "Esp8266";
const char *password = "123123123";

OTA_FUNCTION::OTA_FUNCTION(const char *currentVersion,
                           const char *versionUrl,
                           const char *firmwareUrl)
{
    this->currentVersion = currentVersion;
    this->versionURL = versionUrl;
    this->firmwareURL = firmwareUrl;
}

/**************************************************************
 * MAIN OTA CHECK
 **************************************************************/
void OTA_FUNCTION::checkAndUpdate()
{
    /*
     * WIFI OTA
     */
    if (ChechOTAUpdate)
    {
        Serial.println("\n Connecting to wifi");
        WiFi.mode(WIFI_STA);
        if (LoraDetails.Wifi.length() > 0 && LoraDetails.Pass.length() > 0)
        {
            Serial.println("wifi-connected - 1");
            WiFi.begin(LoraDetails.Wifi, LoraDetails.Pass);
        }
        else
        {
            Serial.println("wifi-connected - 2");
            WiFi.begin(ssid, password);
        }

        const uint32_t wifiStart = millis();
        while (WiFi.status() != WL_CONNECTED)
        {
            if (millis() - wifiStart >= 30000UL) // 30s cap - never hang the boot
            {
                Serial.println("\nWiFi connect timeout - aborting OTA");
                ChechOTAUpdate = false;
                Eeprom.write(); // Persist so we don't re-enter the OTA path on next boot
                delay(1000);
                ESP.restart();
                return;
            }
            delay(500);
            Serial.print(".");
        }

        Serial.println("\nWiFi Connected");
        Serial.println(WiFi.localIP());
        ota_process_1 = true;
        Serial.println("OTA CHECK");
        delay(10);

        String serverVersion = getServerVersion_WIFI();
        Serial.printf("SERVER VERSION: %s\n", serverVersion.c_str());
        Serial.printf("CURRENT VERSION: %s\n", currentVersion);

        if (serverVersion.length() > 0 &&
            serverVersion != currentVersion)
        {
            OTA.current_version = serverVersion;
            ota_process_2 = true;
            Serial.println("NEW OTA FOUND");
            delay(10);
            OTA.Enable = true;
            delay(10);
            performOTA_WIFI();
        }
        else if (serverVersion.length() > 0)
        {
            OTA.current_version = serverVersion;
            ota_process_3 = true;
            Serial.println("OTA UP TO DATE");
            delay(10);
        }
        else
        {
            ota_process_4 = true;
            Serial.println("OTA CHECK FAILED");
        }

        OTA.current_version = serverVersion;
        ChechOTAUpdate = false;
        Eeprom.write();
        delay(5000);
        ESP.restart();
    }
}

/**************************************************************
 **************************************************************
                        WIFI OTA
 **************************************************************
 **************************************************************/

/*
 * GET SERVER VERSION - WIFI
 */
String OTA_FUNCTION::getServerVersion_WIFI()
{
    HTTPClient http;

    http.begin(versionURL);

    int httpCode = http.GET();

    String version = "";

    if (httpCode == HTTP_CODE_OK)
    {
        version = http.getString();

        version.trim();

        Serial.println("WIFI SERVER VERSION: " + version);
    }
    else
    {
        Serial.printf("WIFI VERSION CHECK FAILED: %d\n", httpCode);
    }

    http.end();

    return version;
}

/*
 * OTA UPDATE - WIFI
 */
void OTA_FUNCTION::performOTA_WIFI()
{
    Serial.println("START WIFI OTA");

    HTTPClient http;

    http.begin(firmwareURL);

    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("WIFI OTA HTTP FAILED: %d\n", httpCode);

        http.end();

        return;
    }

    int contentLength = http.getSize();

    if (contentLength <= 0)
    {
        Serial.println("INVALID CONTENT LENGTH");

        http.end();

        return;
    }

    bool canBegin = Update.begin(contentLength);

    if (!canBegin)
    {
        Serial.println("NOT ENOUGH SPACE");

        http.end();

        return;
    }

    WiFiClient *stream = http.getStreamPtr();

    uint8_t buffer[OTA_BUFFER_SIZE];

    int written = 0;
    uint32_t lastProgressMs = millis(); // Watchdog for a stalled download stream

    while (http.connected() && written < contentLength)
    {
        if (millis() - lastProgressMs >= 20000UL) // 20s with no new bytes -> give up
        {
            Serial.println("WIFI OTA STALLED - aborting");
            Update.abort();
            http.end();
            return;
        }

        size_t available = stream->available();

        if (available)
        {
            int readBytes = stream->readBytes(
                buffer,
                min(available, sizeof(buffer)));

            Update.write(buffer, readBytes);

            written += readBytes;
            lastProgressMs = millis();

            /*
             * LIVE OTA PROGRESS
             */
            OTA.progress =
                (written * 100) / contentLength;

            if (_oldProgress != OTA.progress)
            {
                _oldProgress = OTA.progress;
                Serial.printf("WIFI OTA: %d%%\n",
                              OTA.progress);
            }
        }

        delay(1);
    }

    if (Update.end())
    {
        if (Update.isFinished())
        {
            ChechOTAUpdate = false;
            Eeprom.write();
            Serial.println("WIFI OTA SUCCESS");
            Eeprom.write();
            delay(1000);

            ESP.restart();
        }
        else
        {
            Serial.println("WIFI OTA NOT FINISHED");
        }
    }
    else
    {
        Serial.printf("WIFI OTA ERROR: %d\n",
                      Update.getError());
    }

    http.end();
}

