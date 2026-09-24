#include "Headerfile.h"

void _supervisory_Enable_Disable(String payload)
{

    // Remove starting '*' and ending '#'
    if (payload.startsWith("*SUPERVISORY,") && payload.endsWith("#"))
    {

        payload = payload.substring(13, payload.length() - 1);
        LoRa.response(LoraTopics.SupervisoryEnableDisable_return, Response.Ok); // Send acknowledgment

        // Extract tokens
        int firstComma = payload.indexOf(',');
        int secondComma = payload.indexOf(',', firstComma + 1);

        if (firstComma == -1 || secondComma == -1)
        {
            // Invalid format (not 3 values)
            return;
        }

        String val1 = payload.substring(0, firstComma);
        String val2 = payload.substring(firstComma + 1, secondComma);
        String val3 = payload.substring(secondComma + 1);

        // Normalize (optional but safer)
        val1.trim();
        val1.toLowerCase();
        val2.trim();
        val2.toLowerCase();
        val3.trim();
        val3.toLowerCase();

        // Convert to boolean
        bool s1 = (val1 == "disable");
        bool s2 = (val2 == "disable");
        bool s3 = (val3 == "disable");

        // Assign values
        Alert.EnableDisable.Supervisory1 = s1;
        Alert.EnableDisable.Supervisory2 = s2;
        Alert.EnableDisable.Supervisory3 = s3;
    }
}