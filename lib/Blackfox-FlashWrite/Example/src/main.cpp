#include <Arduino.h>
#include "FlashWrite.h"
#include "Eeprom.h"
#include "System.h"

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);
  EEPROM.read();
  EEPROM.init();
  Serial.printf("num1: %.0f\n num2: %.0f\n num3: %.0f\n num4: %0.f\n", num1, num2, num3, num4);
  Serial.print("name");
  Serial.println(name);
  Serial.print("name2");
  Serial.println(name2);
}

void loop()
{
  if (Serial.available() > 0)
  {
    String data = Serial.readString();

    if (data.indexOf("write") != -1)
    {
      num1 = random(300);
      num2 = random(300);
      num3 = random(300);
      num4 = random(300);
      Serial.printf("num1: %.0f\n num2: %.0f\n num3: %.0f\n num4: %0.f\n", num1, num2, num3, num4);
      EEPROM.write();
    }

    if (data.indexOf("string") != -1)
    {
      name = "megesh";
      name2 = "i am from blackfox";
      EEPROM.write();
    }

    if (data.indexOf("clear") != -1)
    {
      name = "";
      name2 = "";
      EEPROM.write();
    }
  }

  delay(1);
}
