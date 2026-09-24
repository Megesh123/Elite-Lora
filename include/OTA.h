/**
 * @file OTA.h
 * @brief OTA Update Support for WIFI
 */

#ifndef __OTA_H__
#define __OTA_H__

#include <Arduino.h>

class OTA_FUNCTION
{
public:
    OTA_FUNCTION(const char *currentVersion,
                 const char *versionUrl,
                 const char *firmwareUrl);

    void checkAndUpdate();

private:
    const char *currentVersion;
    const char *versionURL;
    const char *firmwareURL;

    /*
     * WIFI OTA
     */
    String getServerVersion_WIFI();
    void performOTA_WIFI();
};
extern bool ChechOTAUpdate;
extern bool ota_process_1;
extern bool ota_process_2;
extern bool ota_process_3;
extern bool ota_process_4;
#endif